_G.sys = require("sys")

MPU6050_ADDRESS_AD0_LOW = 0x68
MPU6050_ADDRESS_AD0_HIGH = 0x69
local MPU6050_WHO_AM_I = 0x75

-- 加速度计Accelerometer,陀螺仪Gyroscope,磁力计Magnetometer

local mpu6050_accel_scale = {16384, 8192, 4096, 2048} -- +/- 2,4,8,16 g
local mpu6050_gyro_scale = {131.0, 65.5, 32.8, 16.4} -- +/- 250,500,1000,2000 degrees/sec

-- local MPU6050_ACCEL_SENSITIVE = 1
-- local MPU6050_GYRO_SENSITIVE = 1
local MPU6050_ACCEL_SENSITIVE = mpu6050_accel_scale[1]
local MPU6050_GYRO_SENSITIVE = mpu6050_gyro_scale[4]
local MPU6050_TEMP_SENSITIVE = 340
local MPU6050_TEMP_OFFSET = 36.5

local MPU6050_PWR_MGMT_1 = 0x6B -- 电源管理
local MPU6050_PWR_MGMT_2 = 0x6C
local MPU6050_SMPLRT_DIV = 0x19 -- 采样频率
local MPU6050_CONFIG = 0x1A -- 低通滤波器
local MPU6050_GYRO_CONFIG = 0x1B -- 陀螺仪自检和量程
local MPU6050_ACCEL_CONFIG = 0x1C -- 加速度计自检和量程

local MPU6050_ACCEL_XOUT = 0x3B
local MPU6050_ACCEL_YOUT = 0x3D
local MPU6050_ACCEL_ZOUT = 0x3F
local MPU6050_TEMP_OUT = 0x41
local MPU6050_GYRO_XOUT = 0x43
local MPU6050_GYRO_YOUT = 0x45
local MPU6050_GYRO_ZOUT = 0x47

local MPU6050_FIFO_EN = 0x23
local MPU6050_INT_ENABLE = 0x38
local MPU6050_USER_CTRL = 0x6A

mpu6050 = {i2c_id = 0, i2c_addr = MPU6050_ADDRESS_AD0_LOW}

--[[
    【lua】
    - OOP: https://www.runoob.com/lua/lua-object-oriented.html
    【luatos】
    - i2c:https://wiki.luatos.com/api/i2c.html
    【mpu6050】
    - docin 姿态解算:https://www.docin.com/p-1806017607.html
    - zhihu mpu6050: https://zhuanlan.zhihu.com/p/228805569
    -                https://www.zhihu.com/question/68156189 【好】
    - bilibili:https://www.bilibili.com/video/BV1sL411F7fu?p=4
    -          https://www.bilibili.com/read/cv5745068/
    -          https://www.bilibili.com/read/cv5758469/
    - csdn:https://blog.csdn.net/qq_38091632?type=blog
    - zhihu:https://zhuanlan.zhihu.com/p/195683958
    - weixin-accel:https://mp.weixin.qq.com/s/3H0Z_8wCPAemRcwDW1vMwg
    - weixin-gyro:https://mp.weixin.qq.com/s/U0_DuxeFuwohLHcjQlUf5w
    - 野火:https://doc.embedfire.com/module/module_tutorial/zh/latest/Module_Manual/iic_class/mpu6050.html
    - blog:https://www.cnblogs.com/qsyll0916/p/8030379.html【好】
--]]

function mpu6050:init(obj)
    obj = obj or {}
    setmetatable(obj, {__index = self})
    -- init mpu6050
    if i2c.setup(self.i2c_id, i2c.FAST, self.i2c_addr) == 1 then
        self:write({MPU6050_PWR_MGMT_1, 0x80}) -- reset
        sys.wait(100) -- delay 100ms
        self:write({MPU6050_PWR_MGMT_1, 0x00}) -- wake up
        self:write({MPU6050_SMPLRT_DIV, 0x07}) -- fs = 125Hz
        self:write({MPU6050_CONFIG, 0x07}) -- LPF = 5Hz
        self:write({MPU6050_GYRO_CONFIG, 0x18}) -- gyro 16.4 LSB/(deg/s) -> 2000deg/s
        self:write({MPU6050_ACCEL_CONFIG, 0x01}) -- accel 16384 LSB/g -> 2g
        log.info("mpu6050 init finished")
        -- set other params
        self:write({MPU6050_FIFO_EN, 0x00}) -- disable fifo
        self:write({MPU6050_INT_ENABLE, 0x00}) -- disable interrupt
        self:write({MPU6050_USER_CTRL, 0x00}) -- disable i2c master
        self:write({MPU6050_PWR_MGMT_2, 0x00}) -- enable accel & gyro
        return obj
    end
    -- init failed
    log.error("mpu6050 init failed")
    return nil
end

function mpu6050:read(reg, len)
    -- len: count of bytes
    i2c.send(self.i2c_id, self.i2c_addr, reg)
    return string.unpack(">h", i2c.recv(self.i2c_id, self.i2c_addr, len))
end

function mpu6050:write(data) i2c.send(self.i2c_id, self.i2c_addr, data) end

-- function mpu6050:check() return self:read(MPU6050_WHO_AM_I, 1) end

-- function mpu6050:selftest(times)

--     local zero_gyro = {x = 0, y = 0, z = 0}
--     local zero_accel = {x = 0, y = 0, z = 0}
--     local accel, gyro
--     -- selftest
--     times = times or 500
--     for i = times, 1, -1 do
--         accel = self:accel()
--         zero_accel.x = zero_accel.x + accel.x
--         zero_accel.y = zero_accel.y + accel.y
--         zero_accel.z = zero_accel.z + accel.z
--         gyro = self:gyro()
--         zero_gyro.x = zero_gyro.x + gyro.x
--         zero_gyro.y = zero_gyro.y + gyro.y
--         zero_gyro.z = zero_gyro.z + gyro.z
--     end
--     zero_accel.x = zero_accel.x / times
--     zero_accel.y = zero_accel.y / times
--     zero_accel.z = zero_accel.z / times
--     zero_gyro.x = zero_gyro.x / times
--     zero_gyro.y = zero_gyro.y / times
--     zero_gyro.z = zero_gyro.z / times
--     self.zero_accel = zero_accel
--     self.zero_gyro = zero_gyro
--     self.angle = {pitch = 0, roll = 0, yaw = 0}
-- end

function mpu6050:accel() -- accelerate
    return {
        x = self:read(MPU6050_ACCEL_XOUT, 2) / MPU6050_ACCEL_SENSITIVE,
        y = self:read(MPU6050_ACCEL_YOUT, 2) / MPU6050_ACCEL_SENSITIVE,
        z = self:read(MPU6050_ACCEL_ZOUT, 2) / MPU6050_ACCEL_SENSITIVE
    }
end

function mpu6050:gyro() -- gyroscope
    return {
        x = self:read(MPU6050_GYRO_XOUT, 2) / MPU6050_GYRO_SENSITIVE,
        y = self:read(MPU6050_GYRO_YOUT, 2) / MPU6050_GYRO_SENSITIVE,
        z = self:read(MPU6050_GYRO_ZOUT, 2) / MPU6050_GYRO_SENSITIVE
    }
end

function mpu6050:temp() -- temperature
    return self:read(MPU6050_TEMP_OUT, 2) / MPU6050_TEMP_SENSITIVE +
               MPU6050_TEMP_OFFSET
end

function mpu6050:log()
    local accel = self:accel()
    log.info("[accel]:", accel.x, accel.y, accel.z)

    local gyro = self:gyro()
    log.info("[gyro]:", gyro.x, gyro.y, gyro.z)

    local temp = self:temp()
    log.info("[temp]:", temp)
end

--------------------- angle ---------------------

local RAD_TO_DEG = 57.2958

-- 俯仰pitch(绕y)、滚转roll(绕x)、偏航yaw(绕y)

function mpu6050:angle_self() -- euler angle (以自身为参考系)
    local angle = {pitch = nil, roll = nil, yaw = 0}
    local accel = self:accel()
    angle.pitch = math.atan(accel.x, accel.z) * RAD_TO_DEG
    angle.roll = math.atan(accel.y, accel.z) * RAD_TO_DEG
    return angle
end

function mpu6050:angle_world() -- euler angle (以世界为参考系)
    local angle = {pitch = nil, roll = nil, yaw = 0}
    local accel = self:accel()
    angle.pitch = -math.atan(accel.x, math.sqrt(accel.y ^ 2, accel.z ^ 2)) *
                      RAD_TO_DEG
    angle.roll = math.atan(accel.y, accel.z) * RAD_TO_DEG
    return angle
end

--------------------- kalman ---------------------

--[[
    kalman_struct = {
        Q_angle = 0.1, -- 加速计的过程噪声协方差(custom)
        Q_bias = 0.003, -- 陀螺仪偏差的过程噪声协方差(custom)
        R_measure = 0.03, -- 测量噪声协方差(custom)
        angle = 0, -- 由卡尔曼滤波器计算的角度
        bias = 0, -- 由卡尔曼滤波器计算的偏差
        P = {{0, 0}, {0, 0}} -- 误差协方差矩阵
    },
--]]

mpu6050.kalman = {
    pitch = {
        Q_angle = 0.1,
        Q_bias = 0.003,
        R_measure = 0.03,
        angle = 0,
        bias = 0,
        P = {{0, 0}, {0, 0}}
    },
    roll = {
        Q_angle = 0.1,
        Q_bias = 0.003,
        R_measure = 0.03,
        angle = 0,
        bias = 0,
        P = {{0, 0}, {0, 0}}
    }
}

local lasttime = 0
function delta_time() -- 计算微分时间
    local now = os.clock() -- 单位 s
    -- os.time() 精度 s
    -- os.clock() 精度 ms
    local dt = now - lasttime -- s
    lasttime = now
    return dt
end

function kalman_filter(Kalman, newAngle, newRate, dt)

    local rate = newRate - Kalman.bias
    Kalman.angle = Kalman.angle + dt * rate

    Kalman.P[1][1] = Kalman.P[1][1] + dt *
                         (dt * Kalman.P[2][2] - Kalman.P[1][2] - Kalman.P[2][1] +
                             Kalman.Q_angle)
    Kalman.P[1][2] = Kalman.P[1][2] - dt * Kalman.P[2][2]
    Kalman.P[2][1] = Kalman.P[2][1] - dt * Kalman.P[2][2]
    Kalman.P[2][2] = Kalman.P[2][2] + Kalman.Q_bias * dt

    local S = Kalman.P[1][1] + Kalman.R_measure
    local K = {nil, nil}
    K[1] = Kalman.P[1][1] / S
    K[2] = Kalman.P[2][1] / S

    local y = newAngle - Kalman.angle
    Kalman.angle = Kalman.angle + K[1] * y
    Kalman.bias = Kalman.bias + K[2] * y

    local P00_temp = Kalman.P[1][1]
    local P01_temp = Kalman.P[1][2]

    Kalman.P[1][1] = Kalman.P[1][1] - K[1] * P00_temp
    Kalman.P[1][2] = Kalman.P[1][2] - K[1] * P01_temp
    Kalman.P[2][1] = Kalman.P[2][1] - K[2] * P00_temp
    Kalman.P[2][2] = Kalman.P[2][2] - K[2] * P01_temp

    return Kalman.angle
end

mpu6050.yaw = 0
function mpu6050:angle_with_kalamn_filter(angle, dt) -- angle with kalman_filter
    dt = dt or delta_time()
    local gyro = self:gyro()

    -- 通过积分计算yaw,漂移严重
    -- self.yaw = self.yaw + (gyro.z - self.zero_gyro.z) * dt
    self.yaw = self.yaw + gyro.z * dt
    if self.yaw < -360 then self.yaw = angle + 360 end
    if self.yaw > 360 then self.yaw = angle - 360 end

    return {
        pitch = kalman_filter(self.kalman.pitch, angle.pitch, gyro.x, dt),
        roll = kalman_filter(self.kalman.roll, angle.roll, gyro.y, dt),
        yaw = self.yaw
    }
end

