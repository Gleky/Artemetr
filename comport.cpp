#include "comport.h"
#include "keywords.h"

#include <QSerialPortInfo>
#include <QDebug>


ComPort::ComPort()
{
    connect(&_connectTimer, &QTimer::timeout, this, &ComPort::tryConnect);
    _connectTimer.setInterval(5000);
    _connectTimer.start();
    tryConnect();
}

void ComPort::sendMessage(char *)
{

}

const char *ComPort::readMessage()
{
    return _lastMessage.data();
}

void ComPort::messageReceived()
{
    if (_port == nullptr) {
        auto port = dynamic_cast<QSerialPort *>(sender());
        connectPort(port);
    } else {
        if (_lastMessage.contains(movingCamId)){
            portDisconnected();
            return;
        }
        _lastMessage = _port->readLine();
        notifySubscribers();
    }
}

void ComPort::portDisconnected(/*QSerialPort::SerialPortError error*/)
{
    qDebug() << "Disconnected! =(";
    if (_port != nullptr) {
        _port->close();
        _port->deleteLater();
        _port = nullptr;
    }
    _connectTimer.start();
    _lastMessage.clear();
}

void ComPort::tryConnect()
{
    if (_availablePorts.size() > 0) {
        for (auto port : _availablePorts){
            port->close();
            delete port;
        }
        _availablePorts.clear();
    }

    auto ports = QSerialPortInfo::availablePorts();
    for (auto port : ports) {
        auto tryPort = new QSerialPort(port);
        _availablePorts.append(tryPort);
        connect(tryPort, &QSerialPort::readyRead, this, &ComPort::messageReceived);
        tryPort->setBaudRate(connnectionSpeed);
        tryPort->open(QIODevice::ReadWrite);
    }
}

void ComPort::connectPort(QSerialPort *port)
{
    QString read(port->readLine());
    if (read.contains(movingCamId))
        port->write(connectRequest);
    else if (read.contains(connectApprove)) {
        _port = port;
        qDebug() << "Connected! =)";
        _connectTimer.stop();
        connect(_port, &QSerialPort::errorOccurred, this, &ComPort::portDisconnected);
        for (auto prt : _availablePorts)
            if (prt != _port) {
                prt->close();
                delete prt;
            }
        _availablePorts.clear();
    }
}
