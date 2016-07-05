#include "man.h"

MAN::MAN(QObject* parent)
    : QSerialPort(parent)
    , LoadEnabled(QList<bool>() << true << true << true << true)
    , LoadCurrent(QList<float>() << 0.0f << 0.0f << 0.0f << 0.0f)
{
    setFlowControl(QSerialPort::NoFlowControl);
    setParity(QSerialPort::NoParity);
    setBaudRate(19200);
    setPortName("COM1");

    connect(this, &MAN::PingSignal, this, &MAN::PingSlot);
    connect(this, &MAN::GetValuesSignal, this, &MAN::GetValuesSlot);
    connect(this, &MAN::SetLoadSignal, this, &MAN::SetLoadSlot);
    connect(this, &MAN::SetLoadEnabledSignal, this, &MAN::SetLoadEnabledSlot);

    ::QueryPerformanceFrequency((LARGE_INTEGER*)&T3);
    T1 = T3;
}

bool MAN::Ping(const QString& portName)
{
    mutex.lock();
    emit PingSignal(portName);
    if (mutex.tryLock(500)) {
        mutex.unlock();
        return true;
    }
    mutex.unlock();
    return false;
}

void MAN::PingSlot(const QString& portName)
{
    start();
    int counter = 5;
    setPortName(portName);
    if (open(QSerialPort::ReadWrite)) {
        qDebug() << "open";
        for (int i = 0; i < 4; ++i) {
            Parcel = QString(":%1;254;").arg(i + 1);

            WriteRead(Parcel);

            if (!Check(Parcel, Array)) { //Array[1] = "Ver 5.19"
                close();
                stop();
                return;
            }
            waitForReadyRead(20);
        }
        close();
        mutex.unlock();
    }
    stop();
}

QList<double> MAN::GetValues(int tupe)
{
    QList<double> data;
    mutex.lock();
    emit GetValuesSignal(data, tupe);
    if (mutex.tryLock(2000)) {
        mutex.unlock();
        return data;
    }
    mutex.unlock();
    return data;
}

void MAN::GetValuesSlot(QList<double>& data, int type)
{
    start();
    int counter = 10;
    if (open(QSerialPort::ReadWrite)) {
        for (int i = 0; i < 4; ++i) {
            Parcel = QString(":%1;1;%2;").arg(i + 1).arg(type);

            WriteRead(Parcel);

            if (Check(Parcel, Array)) {
                data << Array[1].toDouble();
            }
            else {
                data << -9999.0;
            }
            waitForReadyRead(50);
        }
        close();
        mutex.unlock();
        stop();
    }
}

bool MAN::SetLoad(const QList<float>& load)
{
    mutex.lock();
    emit SetLoadSignal(load);
    if (mutex.tryLock(1000)) {
        mutex.unlock();
        return true;
    }
    mutex.unlock();
    return false;
}

void MAN::SetLoadSlot(const QList<float>& load)
{
    start();
    LoadCurrent = load;
    int counter = 10;
    bool fl = true;
    if (open(QSerialPort::ReadWrite)) {
        for (int i = 0; i < 4; ++i) {
            Parcel = QString(":%1;73;%2%3;")
                         .arg(i + 1)
                         .arg(LoadEnabled[i] ? "01" : "00")
                         .arg(QString(QByteArray().fromRawData((char*)&LoadCurrent.at(i), 4).toHex().toUpper()));

            WriteRead(Parcel);

            if (!Check(Parcel, Array)) {
                fl = false;
            }
        }
        close();
        if (fl)
            mutex.unlock();
    }
    stop();
}

bool MAN::SetLoadEnabled(const QList<bool>& load)
{
    mutex.lock();
    emit SetLoadEnabledSignal(load);
    if (mutex.tryLock(1000)) {
        mutex.unlock();
        return true;
    }
    mutex.unlock();
    return false;
}

void MAN::SetLoadEnabledSlot(const QList<bool>& load)
{
    start();
    LoadEnabled = load;
    bool fl = true;
    if (open(QSerialPort::ReadWrite)) {
        for (int i = 0; i < 4; ++i) {

            Parcel = QString(":%1;73;%2%3;")
                         .arg(i + 1)
                         .arg(load[i] ? "01" : "00")
                         .arg(QString(QByteArray().fromRawData((char*)&LoadCurrent.at(i), 4).toHex().toUpper()));

            WriteRead(Parcel);

            if (!Check(Parcel, Array)) {
                fl = false;
            }
        }
        close();
        if (fl)
            mutex.unlock();
    }
    stop();
}

int MAN::WriteRead(QString& Parcel)
{
    waitForReadyRead(10);
    Parcel += ControlSum(Parcel) + '\r';
    write(Parcel.toLocal8Bit());
    waitForReadyRead(100);
    Parcel = readAll();
    int counter = 5;
    while (!Parcel.count('\r') && 0 < counter--) {
        waitForReadyRead(10);
        Parcel.append(readAll());
    }
    return Parcel.length();
}
