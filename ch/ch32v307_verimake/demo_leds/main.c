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
