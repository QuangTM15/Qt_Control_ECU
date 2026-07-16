/**
 * @file main.c
 * @brief IoHwAb LED test for the LED Control ECU.
 */

#include <stdint.h>

#include "IoHwAb_Led.h"

static void App_Delay(void)
{
    volatile uint32_t counter;

    for (counter = 0UL; counter < 100000UL; counter++)
    {
        /* Busy wait for test only. */
    }
}

static void App_LongDelay(void)
{
    volatile uint32_t counter;

    for (counter = 0UL; counter < 1000000UL; counter++)
    {
        /* Busy wait for test only. */
    }
}

int main(void)
{
    uint8_t brightness;

    IoHwAb_Led_Init();

    for (;;)
    {
        IoHwAb_Led_On();
        App_LongDelay();

        for (brightness = 100U; brightness > 0U; brightness -= 10U)
        {
            IoHwAb_Led_SetBrightness(brightness);
            App_Delay();
        }

        IoHwAb_Led_Off();
        App_LongDelay();

        IoHwAb_Led_SetBrightness(30U);
        App_LongDelay();

        IoHwAb_Led_On();
        App_LongDelay();

        IoHwAb_Led_Off();
        App_LongDelay();
    }
}
