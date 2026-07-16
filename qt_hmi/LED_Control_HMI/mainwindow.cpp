/**
 * @file mainwindow.cpp
 * @brief Main window implementation for the S32K144 LED Control HMI.
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDateTime>
#include <QSignalBlocker>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_serialManager(new SerialManager(this))
    , m_brightnessTimer(new QTimer(this))
    , m_ledEnabled(false)
    , m_brightness(0)
{
    ui->setupUi(this);

    setWindowTitle(QStringLiteral("S32K144 LED Control HMI"));

    m_brightnessTimer->setSingleShot(true);
    m_brightnessTimer->setInterval(80);

    connect(m_brightnessTimer,
            &QTimer::timeout,
            this,
            &MainWindow::sendBrightnessUpdate);

    ui->sliderBrightness->setRange(0, 100);
    ui->sliderBrightness->setValue(m_brightness);

    ui->labelBrightnessValue->setText(
        QStringLiteral("%1%").arg(m_brightness));

    ui->plainTextEditLog->setReadOnly(true);

    setControlEnabled(false);

    connect(ui->buttonRefresh,
            &QPushButton::clicked,
            this,
            &MainWindow::refreshSerialPorts);

    connect(ui->buttonConnect,
            &QPushButton::clicked,
            this,
            &MainWindow::handleConnectButton);

    connect(ui->buttonOn,
            &QPushButton::clicked,
            this,
            &MainWindow::handleLedOn);

    connect(ui->buttonOff,
            &QPushButton::clicked,
            this,
            &MainWindow::handleLedOff);

    connect(ui->sliderBrightness,
            &QSlider::valueChanged,
            this,
            &MainWindow::handleBrightnessChanged);

    connect(m_serialManager,
            &SerialManager::connected,
            this,
            &MainWindow::handleSerialConnected);

    connect(m_serialManager,
            &SerialManager::disconnected,
            this,
            &MainWindow::handleSerialDisconnected);

    connect(m_serialManager,
            &SerialManager::connectionError,
            this,
            &MainWindow::handleSerialError);

    connect(m_serialManager,
            &SerialManager::lineReceived,
            this,
            &MainWindow::handleSerialLine);

    refreshSerialPorts();
    appendLog(QStringLiteral("HMI started."));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::refreshSerialPorts()
{
    const QString previousPort =
        ui->comboBoxPort->currentText();

    ui->comboBoxPort->clear();
    ui->comboBoxPort->addItems(
        m_serialManager->availablePorts());

    const int previousIndex =
        ui->comboBoxPort->findText(previousPort);

    if (previousIndex >= 0)
    {
        ui->comboBoxPort->setCurrentIndex(previousIndex);
    }

    appendLog(QStringLiteral("Serial port list refreshed."));
}

void MainWindow::handleConnectButton()
{
    if (m_serialManager->isConnected())
    {
        m_serialManager->disconnectPort();
        return;
    }

    const QString portName =
        ui->comboBoxPort->currentText();

    if (portName.isEmpty())
    {
        appendLog(QStringLiteral("No serial port selected."));
        return;
    }

    appendLog(
        QStringLiteral("Connecting to %1 at 9600 baud...")
            .arg(portName));

    m_serialManager->connectToPort(portName);
}

void MainWindow::handleLedOn()
{
    m_brightnessTimer->stop();

    m_ledEnabled = true;
    m_brightness = 100;

    /*
     * Block slider signal to avoid starting the debounce timer
     * while updating the slider programmatically.
     */
    const QSignalBlocker blocker(ui->sliderBrightness);

    ui->sliderBrightness->setValue(m_brightness);
    ui->labelBrightnessValue->setText(
        QStringLiteral("%1%").arg(m_brightness));

    sendCurrentLedCommand();
}

void MainWindow::handleLedOff()
{
    m_brightnessTimer->stop();

    m_ledEnabled = false;
    m_brightness = 0;

    const QSignalBlocker blocker(ui->sliderBrightness);

    ui->sliderBrightness->setValue(m_brightness);
    ui->labelBrightnessValue->setText(
        QStringLiteral("%1%").arg(m_brightness));

    sendCurrentLedCommand();
}

void MainWindow::handleBrightnessChanged(int value)
{
    m_brightness = value;

    ui->labelBrightnessValue->setText(
        QStringLiteral("%1%").arg(m_brightness));

    /*
     * Slider value determines the LED state directly:
     *
     * value > 0 -> LED ON
     * value = 0 -> LED OFF
     */
    m_ledEnabled = (m_brightness > 0);

    if (m_serialManager->isConnected())
    {
        /*
         * Restart debounce timer.
         * Only the latest slider value is transmitted.
         */
        m_brightnessTimer->start();
    }
}

void MainWindow::sendBrightnessUpdate()
{
    if (m_serialManager->isConnected())
    {
        sendCurrentLedCommand();
    }
}

void MainWindow::handleSerialConnected(
    const QString &portName)
{
    m_brightnessTimer->stop();

    ui->buttonConnect->setText(
        QStringLiteral("Disconnect"));

    ui->comboBoxPort->setEnabled(false);
    ui->buttonRefresh->setEnabled(false);

    setControlEnabled(true);

    appendLog(
        QStringLiteral("Connected to %1.").arg(portName));
}

void MainWindow::handleSerialDisconnected()
{
    m_brightnessTimer->stop();

    ui->buttonConnect->setText(
        QStringLiteral("Connect"));

    ui->comboBoxPort->setEnabled(true);
    ui->buttonRefresh->setEnabled(true);

    setControlEnabled(false);

    appendLog(QStringLiteral("Serial port disconnected."));
}

void MainWindow::handleSerialError(
    const QString &message)
{
    appendLog(
        QStringLiteral("Serial error: %1").arg(message));
}

void MainWindow::handleSerialLine(
    const QString &line)
{
    appendLog(
        QStringLiteral("ECU: %1").arg(line));

    if (line.startsWith(QStringLiteral("$ACK,ON,")))
    {
        m_ledEnabled = true;
    }
    else if (line.startsWith(
                 QStringLiteral("$ACK,OFF,")))
    {
        m_ledEnabled = false;
    }
}

void MainWindow::setControlEnabled(bool enabled)
{
    ui->buttonOn->setEnabled(enabled);
    ui->buttonOff->setEnabled(enabled);
    ui->sliderBrightness->setEnabled(enabled);
}

void MainWindow::appendLog(
    const QString &message)
{
    const QString timestamp =
        QDateTime::currentDateTime().toString(
            QStringLiteral("HH:mm:ss"));

    ui->plainTextEditLog->appendPlainText(
        QStringLiteral("[%1] %2")
            .arg(timestamp, message));
}

void MainWindow::sendCurrentLedCommand()
{
    if (!m_serialManager->isConnected())
    {
        appendLog(
            QStringLiteral(
                "Cannot send: serial port is not connected."));
        return;
    }

    m_serialManager->sendLedCommand(
        m_ledEnabled,
        m_brightness);

    appendLog(
        QStringLiteral("HMI: $LED,%1,%2")
            .arg(m_ledEnabled
                     ? QStringLiteral("ON")
                     : QStringLiteral("OFF"))
            .arg(m_brightness));
}