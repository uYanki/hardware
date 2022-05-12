PROJECT = "demo_lcd(lvgl)"
VERSION = "1.0.0"

_G.sys = require("sys")

--[[
    【st7735 lcd】
    spi:https://wiki.luatos.com/api/spi.html
    lcd:https://wiki.luatos.com/api/lcd.html
    rgb2hex:https://www.xiao84.com/tools/103183.html
]]

lcd.init("st7735", {
    port = "device",
    pin_dc = 6,
    pin_rst = 10,
    pin_pwr = 11,
    direction = 0,
    w = 128,
    h = 160,
    xoffset = 2,
    yoffset = 2
}, spi.deviceSetup(2, 7, 0, 0, 8, 40000000, spi.MSB, 1, 1))

---[[
lcd.drawLine(20, 20, 150, 20, 0x001F)
lcd.drawRectangle(20, 40, 120, 70, 0xF800)
lcd.drawCircle(50, 50, 20, 0x0CE0)
lcd.setFont(lcd.font_opposansm8)
lcd.drawStr(40, 10, "luatos", 0X1234)
lcd.drawQrcode(0, 0, "lutos esp32c3", 3) -- Qrcode

-- 使用中文字体需开启位于 luat_conf_bsp.h 里的相应的宏
-- lcd.setFont(lcd.font_opposansm12_chinese)
-- lcd.drawStr(40, 40, "luatos", 0X4321)

-- 使用取模软件PCtoLCD2002,显示中文
lcd.drawXbm(40, 40, 16, 16,
            string.char(0x80, 0x00, 0x80, 0x00, 0x40, 0x01, 0x20, 0x02, 0x10,
                        0x04, 0x48, 0x08, 0x84, 0x10, 0x83, 0x60, 0x00, 0x00,
                        0xF8, 0x0F, 0x00, 0x08, 0x00, 0x04, 0x00, 0x04, 0x00,
                        0x02, 0x00, 0x01, 0x80, 0x00))
---]]

--[[
lvgl.init()
lvgl.disp_set_bg_color(nil, 0xFFFFFF)
local scr = lvgl.obj_create(nil, nil)
local btn = lvgl.btn_create(scr)
lvgl.obj_align(btn, lvgl.scr_act(), lvgl.ALIGN_CENTER, 0, 0)
local label = lvgl.label_create(btn)
lvgl.label_set_text(label, "LuatOS!")
lvgl.scr_load(scr)
---]]

sys.taskInit(function() while 1 do sys.wait(500) end end)

sys.run()
