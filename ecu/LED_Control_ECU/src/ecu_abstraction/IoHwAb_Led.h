/**
 * @file IoHwAb_Led.h
 * @brief LED hardware abstraction interface.
 */

#ifndef IOHWAB_LED_H
#define IOHWAB_LED_H

#include <stdint.h>

/**
 * @brief Initializes the LED hardware abstraction.
 */
void IoHwAb_Led_Init(void);

/**
 * @brief Turns the LED on using the current brightness value.
 */
void IoHwAb_Led_On(void);

/**
 * @brief Turns the LED off.
 */
void IoHwAb_Led_Off(void);

/**
 * @brief Sets the LED brightness.
 *
 * @param brightness_percent Brightness value from 0 to 100 percent.
 */
void IoHwAb_Led_SetBrightness(uint8_t brightness_percent);

/**
 * @brief Returns the currently configured LED brightness.
 *
 * @return Brightness value from 0 to 100 percent.
 */
uint8_t IoHwAb_Led_GetBrightness(void);

/**
 * @brief Returns the current LED state.
 *
 * @return 1 if the LED is enabled, otherwise 0.
 */
uint8_t IoHwAb_Led_GetState(void);

#endif /* IOHWAB_LED_H */