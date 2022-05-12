PROJECT = "adc"
VERSION = "1.0.0"

local sys = require "sys"

sys.taskInit(function()
    adc.open(0);
    while 1 do

        -- 输出一个值 采集到的原始值
        -- val = adc.read(0)
        -- print(val)

        -- 输出两个值 采集到的原始值 和 换算后的电压值(mV)
        print(adc.read(0))
        sys.wait(1000)

    end
end)

sys.run()
