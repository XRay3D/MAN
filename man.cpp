#include "man.h"

MAN::MAN(QObject* parent)
    : QSerialPort(parent)
    , LoadEnabled(QList<bool>() << true << true << true << true)
{
    setFlowControl(QSerialPort::NoFlowControl);
    setParity(QSerialPort::NoParity);
    setBaudRate(19200);

    connect(this, &MAN::PingSignal, this, &MAN::PingSlot);
    connect(this, &MAN::GetValuesSignal, this, &MAN::GetValuesSlot);
    connect(this, &MAN::SetLoadSignal, this, &MAN::SetLoadSlot);

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
            if (!Check(Parcel, Array)) { //Array[1] = "Ver 5.19"
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

bool MAN::SetLoad(const QList<float> &load)
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

    QString Parcel;
    QStringList Array;
    int counter = 10;
    if (open(QSerialPort::ReadWrite)) {
        for (int i = 0; i < 4; ++i) {
            Parcel = QString(":%1;73;%2%3;")
                         .arg(i + 1)
                         .arg(LoadEnabled[i] ? "01" : "00")
                         .arg(QString(QByteArray().fromRawData((char*)&load.at(i), 4).toHex().toUpper()));
            Parcel = Parcel + ControlSum(Parcel) + '\r';
            qDebug() << Parcel;
            write(Parcel.toLocal8Bit());
            waitForReadyRead(100);
            Parcel = readAll();
            while (!Parcel.count('\r') && counter--) {
                waitForReadyRead(10);
                Parcel.append(readAll());
            }
            qDebug() << Parcel;
            if (Check(Parcel, Array)) {
                //                    data << Array[1].toDouble();
            }
            waitForReadyRead(50);
        }
        close();
        mutex.unlock();
    }
}
