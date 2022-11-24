#ifndef I2C_SCAN_DEV
#define I2C_SCAN_DEV

uint8_t data;
for (uint8_t i = 0; i < 255; ++i)
    if (HAL_I2C_Mem_Read(&hi2c1, i, 0, I2C_MEMADD_SIZE_8BIT, &data, 1, 0xff) == HAL_OK)
        printf("0x%02x ", i);
printf("\r\n");

#endif
