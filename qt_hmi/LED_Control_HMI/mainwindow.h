#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QTimer>

#include "serialmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void refreshSerialPorts();
    void handleConnectButton();
    void handleLedOn();
    void handleLedOff();
    void handleBrightnessChanged(int value);
    void sendBrightnessUpdate();

    void handleSerialConnected(const QString &portName);
    void handleSerialDisconnected();
    void handleSerialError(const QString &message);
    void handleSerialLine(const QString &line);

private:
    void setControlEnabled(bool enabled);
    void appendLog(const QString &message);
    void sendCurrentLedCommand();

    Ui::MainWindow *ui;
    SerialManager *m_serialManager;
    QTimer *m_brightnessTimer;

    bool m_ledEnabled;
    int m_brightness;
};

#endif /* MAINWINDOW_H */