#include "hardware/gpio.h"

err_t GPIO_Init(GPIO_TypeDef *pGPIO, const GPIO_Config_t *pConfig)
{
    for (uint8_t Pin = 0; Pin < 16; Pin++)
    {
        if (pConfig->Pin & (1U << Pin))
        {
            /* Configure Alternate Function first */
            if (pConfig->Mode == GPIO_MODE_ALT)
            {
                DRIVER_GPIO_WRITE_4BIT(pGPIO->AFR[Pin / 8], Pin % 8, pConfig->AFn);
            }

            /* Configure output-related settings before enabling output */
            if (pConfig->Mode == GPIO_MODE_OUTPUT || pConfig->Mode == GPIO_MODE_ALT)
            {
                DRIVER_GPIO_WRITE_2BIT(pGPIO->OSPEEDR, Pin, pConfig->Ospeed);
                DRIVER_GPIO_WRITE_BIT(pGPIO->OTYPER, Pin, pConfig->Otype);
            }

            /* Finally configure mode */
            DRIVER_GPIO_WRITE_2BIT(pGPIO->MODER, Pin, pConfig->Mode);

            /* Pull-up / pull-down */
            DRIVER_GPIO_WRITE_2BIT(pGPIO->PUPDR, Pin, pConfig->Pull);
        }
    }

    return ERR_OK;
}

err_t GPIO_ClockEnable(GPIO_TypeDef *pGPIO)
{
    uint32_t index = ((uint32_t)pGPIO - (uint32_t)GPIOA) / 0x400U;

    if (index > 7)
        return ERR_INVALID;

    RCC->AHB1ENR |= (1U << index);
    return ERR_OK;
}

err_t GPIO_ClockDisable(GPIO_TypeDef *pGPIO)
{
    uint32_t index = ((uint32_t)pGPIO - (uint32_t)GPIOA) / 0x400U;

    if (index > 7)
        return ERR_INVALID;

    RCC->AHB1ENR &= ~(1U << index);
    return ERR_OK;
}

err_t GPIO_SetPinLevel(GPIO_TypeDef *pGPIO, uint16_t Pin, uint16_t Level)
{
    if (Level == 1)
    {
        pGPIO->BSRR = Pin;
    }
    else if (Level == 0)
    {
        pGPIO->BSRR = Pin << 16;
    }
}
uint32_t GPIO_GetPinLevel(GPIO_TypeDef *pGPIO, uint16_t Pin)
{
    return (pGPIO->IDR & Pin) ? 1 : 0;
}

static uint32_t GPIO_PortToNum(GPIO_TypeDef *pGPIO)
{
    if (pGPIO == GPIOA)
        return 0;
    if (pGPIO == GPIOB)
        return 1;
    if (pGPIO == GPIOC)
        return 2;
    if (pGPIO == GPIOD)
        return 3;
    if (pGPIO == GPIOE)
        return 4;
    if (pGPIO == GPIOF)
        return 5;
    if (pGPIO == GPIOG)
        return 6;
    if (pGPIO == GPIOH)
        return 7;
    return 0;
}

static int GPIO_PinMaskToPin(uint16_t PinMask)
{
    for (int i = 0; i < 16; i++)
    {
        if (PinMask == (1 << i))
        {
            return i;
        }
    }
    return -1;
}

err_t GPIO_AttachInterrupt(GPIO_TypeDef *pGPIO, uint16_t Pin, GPIO_Edge_t Edge)
{
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    int pin = GPIO_PinMaskToPin(Pin);
    if (pin < 0)
        return ERR_INVALID;

    uint32_t port = GPIO_PortToNum(pGPIO);

    uint32_t index = pin / 4;
    uint32_t shift = (pin % 4) * 4;

    SYSCFG->EXTICR[index] &= ~(0xF << shift);
    SYSCFG->EXTICR[index] |= (port << shift);

    if (Edge == GPIO_IT_FTSR)
    {
        EXTI->FTSR |= Pin;
        EXTI->RTSR &= ~Pin;
    }
    else if (Edge == GPIO_IT_RTSR)
    {
        EXTI->RTSR |= Pin;
        EXTI->FTSR &= ~Pin;
    }
    else
    {
        EXTI->RTSR |= Pin;
        EXTI->FTSR |= Pin;
    }

    EXTI->IMR |= Pin;

    EXTI->PR = Pin;

    return ERR_OK;
}

__attribute__((weak)) void GPIO_IRQCallBack(uint16_t Pin) {}

void GPIO_HandleIRQ(uint16_t Pin)
{
    if (EXTI->PR & Pin)
    {
        EXTI->PR = Pin;
        GPIO_IRQCallBack(Pin);
    }
}