
#ifndef LIB_SPI
#define LIB_SPI

#include <REGX52.H>

#ifndef _uint_
typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;
#define _uint_
#endif

/* mosi(output),miso(input),sclk(output) */

#ifndef _spi_io_
#define _spi_io_
#define SPI_MOSI P3_7
#define SPI_MISO P3_4
#define SPI_SCLK P3_6
#define SPI_CS P3_5
#endif

typedef struct spi {
    uint8_t mode;
    // sbit mosi, miso, sclk, cs;
    uint8_t firstbit;
    uint8_t cpol, cpha;
    uint8_t datasize;
} spi_t;

#define SPI_MODE_FULL 0  // 全双工模式(3线)
#define SPI_MODE_HALF 1  // 半双工模式(2线)

#define SPI_CPOL_LOW 0
#define SPI_CPOL_HIGH 1
#define SPI_CPHA_1EDGE 0
#define SPI_CPHA_2EDGE 1

#define SPI_FIRSTBIT_MSB 0
#define SPI_FIRSTBIT_LSB 1

#define SPI_DATASIZE_8 8    // 8bit
#define SPI_DATASIZE_16 16  // 16bit

// void spi_delay() {}

uint16_t spi_rw(spi_t* spi, uint16_t byte);
uint16_t spi_r(spi_t* spi);

#endif
