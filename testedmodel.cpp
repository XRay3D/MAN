#include "testedmodel.h"
#include "magicgetruntime.h"
#include "measuremodel.h"

#include <QDataStream>
#include <QFile>

QDataStream& operator<<(QDataStream& stream, const TestedTrans& tt)
{
    stream << tt.CurrentINoLoad; //	mA

    stream << tt.VoltageII1NoLoad; //	В
    stream << tt.VoltageII2NoLoad; //	В
    stream << tt.VoltageII3NoLoad; //	В
    stream << tt.VoltageII4NoLoad; //	В

    stream << tt.VoltageII1Load; //	В
    stream << tt.VoltageII2Load; //	В
    stream << tt.VoltageII3Load; //	В
    stream << tt.VoltageII4Load; //	В
    return stream;
}
QDataStream& operator>>(QDataStream& stream, TestedTrans& tt)
{
    stream >> tt.CurrentINoLoad; //	mA

    stream >> tt.VoltageII1NoLoad; //	В
    stream >> tt.VoltageII2NoLoad; //	В
    stream >> tt.VoltageII3NoLoad; //	В
    stream >> tt.VoltageII4NoLoad; //	В

    stream >> tt.VoltageII1Load; //	В
    stream >> tt.VoltageII2Load; //	В
    stream >> tt.VoltageII3Load; //	В
    stream >> tt.VoltageII4Load; //	В
    return stream;
}
TestedModel::TestedModel(QObject* parent)
    : QAbstractTableModel(parent)
{
    m_data.reserve(100);
    if (QFile file("file.dat"); file.open(QIODevice::ReadOnly)) {
        QDataStream in(&file);
        in >> m_data;
    }
}

TestedModel::~TestedModel()
{
    if (QFile file("file.dat"); file.open(QIODevice::WriteOnly)) {
        QDataStream out(&file);
        out << m_data;
    }
}

QVariant TestedModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole || role == Qt::EditRole)
        return get_at(m_data[index.row()], index.column());
    return {};
}

bool TestedModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (role == Qt::EditRole) {
        visit_at(m_data[index.row()], index.column(), [value](auto&& arg) { arg = value.value<std::decay_t<decltype(arg)>>(); });
        return true;
    }
    return {};
}

QVariant TestedModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    static const QString hd[] {
        "Ii хх",
        "Uii 1 хх",
        "Uii 2 хх",
        "Uii 3 хх",
        "Uii 4 хх",
        "Uii 1 нагр",
        "Uii 2 нагр",
        "Uii 3 нагр",
        "Uii 4 нагр",
    };
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return hd[section];
    }
    return QAbstractTableModel::headerData(section, orientation, role);
    return {};
}

Qt::ItemFlags TestedModel::flags(const QModelIndex&) const
{
    return Qt::ItemIsEnabled | Qt::ItemIsEditable;
}

void TestedModel::addFromMM(MeasureModel* model, bool load)
{
    if ((m_load = load)) {
        ++lastLoad;
        if (lastLoad > m_data.size()) {
            emit beginInsertRows({}, m_data.size(), m_data.size());
            m_data.resize(lastLoad);
            emit endInsertRows();
        }
        auto& data = m_data[lastLoad - 1];
        for (int i = 0; i < 4; ++i)
            visit_at(data, i + 5, [val = model->m_data[0][i]](auto&& arg) { arg = val; });
    } else {
        ++lastNoLoad;
        if (lastNoLoad > m_data.size()) {
            emit beginInsertRows({}, m_data.size(), m_data.size());
            m_data.resize(lastNoLoad);
            emit endInsertRows();
        }
        auto& data = m_data[lastNoLoad - 1];
        for (int i = 0; i < 4; ++i)
            visit_at(data, i + 1, [val = model->m_data[0][i]](auto&& arg) { arg = val; });
    }
    emit dataChanged(
        createIndex(std::min(lastLoad, lastNoLoad) - 1, CurrentINoLoad),
        createIndex(std::max(lastLoad, lastNoLoad), ColumnCount - 1), { Qt::DisplayRole });
}

void TestedModel::setCurrent(double val)
{
    if (!m_load) {
        m_data[lastNoLoad - 1].CurrentINoLoad = val;
        emit dataChanged(
            createIndex(std::min(lastLoad, lastNoLoad) - 1, CurrentINoLoad),
            createIndex(std::max(lastLoad, lastNoLoad), CurrentINoLoad), { Qt::DisplayRole });
    }
}

void TestedModel::clear()
{
    emit beginRemoveRows({}, 0, m_data.size() - 1);
    m_data.clear();
    lastLoad = {};
    lastNoLoad = {};
    m_load = {};
    emit endRemoveRows();
}

int TestedModel::last() const { return m_load ? lastLoad : lastNoLoad; }

QString TestedModel::toString() const
{
    QString str;
    for (auto& row : m_data) {
        for (int i = 0; i < pod_size_v<TestedTrans>; ++i) {
            if (i == 1 || i == 5)
                str += " \t";
            visit_at(row, i, [&str](auto&& arg) { str += QString::number(arg) + '\t'; });
        }
        str.append('\n');
    }
    return str;
}
