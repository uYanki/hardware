# hardware_maxibit_kendryte_k210

例程使用总结：

* kendryte-freertos-demo-develop.zip

① board_config.h 的配置

```c
#define OV5640 0
#define OV2640 1 // use this
#define BOARD_KD233 0
#define BOARD_LICHEEDAN 1 // use this
#define BOARD_K61 0
```

② maixbit 配的摄像头不是ov2640/ov5640，个人用的是之前买 esp32cam 带的 ov2640

③ maixbit 配的屏幕是 240*320 的，如果出现花屏，降低 spi 的速率即可

④ 关于缺失 iomem.h 的问题：

在 demo `k210_ov2640` 中的做法是：

将

```c
 static uint32_t* g_lcd_gram0;
 static uint32_t* g_lcd_gram1; 
```

改为

```c
uint32_t g_lcd_gram0[38400] __attribute__((aligned(64)));
uint32_t g_lcd_gram1[38400] __attribute__((aligned(64)));
```

在 demo `k210_face_detect` 中的做法是：

image_process.c 中的 iomem_alloc, iomem_free 改为 malloc, free

⑤ demo 带的人脸模型 kmodel 识别不到自己脸部的问题：

网络上搜索人物图像，对着他们识别即可

⑥使用 spi flash 内的模型：

- 使用 kflash 将 kmodel 写入到地址 0x00A00000 处 
（具体看 main.c 中的 w25qxx_read_data() 读取的位置）
- main.c 中定义 #define LOAD_KMODEL_FROM_FLASH 1

⑦ platformio 里编译说找不到 CMakeList.txt 是，就将项目放到桌面即可
