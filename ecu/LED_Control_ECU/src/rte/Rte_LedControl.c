/**
 * @file Rte_LedControl.c
 * @brief RTE implementation for the LED Control SWC.
 */

#include "Rte_LedControl.h"
#include "IoHwAb_Led.h"

#define RTE_LED_OFF (0U)
#define RTE_LED_ON (1U)
#define RTE_LED_MAX_BRIGHTNESS (100U)

static uint8_t Rte_LedControl_CommandState = RTE_LED_OFF;
static uint8_t Rte_LedControl_CommandBrightness = 0U;
static uint8_t Rte_LedControl_CommandPending = 0U;

void Rte_LedControl_WriteCommand(
    uint8_t ledState,
    uint8_t brightness)
{
    if (ledState != RTE_LED_OFF)
    {
        ledState = RTE_LED_ON;
    }

    if (brightness > RTE_LED_MAX_BRIGHTNESS)
    {
        brightness = RTE_LED_MAX_BRIGHTNESS;
    }

    Rte_LedControl_CommandState = ledState;
    Rte_LedControl_CommandBrightness = brightness;
    Rte_LedControl_CommandPending = 1U;
}

uint8_t Rte_LedControl_ReadCommand(
    uint8_t *ledState,
    uint8_t *brightness)
{
    if ((ledState == (uint8_t *)0) ||
        (brightness == (uint8_t *)0))
    {
        return 0U;
    }

    if (Rte_LedControl_CommandPending == 0U)
    {
        return 0U;
    }

    *ledState = Rte_LedControl_CommandState;
    *brightness = Rte_LedControl_CommandBrightness;

    Rte_LedControl_CommandPending = 0U;

    return 1U;
}

void Rte_LedControl_ApplyOutput(
    uint8_t ledState,
    uint8_t brightness)
{
    /*
     * Store the new brightness first.
     * If the LED is currently off, it remains off.
     */
    IoHwAb_Led_SetBrightness(brightness);

    if (ledState != RTE_LED_OFF)
    {
        IoHwAb_Led_On();
    }
    else
    {
        IoHwAb_Led_Off();
    }
}

uint8_t Rte_LedControl_GetActualState(void)
{
    return IoHwAb_Led_GetState();
}

uint8_t Rte_LedControl_GetActualBrightness(void)
{
    return IoHwAb_Led_GetBrightness();
}