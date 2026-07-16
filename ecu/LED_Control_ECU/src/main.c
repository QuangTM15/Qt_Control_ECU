/**
 * @file main.c
 * @brief UART Echo Test.
 */

#include <stdint.h>

#include "Mcal_Clock.h"
#include "Mcal_Uart.h"

int main(void)
{
    uint8_t rxData;

    /*
     * Initialize system clock.
     */
    Mcal_Clock_Init();

    /*
     * Initialize UART.
     */
    Mcal_Uart_Init();

    /*
     * Startup message.
     */
    Mcal_Uart_SendString("\r\n");
    Mcal_Uart_SendString("=================================\r\n");
    Mcal_Uart_SendString(" S32K144 UART Echo Test\r\n");
    Mcal_Uart_SendString(" Baudrate : 9600\r\n");
    Mcal_Uart_SendString(" Type anything...\r\n");
    Mcal_Uart_SendString("=================================\r\n");

    while (1)
    {
        if (Mcal_Uart_IsDataAvailable() != 0U)
        {
            rxData = Mcal_Uart_ReadByte();

            /*
             * Echo received character.
             */
            Mcal_Uart_SendByte(rxData);
        }
    }
}