PROJECT = "demo_gpio"
VERSION = "1.0.0"

_G.sys = require "sys"

-- PA0=0 , PA1=1 , ... , PB0=16, PB1=17, ...

-- local LED = {green = 41, red = 42, blue = 40}
local LED = { green = pin.PB24, red = pin.PB25, blue = pin.PB26 } -- air103
-- local LED = {green = pin.PB0, red = pin.PB1, blue = pin.PB2} -- w806
local LEDs = { LED.red, LED.green, LED.blue }

-- API Refer: https://wiki.luatos.com/api/gpio.html

for i = 1, #LEDs do gpio.setup(LEDs[i], 1, gpio.PULLUP) end -- output mode & pull up
-- #LEDs: len of array

local key_boot = gpio.setup(pin.PA0, function(val) print("pin_irq", val) end, gpio.PULLUP, gpio.FALLING) -- falling-irq
-- io mode: output(0/1), input(nil), irq(function)
-- pull mode: gpio.PULLUP, gpio.PULLDOWN
-- edge: gpio.FALLING, gpio.RISING,  gpio.BOTH
-- gpio.value: gpio.HIGH(1) ,gpio.LOW(0)

local function led_toggle(pin) gpio.set(pin, math.abs(gpio.get(pin) - 1)) end

sys.taskInit(function()
    while 1 do
        for i = 1, #LEDs do
            led_toggle(LEDs[i]) -- toggle led
            sys.wait(500) -- delay
            log.info("pin val", key_boot()) -- pin value
        end
    end
end)

sys.timerLoopStart(function() print("leds toggle") end, 1500) -- timer

sys.run()
