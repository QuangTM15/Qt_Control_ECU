/**
 * @file Mcal_Clock.h
 * @brief System clock configuration for S32K144.
 */

#ifndef MCAL_CLOCK_H
#define MCAL_CLOCK_H

#include <stdint.h>

/**
 * @brief Initializes the system clock using the 48 MHz FIRC.
 *
 * Resulting clocks:
 * - Core clock: 48 MHz
 * - Bus clock: 48 MHz
 * - Slow/Flash clock: 24 MHz
 */
void Mcal_Clock_Init(void);

/**
 * @brief Returns the configured core clock frequency.
 *
 * @return Core clock frequency in Hz.
 */
uint32_t Mcal_Clock_GetCoreFrequency(void);

/**
 * @brief Returns the configured bus clock frequency.
 *
 * @return Bus clock frequency in Hz.
 */
uint32_t Mcal_Clock_GetBusFrequency(void);

/**
 * @brief Returns the configured FIRC DIV2 clock frequency.
 *
 * @return FIRC DIV2 clock frequency in Hz.
 */
uint32_t Mcal_Clock_GetFircDiv2Frequency(void);

#endif /* MCAL_CLOCK_H */