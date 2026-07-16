/**
 * @file serialmanager.h
 * @brief Serial communication service for the S32K144 ECU.
 */

#ifndef SERIALMANAGER_H
#define SERIALMANAGER_H

#include <QObject>
#include <QSerialPort>
#include <QStringList>

class SerialManager : public QObject
{
    Q_OBJECT

public:
    explicit SerialManager(QObject *parent = nullptr);

    QStringList availablePorts() const;

    bool connectToPort(const QString &portName);
    void disconnectPort();

    bool isConnected() const;
    QString portName() const;

    void sendLedCommand(bool ledEnabled, int brightness);

signals:
    void connected(const QString &portName);
    void disconnected();
    void connectionError(const QString &message);
    void lineReceived(const QString &line);

private slots:
    void handleReadyRead();
    void handleSerialError(QSerialPort::SerialPortError error);

private:
    QSerialPort m_serialPort;
    QByteArray m_receiveBuffer;
};

#endif /* SERIALMANAGER_H */