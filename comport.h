#pragma once

#include <QObject>
#include <QByteArray>
#include <QSerialPort>
#include <QTimer>

#include "icomport.h"

class QSerialPort;

namespace PC {

class ComPort : public QObject, public IComPort
{
    Q_OBJECT
public:
    ComPort();
    ~ComPort();

    void sendMessage(const char *msg) override;
    const char *readMessage() override;

signals:
    void connected();
    void disconnected();

private slots:
    void messageReceived();
    void portDisconnected();
    void tryConnect();

private:
    void connectPort(QSerialPort *port);

    QTimer _connectTimer;

    QSerialPort *_port = nullptr;
    QByteArray _lastMessage;

    QList<QSerialPort *> _availablePorts;

    bool _arduinoNeedUpdate = false;
};

}
