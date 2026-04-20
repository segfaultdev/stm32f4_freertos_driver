#include "stm32f446xx.h"
#include "hardware/gpio.h"

void GPIO_Config(void)
{

    GPIO_ClockEnable(GPIOA);
    GPIO_ClockEnable(GPIOC);

    GPIO_Config_t Config;

    Config.Pin = GPIO_PIN_5;
    Config.Mode = GPIO_MODE_OUTPUT;
    Config.Otype = GPIO_OTYPE_PP;
    Config.Ospeed = GPIO_SPEED_VERY_HIGH;

    GPIO_Init(GPIOA, &Config);

    Config.Pin = GPIO_PIN_13;
    Config.Mode = GPIO_MODE_INPUT;
    Config.Pull = GPIO_PULL_UP;
    GPIO_Init(GPIOC, &Config);
}

int main()
{
    // GPIO_IRQ_Init();
    GPIO_Config();
    GPIO_AttachInterrupt(GPIOC, GPIO_PIN_13, GPIO_IT_FTSR);

    NVIC_EnableIRQ(EXTI15_10_IRQn);
    NVIC_SetPriority(EXTI15_10_IRQn, 2);

    for (;;)
    {
    }

    return 0;
}

void GPIO_IRQCallBack(uint16_t Pin)
{
    for (int i = 0; i < 10; i++)
    {

        GPIOA->ODR ^= 1 << 5;
        for (int i = 0; i < 500000; i++)
            ;
    }
}

void EXTI15_10_IRQHandler(void)
{
    GPIO_HandleIRQ(GPIO_PIN_13);
}
