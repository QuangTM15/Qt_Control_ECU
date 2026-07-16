/**
 * @file LedControl.h
 * @brief LED Control Application Software Component.
 */

#ifndef LED_CONTROL_H
#define LED_CONTROL_H

/**
 * @brief Initializes the LED Control SWC.
 */
void LedControl_Init(void);

/**
 * @brief Processes pending LED commands.
 *
 * This runnable must be called repeatedly from the main loop.
 */
void LedControl_MainFunction(void);

#endif /* LED_CONTROL_H */