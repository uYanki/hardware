
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"

#include "esp_wifi.h"
#include "nvs_flash.h"

// https://docs.espressif.com/projects/esp-idf/zh_CN/latest/esp32/api-reference/network/esp_now.html

#define WIFI_SSID "HUAWEI-Y6AZGD"
#define WIFI_PWD "13631520360"
#define WIFI_MAXIMUM_RETRY 4  // 最大重连次数

void _strcpy(char* dest, const char* src) {
    while ((*dest++ = *src++) != '\0') {}
}

enum {
    WIFI_CONNECTED_BIT = BIT0,
    WIFI_FAIL_BIT      = BIT1
};

static EventGroupHandle_t wifi_event_group_handler;  // 事件标志组

static void wifi_event_handle(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    static uint8_t retry_count = 0;  // 重连次数
    if (event_base == WIFI_EVENT) {
        if (event_id == WIFI_EVENT_STA_START) {
            esp_wifi_connect();
        } else if (event_id == WIFI_EVENT_STA_DISCONNECTED) {  // 失去连接
            if (retry_count++ < WIFI_MAXIMUM_RETRY) {
                printf("reconnect wifi ...%d\r\n", retry_count);
                esp_wifi_connect();  // 尝试重连
            } else {
                xEventGroupSetBits(wifi_event_group_handler, WIFI_FAIL_BIT);  //连接失败
            }
        }
    } else if (event_base == IP_EVENT) {
        if (event_id == IP_EVENT_STA_GOT_IP) {  // 连接成功
            ip_event_got_ip_t* event = (ip_event_got_ip_t*)event_data;
            printf("ip:" IPSTR "\r\n", IP2STR(&event->ip_info.ip));
            retry_count = 0;  // 重连次数清零
            xEventGroupSetBits(wifi_event_group_handler, WIFI_CONNECTED_BIT);
        }
    }
}

void wifi_init_sta(char ssid[32 + 1], char pwd[64 + 1]) {
    wifi_event_group_handler = xEventGroupCreate();

    esp_netif_init();  // 底层TCP/IP堆栈
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handle, NULL);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handle, NULL);  // 连接成功事件

    wifi_config_t wifi_cfg = {};  // 配置 WiFi
    _strcpy((char*)wifi_cfg.sta.ssid, ssid);
    _strcpy((char*)wifi_cfg.sta.password, pwd);
    wifi_cfg.sta.pmf_cfg.capable    = true;
    wifi_cfg.sta.pmf_cfg.required   = false;
    wifi_cfg.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;

    printf("ssid:%s\r\n", wifi_cfg.sta.ssid);
    printf("pwd:%s\r\n", wifi_cfg.sta.password);

    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(WIFI_IF_STA, &wifi_cfg);
    esp_wifi_start();  // 触发连接

    EventBits_t bits = xEventGroupWaitBits(
        wifi_event_group_handler,
        WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,  // 需等待的事件
        pdFALSE, pdFALSE, portMAX_DELAY);

    if (bits & WIFI_CONNECTED_BIT) {
        printf("success to connect to SSID: %s\r\n", ssid);
    } else if (bits & WIFI_FAIL_BIT) {
        printf("fail to connect to SSID: %s\r\n", ssid);
    }

    esp_event_handler_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handle);
    esp_event_handler_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handle);
    vEventGroupDelete(wifi_event_group_handler);
}

extern "C" void app_main(void) {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
        ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        nvs_flash_init();
    }

    nvs_handle hNVS;  // 非易失性存储(NVS), 即数据掉电时不丢失
    nvs_open("wifi_cfg", NVS_READWRITE, &hNVS);
#if 0
    // 保存 ssid 和 pwd
    nvs_set_str(hNVS, "ssid", WIFI_SSID);
    nvs_set_str(hNVS, "pwd", WIFI_PWD);
    wifi_init_sta(WIFI_SSID, WIFI_PWD);
#else
    // 读取 ssid 和 pwd
    char   wifi_ssid[32 + 1] = {0};
    char   wifi_pwd[64 + 1]  = {0};
    size_t len;
    len = sizeof(wifi_ssid);
    nvs_get_str(hNVS, "ssid", wifi_ssid, &len);
    len = sizeof(wifi_pwd);
    nvs_get_str(hNVS, "pwd", wifi_pwd, &len);
    wifi_init_sta(wifi_ssid, wifi_pwd);
#endif
    nvs_commit(hNVS);
    nvs_close(hNVS);

    while (1) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

/*
①结构体初始化为空: typename varname = {};
②在 goto 之后不能定义变量
*/
