@echo off
reg query "HKEY_LOCAL_MACHINE\HARDWARE\DEVICEMAP\SERIALCOMM"
echo >>> please enter COM No
set /p comno=
echo >>> erase flash
esptool --chip esp32-C3 --port COM%comno% erase_flash
echo >>> write fireware
esptool --chip esp32-C3 --port COM%comno% --baud 460800 write_flash -z 0x0 esp32c3-20220117-v1.18.bin
echo >>> fireware info
esptool --chip esp32-c3 --port COM%comno% flash_id
echo >>> press any key to exit
pause
