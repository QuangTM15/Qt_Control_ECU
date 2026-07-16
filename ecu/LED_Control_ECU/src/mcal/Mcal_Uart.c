/**
 * @file Mcal_Uart.c
 * @brief Polling LPUART1 driver implementation for S32K144.
 */

#include "Mcal_Uart.h"
#include "Mcal_Clock.h"
#include "S32K144.h"

#define MCAL_UART_RX_PIN (6UL)
#define MCAL_UART_TX_PIN (7UL)

#define MCAL_UART_BAUD_RATE (9600UL)
#define MCAL_UART_OVERSAMPLING_RATIO (16UL)
#define MCAL_UART_OSR_REGISTER_VALUE \
    (MCAL_UART_OVERSAMPLING_RATIO - 1UL)

void Mcal_Uart_Init(void)
{
    uint32_t uart_clock_hz;
    uint32_t baud_divisor;
    uint32_t sbr_value;

    /*
     * Enable PORTC peripheral clock.
     */
    IP_PCC->PCCn[PCC_PORTC_INDEX] |= PCC_PCCn_CGC_MASK;

    /*
     * Configure UART pins:
     *
     * PTC6 ALT2 -> LPUART1_RX
     * PTC7 ALT2 -> LPUART1_TX
     */
    IP_PORTC->PCR[MCAL_UART_RX_PIN] = PORT_PCR_MUX(2U);
    IP_PORTC->PCR[MCAL_UART_TX_PIN] = PORT_PCR_MUX(2U);

    /*
     * Disable LPUART1 peripheral clock before selecting
     * the functional clock source.
     */
    IP_PCC->PCCn[PCC_LPUART1_INDEX] &= ~PCC_PCCn_CGC_MASK;

    /*
     * PCS = 3 selects FIRC_DIV2 as the LPUART1 clock source.
     */
    IP_PCC->PCCn[PCC_LPUART1_INDEX] =
        PCC_PCCn_PCS(3U) |
        PCC_PCCn_CGC_MASK;

    /*
     * Disable transmitter and receiver during configuration.
     */
    IP_LPUART1->CTRL = 0U;

    uart_clock_hz = Mcal_Clock_GetFircDiv2Frequency();

    baud_divisor =
        MCAL_UART_OVERSAMPLING_RATIO *
        MCAL_UART_BAUD_RATE;

    /*
     * Calculate SBR with rounding to the nearest integer.
     *
     * At 48 MHz, OSR = 16 and baud = 9600:
     *
     * SBR ≈ 48,000,000 / (16 * 9600)
     *     ≈ 312.5
     *
     * Rounded value = 313.
     */
    sbr_value =
        (uart_clock_hz + (baud_divisor / 2UL)) /
        baud_divisor;

    /*
     * Configure:
     * - OSR = 16
     * - SBR calculated for 9600 baud
     * - 1 stop bit
     */
    IP_LPUART1->BAUD =
        LPUART_BAUD_OSR(MCAL_UART_OSR_REGISTER_VALUE) |
        LPUART_BAUD_SBR(sbr_value);

    /*
     * Configure:
     * - 8 data bits
     * - No parity
     * - Receiver enabled
     * - Transmitter enabled
     */
    IP_LPUART1->CTRL =
        LPUART_CTRL_TE_MASK |
        LPUART_CTRL_RE_MASK;
}

void Mcal_Uart_SendByte(uint8_t data)
{
    while ((IP_LPUART1->STAT & LPUART_STAT_TDRE_MASK) == 0U)
    {
        /* Wait until transmit data register is empty. */
    }

    IP_LPUART1->DATA = (uint32_t)data;
}

void Mcal_Uart_SendString(const char *text)
{
    if (text == (const char *)0)
    {
        return;
    }

    while (*text != '\0')
    {
        Mcal_Uart_SendByte((uint8_t)*text);
        text++;
    }
}

uint8_t Mcal_Uart_IsDataAvailable(void)
{
    uint8_t result = 0U;

    if ((IP_LPUART1->STAT & LPUART_STAT_RDRF_MASK) != 0U)
    {
        result = 1U;
    }

    return result;
}

uint8_t Mcal_Uart_ReadByte(void)
{
    return (uint8_t)(IP_LPUART1->DATA & 0xFFU);
}