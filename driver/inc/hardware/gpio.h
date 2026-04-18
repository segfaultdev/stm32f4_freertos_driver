#ifndef _DRIVER_GPIO_H_
#define _DRIVER_GPIO_H_

#include "stm32f446xx.h"
#include "common.h"

#define GPIO_PIN_0 0x0001
#define GPIO_PIN_1 0x0002
#define GPIO_PIN_2 0x0004
#define GPIO_PIN_3 0x0008
#define GPIO_PIN_4 0x0010
#define GPIO_PIN_5 0x0020
#define GPIO_PIN_6 0x0040
#define GPIO_PIN_7 0x0080
#define GPIO_PIN_8 0x0100
#define GPIO_PIN_9 0x0200
#define GPIO_PIN_10 0x0400
#define GPIO_PIN_11 0x0800
#define GPIO_PIN_12 0x1000
#define GPIO_PIN_13 0x2000
#define GPIO_PIN_14 0x4000
#define GPIO_PIN_15 0x8000

#define DRIVER_GPIO_WRITE_4BIT(REG, POS, VAL) \
    ((REG) = ((REG) & ~(0xFU << ((POS) * 4))) | (((VAL) & 0xFU) << ((POS) * 4)))

#define DRIVER_GPIO_WRITE_2BIT(REG, POS, VAL) \
    ((REG) = ((REG) & ~(0x3U << ((POS) * 2))) | (((VAL) & 0x3U) << ((POS) * 2)))

#define DRIVER_GPIO_WRITE_BIT(REG, POS, VAL) \
    ((REG) = ((REG) & ~(1U << (POS))) | (((VAL) & 0x1U) << (POS)))

typedef enum
{
    GPIO_MODE_INPUT = 0x00,
    GPIO_MODE_OUTPUT = 0x01,
    GPIO_MODE_ALT = 0x02,
    GPIO_MODE_ANALOG = 0x03
} GPIO_Mode_t;

typedef enum
{
    GPIO_PULL_NONE = 0x00,
    GPIO_PULL_UP = 0x01,
    GPIO_PULL_DOWN = 0x02
} GPIO_Pupd_t;

typedef enum
{
    GPIO_SPEED_LOW = 0x00,
    GPIO_SPEED_MEDIUM = 0x01,
    GPIO_SPEED_HIGH = 0x02,
    GPIO_SPEED_VERY_HIGH = 0x03
} GPIO_Ospeed_t;

typedef enum
{
    GPIO_OTYPE_PP = 0x00, // Push-Pull
    GPIO_OTYPE_OD = 0x01  // Open-Drain
} GPIO_Otype_t;

typedef struct
{
    uint16_t Pin; // Use your GPIO_PIN_x defines
    GPIO_Mode_t Mode;
    GPIO_Pupd_t Pull;
    GPIO_Ospeed_t Ospeed;
    GPIO_Otype_t Otype;
    uint8_t AFn; // For AF modes (UART, SPI, etc.)
} GPIO_Config_t;

err_t GPIO_ClockEnable(GPIO_TypeDef *pGPIO);
err_t GPIO_ClockDisable(GPIO_TypeDef *pGPIO);
err_t GPIO_Init(GPIO_TypeDef *pGPIO, const GPIO_Config_t *pConfig);
err_t GPIO_SetPinLevel(GPIO_TypeDef *pGPIO, uint16_t Pin, uint16_t Level);
uint32_t GPIO_GetPinLevel(GPIO_TypeDef *pGPIO, uint16_t Pin);


err_t GPIO_SetPinMode(GPIO_TypeDef *pGPIO, uint16_t Pin, GPIO_Mode_t Mode);
err_t GPIO_SetPinPull(GPIO_TypeDef *pGPIO, uint16_t Pin, GPIO_Pupd_t Pull);
err_t GPIO_SetPinOutputSpeed(GPIO_TypeDef *pGPIO, uint16_t Pin, GPIO_Ospeed_t Speed);
err_t GPIO_SetPinOutputType(GPIO_TypeDef *pGPIO, uint16_t Pin, GPIO_Otype_t Otype);

#endif /* _DRIVER_GPIO_H_ */