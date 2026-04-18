#include "stm32f446xx.h"
#include "hardware/gpio.h"

void GPIO_Config(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    GPIO_Config_t Config;
    Config.Pin = GPIO_PIN_5;
    Config.Mode = GPIO_MODE_OUTPUT;
    Config.Otype = GPIO_OTYPE_PP;
    Config.Ospeed = GPIO_SPEED_VERY_HIGH;
    GPIO_Init(GPIOA, &Config);
}

void GPIO_IRQ_Init()
{
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

    GPIOA->MODER &= ~(3U << 10);
    GPIOA->MODER |= 1 << 10;

    GPIOC->MODER &= ~(3U << 26);
    GPIOC->PUPDR &= ~(3U << 26);
    GPIOC->PUPDR |= GPIO_PUPDR_PUPD13_0;

    SYSCFG->EXTICR[3] &= ~(0xF << 4);
    SYSCFG->EXTICR[3] |= 2 << 4;

    EXTI->IMR |= 1 << 13;
    EXTI->FTSR |= 1 << 13;
    EXTI->RTSR &= ~(1 << 13);

    NVIC_EnableIRQ(EXTI15_10_IRQn);
    NVIC_SetPriority(EXTI15_10_IRQn, 2);
}

int main()
{
    // GPIO_IRQ_Init();
    GPIO_Config();

    for (;;)
    {
        GPIOA->ODR ^= 1 << 5;
        for (int i = 0; i < 1000000; i++)
            ;
    }

    return 0;
}

void EXTI15_10_IRQHandler(void)
{
    if (EXTI->PR & (1 << 13))
    {
        EXTI->PR |= (1 << 13);

        for (int i = 0; i < 5; i++)
        {
            GPIOA->ODR ^= 1 << 5;
            for (int i = 0; i < 250000; i++)
                ;
        }
    }
}