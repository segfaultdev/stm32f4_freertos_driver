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