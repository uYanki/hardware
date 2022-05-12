PROJECT = "gpio"
VERSION = "1.0.0"

_G.sys = require "sys"

local PIN_LEDs = {0, 1, 12, 18}
local PIN_KEYs = {2, 3, 10, 6, 7}

local LEDs, KEYs = {}, {}

for i = 1, #PIN_LEDs do LEDs[i] = gpio.setup(PIN_LEDs[i], 1) end
for i = 1, #PIN_KEYs do
    KEYs[i] = gpio.setup(PIN_KEYs[i], function(val) print("irq", val) end,
                         gpio.PULLUP, gpio.FALLING)
end

sys.taskInit(function()
    log.info(">>>>>>>>>>>>> start")
    while 1 do
        for i = 1, #LEDs do LEDs[i](1) end
        sys.wait(500)
        for i = 1, #LEDs do LEDs[i](0) end
        sys.wait(500)
    end
end)

sys.run()
