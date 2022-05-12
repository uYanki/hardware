# hardware_verimake_opench_ch32v307
Refer: https://verimake.com/d/37-opench-ch32v307-risc-v

development environment: MounRiverStudio(MRS) http://mounriver.com/download

### board_opench_ch32v307

![board_opench_ch32v307](README.assets/board_opench_ch32v307.jpg)

### Create Project (demo)

![image-20220412115929092](README.assets/how_to_create_a_project.png)

![image-20220412121343551](README.assets/how_to_build_adn_download.png)

`main.c`

```c
#include "debug.h"
int main(void) {
    u16 i = 0;

    // Init Delay & Printf
    Delay_Init();
    USART_Printf_Init(115200);
    printf("SystemClk:%d\r\n", SystemCoreClock);

    // Init GPIO: LED1(PE11) & LED2(PE12)
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    // Main Loop
    while (1) {
        Delay_Ms(250);
        GPIO_WriteBit(GPIOE, GPIO_Pin_11, i ? 1 : 0);
        GPIO_WriteBit(GPIOE, GPIO_Pin_12, i = i ? 0 : 1);
    }
}
```

