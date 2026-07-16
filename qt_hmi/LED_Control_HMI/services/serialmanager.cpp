/**
 * @file serialmanager.cpp
 * @brief Serial communication service implementation.
 */

#include "serialmanager.h"

#include <QSerialPortInfo>

SerialManager::SerialManager(QObject *parent)
    : QObject(parent)
{
    connect(&m_serialPort,
            &QSerialPort::readyRead,
            this,
            &SerialManager::handleReadyRead);

    connect(&m_serialPort,
            &QSerialPort::errorOccurred,
            this,
            &SerialManager::handleSerialError);
}

QStringList SerialManager::availablePorts() const
{
    QStringList ports;

    const QList<QSerialPortInfo> portInfos =
        QSerialPortInfo::availablePorts();

    for (const QSerialPortInfo &portInfo : portInfos)
    {
        ports.append(portInfo.portName());
    }

    return ports;
}

bool SerialManager::connectToPort(const QString &portName)
{
    if (m_serialPort.isOpen())
    {
        m_serialPort.close();
    }

    m_serialPort.setPortName(portName);
    m_serialPort.setBaudRate(QSerialPort::Baud9600);
    m_serialPort.setDataBits(QSerialPort::Data8);
    m_serialPort.setParity(QSerialPort::NoParity);
    m_serialPort.setStopBits(QSerialPort::OneStop);
    m_serialPort.setFlowControl(QSerialPort::NoFlowControl);

    if (!m_serialPort.open(QIODevice::ReadWrite))
    {
        emit connectionError(m_serialPort.errorString());
        return false;
    }

    m_receiveBuffer.clear();

    emit connected(portName);

    return true;
}

void SerialManager::disconnectPort()
{
    if (m_serialPort.isOpen())
    {
        m_serialPort.close();
        emit disconnected();
    }
}

bool SerialManager::isConnected() const
{
    return m_serialPort.isOpen();
}

QString SerialManager::portName() const
{
    return m_serialPort.portName();
}

void SerialManager::sendLedCommand(bool ledEnabled, int brightness)
{
    if (!m_serialPort.isOpen())
    {
        emit connectionError(
            QStringLiteral("Serial port is not connected."));
        return;
    }

    brightness = qBound(0, brightness, 100);

    const QByteArray state =
        ledEnabled ? QByteArrayLiteral("ON")
                   : QByteArrayLiteral("OFF");

    const QByteArray frame =
        QByteArrayLiteral("$LED,") +
        state +
        QByteArrayLiteral(",") +
        QByteArray::number(brightness) +
        QByteArrayLiteral("\r\n");

    m_serialPort.write(frame);
}

void SerialManager::handleReadyRead()
{
    m_receiveBuffer.append(m_serialPort.readAll());

    qsizetype newlineIndex = m_receiveBuffer.indexOf('\n');

    while (newlineIndex >= 0)
    {
        QByteArray line =
            m_receiveBuffer.left(newlineIndex);

        m_receiveBuffer.remove(0, newlineIndex + 1);

        if (!line.isEmpty() && line.endsWith('\r'))
        {
            line.chop(1);
        }

        emit lineReceived(
            QString::fromUtf8(line));

        newlineIndex = m_receiveBuffer.indexOf('\n');
    }
}

void SerialManager::handleSerialError(
    QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::NoError)
    {
        return;
    }

    if (error == QSerialPort::ResourceError)
    {
        const QString message = m_serialPort.errorString();

        m_serialPort.close();

        emit connectionError(message);
        emit disconnected();
    }
}