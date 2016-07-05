#ifndef MAN_H
#define MAN_H

#include <QObject>
#include <QtSerialPort>

#include "elemer.h"
#include "qt_windows.h"

class MAN : public QSerialPort, public ELEMER {
    Q_OBJECT
public:
    explicit MAN(QObject* parent = 0);

    bool Ping(const QString& portName);
    void PingSlot(const QString& portName);

    QList<double> GetValues(int tupe = 0);
    void GetValuesSlot(QList<double>& data, int type);

    bool SetLoad(const QList<float>& load);
    void SetLoadSlot(const QList<float>& load);

    bool SetLoadEnabled(const QList<bool>& load);
    void SetLoadEnabledSlot(const QList<bool>& load);

signals:
    void PingSignal(const QString&);
    void GetValuesSignal(QList<double>&, int);
    void SetLoadSignal(const QList<float>&);
    void SetLoadEnabledSignal(const QList<bool>&);
public slots:

private:
    QMutex mutex;

    QString Parcel;
    QStringList Array;

    QList<float> LoadCurrent;
    QList<double> MeasVal;
    QList<bool> LoadEnabled;

    double T1;
    long long T2;
    long long T3;

    void start()
    {
        ::QueryPerformanceCounter((LARGE_INTEGER*)&T2);
    }
    void stop()
    {
        ::QueryPerformanceCounter((LARGE_INTEGER*)&T3);
        qDebug() << ((T3 - T2) / T1);
    }

    int WriteRead(QString& Parcel);
};

#endif // MAN_H
