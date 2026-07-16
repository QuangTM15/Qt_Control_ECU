/**
 * @file ComService.h
 * @brief UART communication service for LED control commands.
 */

#ifndef COM_SERVICE_H
#define COM_SERVICE_H

#include <stdint.h>

/**
 * @brief LED command received from the external HMI.
 */
typedef struct
{
    uint8_t ledState;
    uint8_t brightness;
} ComService_LedCommandType;

/**
 * @brief Initializes the communication service and UART interface.
 */
void ComService_Init(void);

/**
 * @brief Processes received UART bytes.
 *
 * This function must be called repeatedly from the main loop.
 */
void ComService_MainFunction(void);

/**
 * @brief Checks whether a new valid LED command is available.
 *
 * @return 1 if a command is available, otherwise 0.
 */
uint8_t ComService_IsCommandAvailable(void);

/**
 * @brief Gets the latest valid LED command.
 *
 * Reading the command clears the command-available flag.
 *
 * @param command Pointer to the destination command structure.
 *
 * @return 1 if a command was successfully returned, otherwise 0.
 */
uint8_t ComService_GetCommand(ComService_LedCommandType *command);

/**
 * @brief Sends the current LED state back to the HMI.
 *
 * @param ledState LED state: 0 for OFF, 1 for ON.
 * @param brightness Brightness from 0 to 100 percent.
 */
void ComService_SendLedStatus(uint8_t ledState, uint8_t brightness);

#endif /* COM_SERVICE_H */