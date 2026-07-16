/**
 * @file Mcal_Dio.h
 * @brief Minimal DIO MCAL interface for the LED Control ECU.
 */

#ifndef MCAL_DIO_H
#define MCAL_DIO_H

/**
 * @brief Initializes the DIO channel connected to the red LED.
 */
void Mcal_Dio_Init(void);

/**
 * @brief Turns the red LED on.
 */
void Mcal_Dio_LedOn(void);

/**
 * @brief Turns the red LED off.
 */
void Mcal_Dio_LedOff(void);

/**
 * @brief Toggles the current red LED state.
 */
void Mcal_Dio_LedToggle(void);

#endif /* MCAL_DIO_H */