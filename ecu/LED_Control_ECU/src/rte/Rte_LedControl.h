/**
 * @file Rte_LedControl.h
 * @brief RTE interface for the LED Control SWC.
 */

#ifndef RTE_LED_CONTROL_H
#define RTE_LED_CONTROL_H

#include <stdint.h>

/**
 * @brief Writes a new LED command into the RTE.
 *
 * @param ledState 0 for OFF, 1 for ON.
 * @param brightness Brightness from 0 to 100 percent.
 */
void Rte_LedControl_WriteCommand(
    uint8_t ledState,
    uint8_t brightness);

/**
 * @brief Reads the latest unprocessed LED command.
 *
 * Reading a command clears the pending-command flag.
 *
 * @param ledState Destination for LED state.
 * @param brightness Destination for brightness.
 *
 * @return 1 if a new command was read, otherwise 0.
 */
uint8_t Rte_LedControl_ReadCommand(
    uint8_t *ledState,
    uint8_t *brightness);

/**
 * @brief Applies the desired LED state through ECU abstraction.
 *
 * @param ledState 0 for OFF, 1 for ON.
 * @param brightness Brightness from 0 to 100 percent.
 */
void Rte_LedControl_ApplyOutput(
    uint8_t ledState,
    uint8_t brightness);

/**
 * @brief Reads the actual LED state.
 *
 * @return 0 for OFF, 1 for ON.
 */
uint8_t Rte_LedControl_GetActualState(void);

/**
 * @brief Reads the configured LED brightness.
 *
 * @return Brightness from 0 to 100 percent.
 */
uint8_t Rte_LedControl_GetActualBrightness(void);

#endif /* RTE_LED_CONTROL_H */