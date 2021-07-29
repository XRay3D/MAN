#include "man.h"

#include <QDebug>
#include <QElapsedTimer>

using namespace Elemer;

Man::Man(QObject* parent)
    : Device(parent)
{
}

bool Man::setCurrent(double value, int addr)
{
    QMutexLocker locker(&m_mutex);
    m_current[addr] = value;
    m_address = addr;
    return isConnected() && writeHex<SetPar>(m_load[addr], m_current[addr]) == 0;
}

bool Man::setEnabledCurrent(bool enable, int addr)
{
    QMutexLocker locker(&m_mutex);
    m_load[addr] = enable;
    m_address = addr;
    return isConnected() && writeHex<SetPar>(m_load[addr], m_current[addr]) == 0;
}

bool Man::getCurrent(int addr)
{
    QMutexLocker locker(&m_mutex);
    if (!m_connected)
        return false;

    bool ok {};
    auto readChannels = [this](std::vector<int>&& channels) {
        for (int currentAddr : channels) {
            m_address = currentAddr;
            float val {};
            if (!read<GetData, DataType::Voltage>(val))
                return false;
            constexpr double k = 0.05;
            if (abs(val - m_valuesI[currentAddr - 1]) > 1.0)
                m_valuesI[currentAddr - 1] = val;
            else
                m_valuesI[currentAddr - 1] = m_valuesI[currentAddr - 1] * (1.0 - k) + val * k;
        }
        return true;
    };
    ok = addr ? readChannels({ addr }) : readChannels({ 1, 2, 3, 4 });
    emit Current(m_valuesI);
    return ok;
}

bool Man::getVoltage(const QVector<int>& addr)
{
    QMutexLocker locker(&m_mutex);
    if (!m_connected)
        return false;

    for (int currentAddr : addr) {
        m_address = currentAddr;
        float val {};
        if (!read<GetData, DataType::Voltage>(val))
            return false;
        constexpr double k = 0.05;
        if (abs(val - m_valuesV[currentAddr - 1]) > 2.0)
            m_valuesV[currentAddr - 1] = val;
        else
            m_valuesV[currentAddr - 1] = m_valuesV[currentAddr - 1] * (1.0 - k) + val * k;
    }

    emit Voltage(m_valuesV);
    return true;
}
