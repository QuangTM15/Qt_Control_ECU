/**
 * @file Mcal_Clock.c
 * @brief System clock configuration for S32K144.
 */

#include "Mcal_Clock.h"
#include "S32K144.h"

#define MCAL_CLOCK_FIRC_FREQUENCY_HZ (48000000UL)
#define MCAL_CLOCK_CORE_FREQUENCY_HZ (48000000UL)
#define MCAL_CLOCK_BUS_FREQUENCY_HZ (48000000UL)
#define MCAL_CLOCK_SLOW_FREQUENCY_HZ (24000000UL)
#define MCAL_CLOCK_FIRC_DIV2_FREQUENCY_HZ (48000000UL)

void Mcal_Clock_Init(void)
{
    /*
     * Enable FIRC if it is not already enabled.
     */
    if ((IP_SCG->FIRCCSR & SCG_FIRCCSR_FIRCEN_MASK) == 0U)
    {
        IP_SCG->FIRCCSR |= SCG_FIRCCSR_FIRCEN_MASK;
    }

    /*
     * Wait until FIRC becomes valid.
     */
    while ((IP_SCG->FIRCCSR & SCG_FIRCCSR_FIRCVLD_MASK) == 0U)
    {
        /* Wait for FIRC to stabilize. */
    }

    /*
     * Configure FIRC divider outputs:
     *
     * FIRCDIV1 = divide by 1 -> 48 MHz
     * FIRCDIV2 = divide by 1 -> 48 MHz
     */
    IP_SCG->FIRCDIV =
        SCG_FIRCDIV_FIRCDIV1(1U) |
        SCG_FIRCDIV_FIRCDIV2(1U);

    /*
     * Configure RUN mode:
     *
     * SCS     = 3: FIRC
     * DIVCORE = 0: divide by 1 -> 48 MHz
     * DIVBUS  = 0: divide by 1 -> 48 MHz
     * DIVSLOW = 1: divide by 2 -> 24 MHz
     */
    IP_SCG->RCCR =
        SCG_RCCR_SCS(3U) |
        SCG_RCCR_DIVCORE(0U) |
        SCG_RCCR_DIVBUS(0U) |
        SCG_RCCR_DIVSLOW(1U);

    /*
     * Wait until FIRC is selected as the system clock.
     */
    while (((IP_SCG->CSR & SCG_CSR_SCS_MASK) >>
            SCG_CSR_SCS_SHIFT) != 3U)
    {
        /* Wait for system clock switch. */
    }
}

uint32_t Mcal_Clock_GetCoreFrequency(void)
{
    return MCAL_CLOCK_CORE_FREQUENCY_HZ;
}

uint32_t Mcal_Clock_GetBusFrequency(void)
{
    return MCAL_CLOCK_BUS_FREQUENCY_HZ;
}

uint32_t Mcal_Clock_GetFircDiv2Frequency(void)
{
    return MCAL_CLOCK_FIRC_DIV2_FREQUENCY_HZ;
}