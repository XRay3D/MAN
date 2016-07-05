#include "man.h"

MAN::MAN(QObject* parent)
    : QSerialPort(parent)
{
    setFlowControl(QSerialPort::NoFlowControl);
    setParity(QSerialPort::NoParity);
    setBaudRate(19200);

    connect(this, &MAN::PingSignal, this, &MAN::PingSlot);
    connect(this, &MAN::GetValuesSignal, this, &MAN::GetValuesSlot);

    ::QueryPerformanceFrequency((LARGE_INTEGER*)&T3);
    T1 = T3;
}

bool MAN::Ping(const QString& portName)
{
    mutex.lock();
    emit PingSignal(portName);
    if (mutex.tryLock(1000)) {
        mutex.unlock();
        return true;
    }
    mutex.unlock();
    return false;
}

void MAN::PingSlot(const QString& portName)
{
    QString Parcel;
    QStringList Array;
    int counter = 5;

    setPortName(portName);
    if (open(QSerialPort::ReadWrite)) {
        qDebug() << "open";
        for (int i = 0; i < 4; ++i) {
            Parcel = QString(":%1;254;").arg(i + 1);
            Parcel = Parcel + ControlSum(Parcel) + '\r';
            write(Parcel.toLocal8Bit());
            waitForReadyRead(100);
            Parcel = readAll();
            while (!Parcel.count('\r') && counter--) {
                waitForReadyRead(10);
                Parcel.append(readAll());
            }
            if (!Check(Parcel, Array)) {
                close();
                return;
            }
            waitForReadyRead(20);
        }
        close();
        mutex.unlock();
    }
}

QList<double> MAN::GetValues()
{
    QList<double> data;
    mutex.lock();
    emit GetValuesSignal(data);
    if (mutex.tryLock(2000)) {
        mutex.unlock();
        return data;
    }
    mutex.unlock();
    return data;
}

void MAN::GetValuesSlot(QList<double>& data)
{
    QString Parcel;
    QStringList Array;
    int counter = 10;
    if (open(QSerialPort::ReadWrite)) {
        for (int i = 0; i < 4; ++i) {
            Parcel = QString(":%1;1;0;").arg(i + 1);
            Parcel = Parcel + ControlSum(Parcel) + '\r';
            write(Parcel.toLocal8Bit());
            waitForReadyRead(100);
            Parcel = readAll();
            while (!Parcel.count('\r') && counter--) {
                waitForReadyRead(10);
                Parcel.append(readAll());
            }
            if (Check(Parcel, Array)) {
                data << Array[1].toDouble();
            }
            waitForReadyRead(50);
        }
        close();
        mutex.unlock();
    }
}
