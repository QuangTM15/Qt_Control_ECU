/**
 * @file main.c
 * @brief Main entry point for the LED Control ECU.
 */

#include "mcal/Mcal_Dio.h"

/**
 * @brief Simple software delay used only for the initial hardware test.
 */
static void App_Delay(void)
{
    volatile unsigned long counter;

    for (counter = 0UL; counter < 1000000UL; counter++)
    {
        /* Busy wait for initial LED test only. */
    }
}

int main(void)
{
    Mcal_Dio_Init();

    for (;;)
    {
        Mcal_Dio_LedToggle();
        App_Delay();
    }
}
