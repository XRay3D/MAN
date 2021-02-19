#include "measuremodel.h"
#include "hwi/interface.h"
#include <QMessageBox>

MeasureModel::MeasureModel(QObject* parent)
    : QAbstractTableModel(parent)
{
}

void MeasureModel::setVoltages(const QVector<float>& val)
{
    for (int i = 0; i < 4; ++i)
        m_data[0][i] = val[i];
    emit dataChanged(createIndex(0, 0), createIndex(0, 3), { Qt::DisplayRole });
}

void MeasureModel::setCurrents(const QVector<float>& val)
{
    for (int i = 0; i < 4; ++i) {
        m_data[1][i] = val[i];
        emit setCurrent(val[i], i + 1);
    }
    emit dataChanged(createIndex(1, 0), createIndex(1, 3), { Qt::DisplayRole });
}

void MeasureModel::setLoad(bool val)
{
    int ctr {};
    for (bool& load : m_load) {
        load = val;
        emit setEnabLoad(val, ++ctr);
    }
    emit dataChanged(createIndex(1, 0), createIndex(1, 3), { Qt::CheckStateRole });
}

void MeasureModel::setTrans(const Trans& trans)
{
    memset(m_spread, 0, sizeof(double) * 8);
    memset(m_voltage, 0, sizeof(double) * 8);
    for (auto& coil : trans.SecondaryCoils) {
        if (coil.ChannelNumberMAN < 1) {
            QMessageBox::critical(nullptr, "Ошибка", trans.Name + ": обмотка не содержит номер канала МАН!\n"
                                                                  "Отредактируйте четез контекстное меню выбора трансформатора.",
                QMessageBox::Ok);
            return;
        }
        m_spread[0][coil.ChannelNumberMAN - 1] = coil.NoLoadVoltageSpread;
        m_spread[1][coil.ChannelNumberMAN - 1] = coil.LoadVoltageSpread;
        m_voltage[0][coil.ChannelNumberMAN - 1] = coil.NoLoadVoltage;
        m_voltage[1][coil.ChannelNumberMAN - 1] = coil.LoadVoltage;
        m_data[1][coil.ChannelNumberMAN - 1] = coil.RatedOutputCurrentII;
        emit setCurrent(coil.RatedOutputCurrentII, coil.ChannelNumberMAN);
    }
    emit dataChanged(createIndex(1, 0), createIndex(1, 3), { Qt::DisplayRole });
}

int MeasureModel::rowCount(const QModelIndex&) const { return 2; }

int MeasureModel::columnCount(const QModelIndex&) const { return 4; }

QVariant MeasureModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole || role == Qt::EditRole)
        return m_data[index.row()][index.column()];
    else if (role == Qt::TextAlignmentRole)
        return Qt::AlignCenter;
    else if (role == Qt::CheckStateRole && index.row())
        return m_load[index.column()] ? Qt::Checked : Qt::Unchecked;
    else if (role == Qt::BackgroundColorRole && !index.row()) {
        const int row = index.row(),
                  col = index.row();
        return (m_voltage[m_load[col]][col] + m_spread[row][col]) >= m_data[row][col]
                && m_data[row][col] >= (m_voltage[m_load[col]][col] - m_spread[m_load[col]][col])
            ? QColor(0x70, 0xFF, 0x70)
            : QColor(0xFF, 0x70, 0x70);
    }
    return {};
}

bool MeasureModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (role == Qt::EditRole) {
        m_data[index.row()][index.column()] = value.toDouble();
        emit setCurrent(m_data[index.row()][index.column()], index.column() + 1);
        return true;
    } else if (role == Qt::CheckStateRole) {
        m_load[index.column()] = value.value<Qt::CheckState>() == Qt::Checked;
        emit setEnabLoad(m_load[index.column()], index.column() + 1);
        emit updateQCheckBox(m_load);
        return true;
    }
    return {};
}

QVariant MeasureModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal)
            return QString("Канал %1").arg(++section);
        else
            return section == 0 ? "Напряжение   " : "";
    }
    return {};
}

Qt::ItemFlags MeasureModel::flags(const QModelIndex& index) const
{
    return Qt::ItemIsEnabled | (index.row() ? Qt::ItemIsUserCheckable | Qt::ItemIsEditable : Qt::NoItemFlags);
}
