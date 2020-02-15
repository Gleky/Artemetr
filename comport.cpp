#include "comport.h"

#include <QSerialPortInfo>
#include <QSerialPort>
#include <QDebug>


ComPort::ComPort()
{
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
    _lastMessage = _port->readLine();
    notifySubscribers();
}

void ComPort::tryConnect()
{
    auto ports = QSerialPortInfo::availablePorts();
    for (auto port : ports) {
        qDebug() << port.portName();
//        qDebug() << port.description();
//        qDebug() << port.manufacturer();
//        qDebug() << port.vendorIdentifier();
//        qDebug() << port.serialNumber();
//        qDebug() << port.productIdentifier();
        _port = new QSerialPort(port);
        _port->open(QIODevice::ReadWrite);
        connect(_port, &QSerialPort::readyRead, this, &ComPort::messageReceived);
    }
}
