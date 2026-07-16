/**
 * @file main.c
 * @brief Main integration for the AUTOSAR-inspired LED Control ECU.
 */

#include <stdint.h>

#include "Mcal_Clock.h"
#include "IoHwAb_Led.h"
#include "ComService.h"
#include "Rte_LedControl.h"
#include "LedControl.h"

int main(void)
{
    ComService_LedCommandType receivedCommand;

    /*
     * Initialize the MCU system clock first.
     */
    Mcal_Clock_Init();

    /*
     * Initialize the LED hardware abstraction.
     * This also initializes the PWM driver.
     */
    IoHwAb_Led_Init();

    /*
     * Initialize the LED application SWC.
     * Initial state: LED OFF, stored brightness 100%.
     */
    LedControl_Init();

    /*
     * Initialize UART communication.
     * ECU sends "$READY\r\n" after startup.
     */
    ComService_Init();

    for (;;)
    {
        /*
         * Receive UART bytes and parse complete commands.
         */
        ComService_MainFunction();

        /*
         * Transfer a valid command from Communication Service
         * into the RTE.
         */
        if (ComService_GetCommand(&receivedCommand) != 0U)
        {
            Rte_LedControl_WriteCommand(
                receivedCommand.ledState,
                receivedCommand.brightness);
        }

        /*
         * Execute the LED Control runnable.
         */
        LedControl_MainFunction();
    }
}
