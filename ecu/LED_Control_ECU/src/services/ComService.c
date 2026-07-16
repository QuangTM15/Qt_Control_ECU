/**
 * @file ComService.c
 * @brief UART communication service implementation.
 */

#include "ComService.h"
#include "Mcal_Uart.h"

#define COM_SERVICE_RX_BUFFER_SIZE (32U)

#define COM_SERVICE_LED_OFF (0U)
#define COM_SERVICE_LED_ON (1U)

#define COM_SERVICE_MIN_BRIGHTNESS (0U)
#define COM_SERVICE_MAX_BRIGHTNESS (100U)

static char ComService_RxBuffer[COM_SERVICE_RX_BUFFER_SIZE];
static uint8_t ComService_RxIndex = 0U;

static ComService_LedCommandType ComService_LatestCommand;
static uint8_t ComService_CommandAvailable = 0U;

/**
 * @brief Resets the receive buffer.
 */
static void ComService_ResetRxBuffer(void)
{
    uint8_t index;

    for (index = 0U; index < COM_SERVICE_RX_BUFFER_SIZE; index++)
    {
        ComService_RxBuffer[index] = '\0';
    }

    ComService_RxIndex = 0U;
}

/**
 * @brief Compares the start of a string with a fixed pattern.
 */
static uint8_t ComService_MatchPrefix(
    const char *text,
    const char *prefix)
{
    uint8_t index = 0U;

    while (prefix[index] != '\0')
    {
        if (text[index] != prefix[index])
        {
            return 0U;
        }

        index++;
    }

    return 1U;
}

/**
 * @brief Converts decimal ASCII digits to an unsigned value.
 */
static uint8_t ComService_ParseBrightness(
    const char *text,
    uint8_t *brightness)
{
    uint16_t value = 0U;
    uint8_t digitCount = 0U;
    uint8_t index = 0U;

    if (brightness == (uint8_t *)0)
    {
        return 0U;
    }

    while ((text[index] >= '0') && (text[index] <= '9'))
    {
        value =
            (uint16_t)((value * 10U) +
                       (uint16_t)(text[index] - '0'));

        digitCount++;
        index++;

        if (digitCount > 3U)
        {
            return 0U;
        }
    }

    if (digitCount == 0U)
    {
        return 0U;
    }

    /*
     * Brightness must be followed by the end of the string.
     */
    if (text[index] != '\0')
    {
        return 0U;
    }

    if (value > COM_SERVICE_MAX_BRIGHTNESS)
    {
        return 0U;
    }

    *brightness = (uint8_t)value;

    return 1U;
}

/**
 * @brief Parses a complete LED command frame.
 *
 * Supported frames:
 *
 * $LED,ON,75
 * $LED,OFF,0
 */
static uint8_t ComService_ParseFrame(void)
{
    uint8_t ledState;
    uint8_t brightness;
    const char *brightnessText;

    if (ComService_MatchPrefix(
            ComService_RxBuffer,
            "$LED,ON,") != 0U)
    {
        ledState = COM_SERVICE_LED_ON;
        brightnessText = &ComService_RxBuffer[8];
    }
    else if (ComService_MatchPrefix(
                 ComService_RxBuffer,
                 "$LED,OFF,") != 0U)
    {
        ledState = COM_SERVICE_LED_OFF;
        brightnessText = &ComService_RxBuffer[9];
    }
    else
    {
        return 0U;
    }

    if (ComService_ParseBrightness(
            brightnessText,
            &brightness) == 0U)
    {
        return 0U;
    }

    ComService_LatestCommand.ledState = ledState;
    ComService_LatestCommand.brightness = brightness;
    ComService_CommandAvailable = 1U;

    return 1U;
}

/**
 * @brief Sends an unsigned integer from 0 to 100 as ASCII.
 */
static void ComService_SendNumber(uint8_t value)
{
    if (value >= 100U)
    {
        Mcal_Uart_SendByte('1');
        Mcal_Uart_SendByte('0');
        Mcal_Uart_SendByte('0');
    }
    else
    {
        if (value >= 10U)
        {
            Mcal_Uart_SendByte(
                (uint8_t)('0' + (value / 10U)));
        }

        Mcal_Uart_SendByte(
            (uint8_t)('0' + (value % 10U)));
    }
}

void ComService_Init(void)
{
    ComService_ResetRxBuffer();

    ComService_LatestCommand.ledState = COM_SERVICE_LED_OFF;
    ComService_LatestCommand.brightness =
        COM_SERVICE_MIN_BRIGHTNESS;

    ComService_CommandAvailable = 0U;

    Mcal_Uart_Init();

    /*
     * Notify the Qt HMI that the ECU is ready.
     */
    Mcal_Uart_SendString("$READY\r\n");
}

void ComService_MainFunction(void)
{
    uint8_t receivedByte;

    while (Mcal_Uart_IsDataAvailable() != 0U)
    {
        receivedByte = Mcal_Uart_ReadByte();

        /*
         * Ignore carriage return.
         */
        if (receivedByte == '\r')
        {
            continue;
        }

        /*
         * Newline marks the end of one frame.
         */
        if (receivedByte == '\n')
        {
            ComService_RxBuffer[ComService_RxIndex] = '\0';

            if (ComService_ParseFrame() != 0U)
            {
                ComService_SendLedStatus(
                    ComService_LatestCommand.ledState,
                    ComService_LatestCommand.brightness);
            }
            else
            {
                Mcal_Uart_SendString("$NACK\r\n");
            }

            ComService_ResetRxBuffer();
        }
        else
        {
            if (ComService_RxIndex <
                (COM_SERVICE_RX_BUFFER_SIZE - 1U))
            {
                ComService_RxBuffer[ComService_RxIndex] =
                    (char)receivedByte;

                ComService_RxIndex++;
            }
            else
            {
                /*
                 * Buffer overflow: discard the incomplete frame.
                 */
                ComService_ResetRxBuffer();
                Mcal_Uart_SendString("$NACK\r\n");
            }
        }
    }
}

uint8_t ComService_IsCommandAvailable(void)
{
    return ComService_CommandAvailable;
}

uint8_t ComService_GetCommand(
    ComService_LedCommandType *command)
{
    if (command == (ComService_LedCommandType *)0)
    {
        return 0U;
    }

    if (ComService_CommandAvailable == 0U)
    {
        return 0U;
    }

    command->ledState =
        ComService_LatestCommand.ledState;

    command->brightness =
        ComService_LatestCommand.brightness;

    ComService_CommandAvailable = 0U;

    return 1U;
}

void ComService_SendLedStatus(
    uint8_t ledState,
    uint8_t brightness)
{
    Mcal_Uart_SendString("$ACK,");

    if (ledState != 0U)
    {
        Mcal_Uart_SendString("ON,");
    }
    else
    {
        Mcal_Uart_SendString("OFF,");
    }

    ComService_SendNumber(brightness);
    Mcal_Uart_SendString("\r\n");
}