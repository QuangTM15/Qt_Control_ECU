/**
 * @file Mcal_Pwm.c
 * @brief Minimal FTM0 PWM driver for S32K144.
 */

#include "Mcal_Pwm.h"
#include "S32K144.h"

#define MCAL_PWM_LED_PIN (15UL)
#define MCAL_PWM_MODULO_VALUE (999UL)

void Mcal_Pwm_Init(void)
{
    /* Enable clock for PORTD. */
    IP_PCC->PCCn[PCC_PORTD_INDEX] |= PCC_PCCn_CGC_MASK;

    /* Configure PTD15 as FTM0_CH0 using ALT2. */
    IP_PORTD->PCR[MCAL_PWM_LED_PIN] = PORT_PCR_MUX(2U);

    /* Disable FTM0 clock before configuration. */
    IP_PCC->PCCn[PCC_FTM0_INDEX] &= ~PCC_PCCn_CGC_MASK;

    /* Select system clock source for FTM0. */
    IP_PCC->PCCn[PCC_FTM0_INDEX] =
        PCC_PCCn_PCS(1U) |
        PCC_PCCn_CGC_MASK;

    /* Enable writes to protected FTM registers. */
    IP_FTM0->MODE |= FTM_MODE_WPDIS_MASK;

    /* Stop counter while configuring. */
    IP_FTM0->SC = 0U;

    /* Reset counter. */
    IP_FTM0->CNT = 0U;

    /* Set PWM period. */
    IP_FTM0->MOD = MCAL_PWM_MODULO_VALUE;

    IP_FTM0->CONTROLS[0].CnSC =
        FTM_CnSC_MSB_MASK |
        FTM_CnSC_ELSA_MASK;

    IP_FTM0->CONTROLS[0].CnV = 0U;

    IP_FTM0->SC =
        FTM_SC_PWMEN0_MASK |
        FTM_SC_CLKS(1U) |
        FTM_SC_PS(7U);
}

void Mcal_Pwm_SetDuty(uint8_t duty_percent)
{
    uint32_t compare_value;

    if (duty_percent > 100U)
    {
        duty_percent = 100U;
    }

    compare_value =
        ((uint32_t)duty_percent *
         (MCAL_PWM_MODULO_VALUE + 1UL)) /
        100UL;

    IP_FTM0->CONTROLS[0].CnV = compare_value;
}
