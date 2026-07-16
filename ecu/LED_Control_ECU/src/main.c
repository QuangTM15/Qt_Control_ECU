/**
 * @file main.c
 * @brief PWM brightness test for the LED Control ECU.
 */

#include <stdint.h>

#include "mcal/Mcal_Pwm.h"

/**
 * @brief Simple blocking delay used only for PWM hardware testing.
 */
static void App_Delay(void)
{
    volatile uint32_t counter;

    for (counter = 0UL; counter < 100000UL; counter++)
    {
        /* Busy wait for initial PWM test only. */
    }
}

/**
 * @brief Longer pause after switching the LED off.
 */
static void App_LongDelay(void)
{
    volatile uint32_t counter;

    for (counter = 0UL; counter < 1000000UL; counter++)
    {
        /* Busy wait for initial PWM test only. */
    }
}

int main(void)
{
    uint8_t brightness;

    Mcal_Pwm_Init();

    for (;;)
    {
        /*
         * Gradually increase LED brightness
         * from 0 percent to 100 percent.
         */
        for (brightness = 0U; brightness <= 100U; brightness++)
        {
            Mcal_Pwm_SetDuty(brightness);
            App_Delay();
        }

        /*
         * Turn the LED off immediately.
         */
        Mcal_Pwm_SetDuty(0U);

        App_LongDelay();
    }
}
