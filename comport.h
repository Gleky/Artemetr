#pragma once

#include <QObject>
#include <QByteArray>

#include "icomport.h"

class QSerialPort;

class ComPort : public IComPort, public QObject
{
public:
    ComPort();
    void sendMessage(char *) override;
    const char *readMessage() override;

private slots:
    void messageReceived();

private:
    void tryConnect();
    bool isConnected = false;

    QSerialPort *_port;
    QByteArray _lastMessage;
};

