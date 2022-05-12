PROJECT = "adc"
VERSION = "1.0.0"

_G.sys = require "sys"

-- for i = 0, 4 do adc.open(i) end
-- for i = 0, 4 do log.info("adc" .. i, adc.read(i)) end

sys.taskInit(function()
    adc.open(1)
    while 1 do
        print("adc:" .. adc.read(1))
        sys.wait(500)
    end
end)

sys.run()
