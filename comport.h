#pragma once

#include <QObject>
#include <QByteArray>
#include <QSerialPort>
#include <QTimer>

#include "icomport.h"

class QSerialPort;

namespace PC {

class ComPort : public IComPort, public QObject
{
public:
    ComPort();
    ~ComPort();

    void sendMessage(const char *) override;
    const char *readMessage() override;

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
};

}
