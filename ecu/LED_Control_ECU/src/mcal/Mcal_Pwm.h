/**
 * @file Mcal_Pwm.h
 * @brief Minimal PWM MCAL interface for LED brightness control.
 */

#ifndef MCAL_PWM_H
#define MCAL_PWM_H

#include <stdint.h>

/**
 * @brief Initializes FTM0 channel 0 PWM output on PTD15.
 */
void Mcal_Pwm_Init(void);

/**
 * @brief Sets LED brightness.
 *
 * @param duty_percent PWM duty cycle from 0 to 100 percent.
 */
void Mcal_Pwm_SetDuty(uint8_t duty_percent);

#endif /* MCAL_PWM_H */