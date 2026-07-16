/**
 * @file Mcal_Dio.c
 * @brief Minimal DIO MCAL implementation for S32K144.
 */

#include "Mcal_Dio.h"
#include "S32K144.h"

#define MCAL_DIO_LED_PORT_INDEX (15UL)
#define MCAL_DIO_LED_MASK (1UL << MCAL_DIO_LED_PORT_INDEX)

/**
 * @brief Initializes PTD15 as a GPIO output.
 */
void Mcal_Dio_Init(void)
{
    /* Enable clock for PORTD. */
    IP_PCC->PCCn[PCC_PORTD_INDEX] |= PCC_PCCn_CGC_MASK;

    /* Configure PTD15 pin multiplexer as GPIO. */
    IP_PORTD->PCR[MCAL_DIO_LED_PORT_INDEX] = PORT_PCR_MUX(1U);

    /* Configure PTD15 as output. */
    IP_PTD->PDDR |= MCAL_DIO_LED_MASK;

    /* LED is active-low, so set output high to turn it off initially. */
    IP_PTD->PSOR = MCAL_DIO_LED_MASK;
}

void Mcal_Dio_LedOn(void)
{
    /* Clear output bit: active-low LED turns on. */
    IP_PTD->PCOR = MCAL_DIO_LED_MASK;
}

void Mcal_Dio_LedOff(void)
{
    /* Set output bit: active-low LED turns off. */
    IP_PTD->PSOR = MCAL_DIO_LED_MASK;
}

void Mcal_Dio_LedToggle(void)
{
    IP_PTD->PTOR = MCAL_DIO_LED_MASK;
}
