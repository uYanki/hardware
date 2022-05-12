PROJECT = "HTTP"
VERSION = "1.0.0"

_G.sys = require("sys")

--[[
    gpio:https://wiki.luatos.com/api/gpio.html?highlight=gpio
    lcd:https://wiki.luatos.com/api/lcd.html
        https://wiki.luatos.com/peripherals/lcd_air10x/index.html
    json:https://wiki.luatos.com/api/json.html?highlight=json
    sys:https://wiki.luatos.com/api/sys.html
    rgb2hex (color):https://www.xiao84.com/tools/103183.html
    lua:https://c.runoob.com/compile/66/
]]

-- http

-- post & get 都是异步的写法, 同步的老出错

function http_get(url, http_cbk)
    local httpc = esphttp.init(esphttp.GET, url) -- 初始化请求
    if httpc then
        local ok, err = esphttp.perform(httpc, true) -- 发起请求
        if ok then
            while 1 do
                local result, c, ret, data = sys.waitUntil("ESPHTTP_EVT", 200)
                if c == httpc then
                    if esphttp.is_done(httpc, ret) then
                        break
                    end
                    if ret == esphttp.EVENT_ON_DATA and esphttp.status_code(httpc) == 200 then
                        http_cbk(data) -- do something...
                    end
                end
            end
        end
        esphttp.cleanup(httpc)
    end
end

function http_post(url, content, http_cbk)
    local httpc = esphttp.init(esphttp.POST, url) -- 初始化请求
    if httpc then
        esphttp.set_header(httpc, "Content-Type", "application/json") -- 设置请求头 (内容类型)
        esphttp.post_field(httpc, content) -- 设置请求体
        local ok, err = esphttp.perform(httpc, true) -- 发起请求
        if ok then
            while 1 do
                local result, c, ret, data = sys.waitUntil("ESPHTTP_EVT", 200)
                if c == httpc then
                    if esphttp.is_done(httpc, ret) then
                        break
                    end
                    if ret == esphttp.EVENT_ON_DATA and esphttp.status_code(httpc) == 200 then
                        http_cbk(data) -- do something...
                    end
                end
            end
        end
        esphttp.cleanup(httpc)
    end
end

-- http task


--[[ 注:
    ① 使用锁的方式防止频繁创建任务导致堆栈溢出
    ② http 需使用 sys.taskInit() 来进行调用
    ③ 需使用 sys.wait(50), 否则莫名其妙就报错
--]]

-- 目标地址是个人使用 flask + 花生壳 搭建的服务器的排行榜接口

http_lock = 0

function http_get_score()
    if http_lock == 0 then
        http_lock = 1
        sys.taskInit(function()
            sys.wait(50)
            http_get("http://205p4y2225.51vip.biz/api/top", function(data)
                data_json, result, err = json.decode(data)
                lcd.clear(0xFFFF)
                lcd.drawStr(10, 20, "time:" .. data_json.datetime, 0X4321)
                lcd.drawStr(10, 40, "name:" .. data_json.name, 0X1231)
                lcd.drawStr(10, 60, "score:" .. data_json.score, 0X1234)
            end)
            http_lock = 0
        end)
    end
end

function http_post_score()
    if http_lock == 0 then
        http_lock = 1
        sys.taskInit(function()
            sys.wait(50)
            random_num = tostring(esp32.random())
            http_post("http://205p4y2225.51vip.biz/api/rank/", "{\"name\":\"esp32c3\",\"score\":" .. random_num .. "}", function()
                lcd.clear(0xFFFF)
                lcd.drawStr(10, 30, "name:esp32c3", 0X1231)
                lcd.drawStr(10, 50, "score:" .. random_num, 0X1234)
            end)
            http_lock = 0
        end)
    end
end

-- Init Keys
local keys = {
    up = gpio.setup(8, function(val) lcd.invoff() end, gpio.PULLUP, gpio.FALLING),
    down = gpio.setup(13, function(val) lcd.invon() end, gpio.PULLUP, gpio.FALLING),
    -- left = gpio.setup(5, function(val) lcd.on() end, gpio.PULLUP, gpio.FALLING),
    -- right = gpio.setup(9, function(val) lcd.off() end, gpio.PULLUP, gpio.FALLING),
    -- center = gpio.setup(4, function(val) http_get_score() end, gpio.PULLUP, gpio.FALLING)
    left = gpio.setup(5, function(val) http_post_score() end, gpio.PULLUP, gpio.FALLING),
    right = gpio.setup(9, function(val) http_get_score() end, gpio.PULLUP, gpio.FALLING),
}

-- Init LCD
spi_lcd = spi.deviceSetup(2, 7, 0, 0, 8, 40000000, spi.MSB, 1, 1)
lcd.init("st7735s", {
    port = "device",
    pin_dc = 6,
    pin_rst = 10,
    pin_pwr = 11,
    direction = 2,
    w = 160,
    h = 80,
    xoffset = 0,
    yoffset = 24
}, spi_lcd)
-- 调整 xoffset & yoffset 来防止花屏, 这不是调点 (0,0) 的位置
lcd.setFont(lcd.font_opposansm8)


sys.taskInit(function()

    -- Init WIFI
    wlan.init()
    wlan.setMode(wlan.STATION)
    wlan.connect("HUAWEI-Y6AZGD", "xxx")
    result, data = sys.waitUntil("IP_READY")
    log.info("wlan", "IP_READY", result, data) -- log ip address
    lcd.drawStr(20, 20, "IP:" .. data, 0X1234) -- show ip address on screen

    -- Main Loop
    while true do
        sys.wait(1000)
    end

end)

sys.run()
