#include "comport.h"
#include "keywords.h"

#include "point.h"

#include <QSerialPortInfo>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QProcess>


void checkNewArduinoFirmware(bool &needUpdate);
void updateArduinoFirmware(QSerialPort *port, bool &needUpdate);

namespace PC {

ComPort::ComPort()
{
    checkNewArduinoFirmware(_arduinoNeedUpdate);
    connect(&_connectTimer, &QTimer::timeout, this, &ComPort::tryConnect);
    _connectTimer.setInterval(5000);
    _connectTimer.start();
    tryConnect();
}

ComPort::~ComPort()
{
    if ( _port != nullptr ) {
        _port->write(disconnectRequest);
        _port->close();
        _port->deleteLater();
    }
}

void ComPort::sendMessage(const char *msg)
{
//    Point newPos;
//    newPos.X = *reinterpret_cast<const int16_t *>( msg+xPos );
//    newPos.Y = *reinterpret_cast<const int16_t *>( msg+yPos );

    if ( _port == nullptr )
        return;

    int x=0;//debug
    x=_port->write(msg, commandSize);
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
        _port->clear();
        qDebug() << _lastMessage << endl;
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
    if ( port == nullptr )
        return;

    QString read(port->readLine());
    if (read.contains(movingCamId)) {
        if ( _arduinoNeedUpdate ) {
            updateArduinoFirmware(port, _arduinoNeedUpdate);
            return;
        }
        port->write(connectRequest); //here port has found
    }
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

}

const QString hwfwDir = "/hwFirmware/";
const QString newFwName = "firmware.hex";
const QString loadedFwName = "current_firmware.hex";

void checkNewArduinoFirmware(bool &needUpdate)
{
    needUpdate = false;

    auto firmwareDir = QDir::currentPath();
    firmwareDir += hwfwDir;

    auto newFirmwarePath = firmwareDir + "firmware.hex";

    if ( QFile::exists( newFirmwarePath ) )
        needUpdate = true;
}

void updateArduinoFirmware(QSerialPort *port,bool &needUpdate)
{
    port->close();
    const auto firmwareDir = QDir::currentPath() + hwfwDir;

    const QString avrDudePath = firmwareDir + "avrdude.exe";
    QStringList arguments = {"-p m328p", "-c arduino", "-b 57600", "-U flash:w:"+loadedFwName};
    QString portNumber = port->portName();
    arguments << "-P "+portNumber;

    auto newFirmwarePath = firmwareDir + newFwName;
    auto firmwarePath = firmwareDir + loadedFwName;
    auto tempFirmwarePath = firmwareDir + loadedFwName + "old";
    QFile::rename(firmwarePath, tempFirmwarePath);
    QFile::rename(newFirmwarePath, firmwarePath);

    bool res = QProcess::execute(avrDudePath);
    if (res) {
        needUpdate = false;
        QFile::remove(tempFirmwarePath);
    }
    else {
        QFile::rename(firmwarePath, newFirmwarePath);
        QFile::rename(tempFirmwarePath, firmwarePath);
    }
}
