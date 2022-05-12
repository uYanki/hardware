PROJECT = "ws2812"
VERSION = "1.0.0"

local sys = require "sys"
-- https://wiki.luatos.com/api/zbuff.html
-- local buff = zbuff.create({8, 3, 24})
-- buff:drawLine(1, 2, 3, 0x001234)
sys.taskInit(function()
    sensor.ws2812b(7, {0x00ffff}, 300, 700, 700, 700)
    -- sensor.ws2812b(pin, data, T0H, T0L, T1H, T1L)
    print("ookkkkk")
    while 1 do
        sys.wait(100)
        sys.wait(100)
    end
end)

-- 用户代码已结束---------------------------------------------
-- 结尾总是这一句
sys.run()
-- sys.run()之后后面不要加任何语句!!!!!
