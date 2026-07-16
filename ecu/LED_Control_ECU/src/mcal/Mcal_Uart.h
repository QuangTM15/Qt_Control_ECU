/**
 * @file Mcal_Uart.h
 * @brief Minimal polling LPUART1 driver for S32K144.
 */

#ifndef MCAL_UART_H
#define MCAL_UART_H

#include <stdint.h>

/**
 * @brief Initializes LPUART1 on PTC6/PTC7.
 *
 * Configuration:
 * - RX: PTC6 ALT2
 * - TX: PTC7 ALT2
 * - Baud rate: 115200
 * - 8 data bits
 * - No parity
 * - 1 stop bit
 */
void Mcal_Uart_Init(void);

/**
 * @brief Sends one byte through LPUART1.
 *
 * @param data Byte to transmit.
 */
void Mcal_Uart_SendByte(uint8_t data);

/**
 * @brief Sends a null-terminated string through LPUART1.
 *
 * @param text Pointer to the string.
 */
void Mcal_Uart_SendString(const char *text);

/**
 * @brief Checks whether one received byte is available.
 *
 * @return 1 if data is available, otherwise 0.
 */
uint8_t Mcal_Uart_IsDataAvailable(void);

/**
 * @brief Reads one received byte.
 *
 * This function should only be called after
 * Mcal_Uart_IsDataAvailable() returns 1.
 *
 * @return Received byte.
 */
uint8_t Mcal_Uart_ReadByte(void);

#endif /* MCAL_UART_H */