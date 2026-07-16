/**
 * @file IoHwAb_Led.c
 * @brief LED hardware abstraction implementation.
 */

#include "IoHwAb_Led.h"
#include "Mcal_Pwm.h"

#define IOHWAB_LED_MIN_BRIGHTNESS (0U)
#define IOHWAB_LED_MAX_BRIGHTNESS (100U)
#define IOHWAB_LED_DEFAULT_BRIGHTNESS (100U)

static uint8_t IoHwAb_Led_Brightness = IOHWAB_LED_DEFAULT_BRIGHTNESS;
static uint8_t IoHwAb_Led_State = 0U;

void IoHwAb_Led_Init(void)
{
    Mcal_Pwm_Init();

    IoHwAb_Led_Brightness = IOHWAB_LED_DEFAULT_BRIGHTNESS;
    IoHwAb_Led_State = 0U;

    Mcal_Pwm_SetDuty(IOHWAB_LED_MIN_BRIGHTNESS);
}

void IoHwAb_Led_On(void)
{
    IoHwAb_Led_State = 1U;
    Mcal_Pwm_SetDuty(IoHwAb_Led_Brightness);
}

void IoHwAb_Led_Off(void)
{
    IoHwAb_Led_State = 0U;
    Mcal_Pwm_SetDuty(IOHWAB_LED_MIN_BRIGHTNESS);
}

void IoHwAb_Led_SetBrightness(uint8_t brightness_percent)
{
    if (brightness_percent > IOHWAB_LED_MAX_BRIGHTNESS)
    {
        brightness_percent = IOHWAB_LED_MAX_BRIGHTNESS;
    }

    IoHwAb_Led_Brightness = brightness_percent;

    if (IoHwAb_Led_State != 0U)
    {
        Mcal_Pwm_SetDuty(IoHwAb_Led_Brightness);
    }
}

uint8_t IoHwAb_Led_GetBrightness(void)
{
    return IoHwAb_Led_Brightness;
}

uint8_t IoHwAb_Led_GetState(void)
{
    return IoHwAb_Led_State;
}
