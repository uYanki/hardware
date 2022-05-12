PROJECT = "pwm2"
VERSION = "1.0.0"

-- 一定要添加sys.lua !!!!
local sys = require "sys"
-- io10
-- https://wiki.luatos.com/api/pwm.html?highlight=pwm
sys.taskInit(function()
    print("<<<<!2!<<<<<<")
    -- while 1 do
    --     for i = 100, 5000, 100 do
    --         pwm.open(2, i, 50)
    --         sys.wait(1000)
    --         pwm.close(2)
    --         sys.wait(500)
    --         print(i)
    --     end
    --     sys.wait(1000)
    -- end

    ch = pwm2.init(0, 0, 10, 10, 1 * 1000, 1)
    pwm2.setFadeWithTime(ch, 1024, 10000, 1)

    -- while 1 do
    --     -- log.info("pwm", "channel", ch)
    --     pwm2.setFadeWithTime(ch, 1024, 10000, 1)
    -- end

end)

-- 用户代码已结束---------------------------------------------
-- 结尾总是这一句
sys.run()
-- sys.run()之后后面不要加任何语句!!!!!
