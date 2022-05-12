PROJECT = "IMU"
VERSION = "1.0.0"

_G.sys = require("sys")
require("mpu6050")

function print_angle(label, angle)
    log.info(label, angle.pitch, angle.roll, angle.yaw)
end

sys.taskInit(function()
    local imu = mpu6050:init()

    local times = 0
    ---[[
    local angle = {pitch = 0, roll = 0, yaw = 0}
    -- local accel = {x = 0, y = 0, z = 0}
    -- local gyro = {x = 0, y = 0, z = 0}

    while 1 do
        -- imu:log()
        -- sys.wait(1000)
        angle = imu:angle_with_kalamn_filter(imu:angle_self())
        times = times + 1
        if times == 20 then
            times = 0
            log.info(angle.pitch, angle.roll, angle.yaw)
        end
    end
    ---]]
end)

sys.run()
