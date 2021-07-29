#pragma once

#include <QtSerialPort>
#include <commoninterfaces.h>
#include <ed_device.h>

class Man : public Elemer::Device {
    Q_OBJECT

public:
    Man(QObject* parent = nullptr);
    Elemer::DeviceType type() const override { return Elemer::MAN; }

    bool setCurrent(double value, int addr = 0);
    bool setEnabledCurrent(bool enable, int addr = 0);
    bool getCurrent(int addr = 0);
    bool getVoltage(const QVector<int> &addr);

private:
    //    // uint getUintData(QByteArray data);
    //    // bool getSuccess(QByteArray data);
    //    // QByteArray m_data;
    //    QList<QByteArray> m_data;
    std::map<int, bool> m_load;
    std::map<int, float> m_current;
    QVector<float> m_valuesV { 0.0, 0.0, 0.0, 0.0 };
    QVector<float> m_valuesI { 0.0, 0.0, 0.0, 0.0 };

    //    int dev;
    //    int serNum;
    //    int address;
    //    int chMum;
    //    int baudRate;
    //    int outPin;
    //    int out;

    enum uartCmd : uint8_t {
        GetType = 0,
        GetData = 1,

        GetProtocolType = 32,
        SetDevAddress = 33,
        SetDevSpeed = 34,

        // ReadPar = 37,
        // WritePar = 38,
        // ModifPar = 39,

        FOpen = 40,
        FSeek = 41,
        FRead = 42,
        FWrite = 43,
        FClose = 44,
        SetPar = 73, // Запись уставок и сотояния кнопок
        GetVer = 0XFE,
        ResetCpu = 0XFF

    };
    enum class DataType : uint8_t {
        Voltage,
        Current,
        Temperature
    };

signals:
    void SetValue(const QVector<quint16>&);
    void Voltage(const QVector<float>&);
    void Current(const QVector<float>&);

    //private:
    //    bool m_connected = false;
    //    bool m_result = false;
    //    int m_counter = 0;
    //    MANPort* m_port;
    //    QMutex m_mutex;
    //    QSemaphore m_semaphore;
    //    QThread m_portThread;
};
