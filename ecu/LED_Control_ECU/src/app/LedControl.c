/**
 * @file LedControl.c
 * @brief LED Control Application Software Component implementation.
 */

#include <stdint.h>

#include "LedControl.h"
#include "Rte_LedControl.h"

#define LED_CONTROL_DEFAULT_STATE (0U)
#define LED_CONTROL_DEFAULT_BRIGHTNESS (100U)

void LedControl_Init(void)
{
    /*
     * Start with the LED switched off.
     * The stored default brightness is 100%.
     */
    Rte_LedControl_ApplyOutput(
        LED_CONTROL_DEFAULT_STATE,
        LED_CONTROL_DEFAULT_BRIGHTNESS);
}

void LedControl_MainFunction(void)
{
    uint8_t requestedState;
    uint8_t requestedBrightness;

    if (Rte_LedControl_ReadCommand(
            &requestedState,
            &requestedBrightness) != 0U)
    {
        Rte_LedControl_ApplyOutput(
            requestedState,
            requestedBrightness);
    }
}