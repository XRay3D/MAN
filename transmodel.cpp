#include "transmodel.h"
#include "magicgetruntime.h"

#include <QByteArray>
#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>

static const std::map<int, const char*> coilKeys {
    { 0, "WindingNumber" },
    { 1, "PinNumbers" },
    { 2, "NoLoadVoltage" },
    { 3, "NoLoadVoltageSpread" },
    { 4, "LoadVoltage" },
    { 5, "LoadVoltageSpread" },
    { 6, "RatedOutputCurrentII" },
    { 7, "ChannelNumberMAN" }
};

void TransModel::load()
{

    QFile file("Trans.json");
    if (file.open(QFile::ReadOnly)) {
        auto doc { QJsonDocument::fromJson(file.readAll()) };
        QJsonArray array(doc.array());
        qDebug() << "array" << array.size();

        m_data.reserve(array.size());

        for (int i = 0; i < array.size(); ++i) {
            auto transObj { array[i].toObject() };
            m_data.resize(m_data.size() + 1);
            auto& trans = m_data.back();
            trans.Name = transObj.value("Name").toString();
            trans.NoLoadCurrentConsumptionI = transObj.value("NoLoadCurrentConsumptionI").toDouble();
            auto readCoil = [](std::vector<Coil>& coils, QJsonArray array) {
                coils.reserve(array.size());
                for (auto jRef : array) {
                    auto coilObj { jRef.toObject() };
                    coils.resize(coils.size() + 1);
                    for (int i = 0; i < pod_size_v<Coil>; ++i)
                        visit_at(coils.back(), i, [val = coilObj.value(coilKeys.at(i)).toVariant()](auto&& arg) {
                            using Ty = std::decay_t<decltype(arg)>;
                            arg = val.value<Ty>();
                        });
                }
            };
            readCoil(trans.PrimaryCoils, transObj.value("PrimaryCoils").toArray());
            readCoil(trans.SecondaryCoils, transObj.value("SecondaryCoils").toArray());
        }
    } else {
        qDebug() << file.errorString();
        m_data.reserve(100);
        QString data("ТП2(220/8)"
                     "|1|1-5        |242          |187        |       |0.04   |\n"
                     "|2|7-9        |11.3±0.5     |6.5±0.4    |0.2    |       |1\n"
                     "ТП8(220/37)М5"
                     "|1|3-6        |242          |154        |       |0.052  |\n"
                     "|2|10-15      |47±1.4       |22±0.65    |0.16   |       |1\n"
                     "ТП10(2x6)37_М8"
                     "|1|1-2.3-4    |12           |12         |       |0.5    |\n"
                     "|2|5-6        |42.5±1.3     |37±1.11    |0.175  |       |1\n"
                     "ТП10(220/37)"
                     "|1|1-2        |242          |187        |       |0.04   |\n"
                     "|2|3-4        |47.4±1.4     |27.5±1.1   |0.175  |       |1\n"
                     "ТП15(2x110/2x14.5х40х8)"
                     "|1|1-4, 7-10  |242          |242        |       |0.06   |\n"
                     "|2|11-12      |16.5±0.52    |16.5±0.52  |       |       |1\n"
                     "|3|13-14      |16.5±0.52    |16.5±0.52  |       |       |2\n"
                     "|4|16-18      |46.4±1.3     |46.4±1.3   |       |       |3\n"
                     "|5|19-20      |9.3±0.31     |9.3±0.31   |       |       |4\n"
                     "ТП15(2x110/4x31)"
                     "|1|1-4, 7-10  |242          |187        |       |0.05   |\n"
                     "|2|11-12      |40±1.2       |24.3±0.83  |0.13   |       |1\n"
                     "|3|13-14      |40±1.2       |24.3±0.83  |0.13   |       |2\n"
                     "|4|17-18      |40±1.2       |24.3±0.83  |0.13   |       |3\n"
                     "|5|19-20      |40±1.2       |24.3±0.83  |0.13   |       |4\n"
                     "ТП15(230/43)М3_Т120"
                     "|1|4-7        |230          |230        |       |0.025  |\n"
                     "|2|15-16      |41.6±1.2     |38±1.0     |0.3    |       |4\n"
                     "ТП15(2x110/4x42)"
                     "|1|1-4, 7-10  |242          |187        |       |0.05   |\n"
                     "|2|11-12      |55.6±8       |32±1.12    |0.09   |       |1\n"
                     "|3|13-14      |55.6±8       |32±1.12    |0.09   |       |2\n"
                     "|4|17-18      |55.6±8       |32±1.12    |0.09   |       |3\n"
                     "|5|19-20      |55.6±8       |32±1.12    |0.09   |       |4\n"
                     "ТП28(2x110/2х26)"
                     "|1|1-4, 7-10  |242          |187        |       |0.065  |\n"
                     "|2|11-13      |32.4±0.93    |20.76±0.62 |0.5    |       |1\n"
                     "|3|18-20      |32.4±0.93    |20.76±0.62 |0.5    |       |2\n"
                     "ТП28(2х110/4х31)"
                     "|1|1-4, 7-10  |242          |187        |       |0.065  |\n"
                     "|2|11-12      |38.8±1.24    |24±0.8     |0.22   |       |1\n"
                     "|3|13-14      |38.8±1.24    |24±0.8     |0.22   |       |2\n"
                     "|4|17-18      |38.8±1.24    |24±0.8     |0.22   |       |3\n"
                     "|5|19-20      |38.8±1.24    |24±0.8     |0.22   |       |4\n"
                     "ТП28(2х110/4х42)"
                     "|1|1-4, 7-10  |242          |187        |       |0.05   |\n"
                     "|2|11-12      |55.6±6.4     |32±0.84    |0.15   |       |1\n"
                     "|3|13-14      |55.6±6.4     |32±0.84    |0.15   |       |2\n"
                     "|4|17-18      |55.6±6.4     |32±0.84    |0.15   |       |3\n"
                     "|5|19-20      |55.6±6.4     |32±0.84    |0.15   |       |4\n"
                     "ТП28(2х110/2х17-8.5-8)"
                     "|1|1-4, 7-10  |242          |187        |       |0.065  |\n"
                     "|2|11-12      |22.6±0.8     |14.25±0.43 |0.25   |       |1\n"
                     "|3|13-14      |22.6±0.8     |14.25±0.43 |0.25   |       |2\n"
                     "|4|15-16      |11.55±0.45   |7±0.41     |0.1    |       |3\n"
                     "|5|19-20      |10.8±0.31    |6.65±0.41  |0.6    |       |4\n"
                     "ТП60 (2х110/2х12-24) М3_Т120"
                     "|1|1-4, 9-12  |242          |187        |       |0.09   |\n"
                     "|2|13-15      |13±0.7       |7.5±0.3    |2.00   |       |1\n"
                     "|3|17-20      |31±0.93      |20.9±0.53  |0.1    |       |2\n"
                     "|4|22-24      |13±0.7       |7.5±0.3    |2.00   |       |3");
        auto tt = data.split("T");
        for (auto& trans : tt) {
            for (auto& wind : trans.split('\n')) {
                auto par = wind.split('|');
                if (par[0].size()) {
                    m_data.resize(m_data.size() + 1);
                    m_data.back().Name = par[0].trimmed();
                    m_data.back().NoLoadCurrentConsumptionI = par[6].toDouble();
                    m_data.back().PrimaryCoils.resize(m_data.back().PrimaryCoils.size() + 1);
                    auto& coil = m_data.back().PrimaryCoils.back();
                    coil.LoadVoltage = par[4].toDouble();
                    coil.LoadVoltageSpread = {};
                    coil.NoLoadVoltage = par[3].toDouble();
                    coil.NoLoadVoltageSpread = {};
                    coil.PinNumbers = par[2].trimmed(); //|Номера выводов
                    coil.WindingNumber = par[1].toInt(); //|Номер обмотки
                } else {
                    m_data.back().SecondaryCoils.resize(m_data.back().SecondaryCoils.size() + 1);
                    auto& coil = m_data.back().SecondaryCoils.back();
                    auto spread = par[3].split("±");
                    coil.NoLoadVoltage = spread[0].toDouble();
                    coil.LoadVoltageSpread = spread[1].toDouble();
                    spread = par[4].split("±");
                    coil.ChannelNumberMAN = par.last().toInt();
                    coil.LoadVoltage = spread[0].toDouble();
                    coil.NoLoadVoltageSpread = spread[1].toDouble();
                    coil.PinNumbers = par[2].trimmed(); //|Номера выводов
                    coil.RatedOutputCurrentII = par[5].toDouble();
                    coil.WindingNumber = par[1].toInt(); //|Номер обмотки
                }
            }
        }
    }
}

void TransModel::save()
{
    QFile file("Trans.json");
    if (file.open(QFile::WriteOnly)) {
        QJsonArray array;
        for (auto& [Name, NoLoadCurrentConsumptionI, PrimaryCoils, SecondaryCoils] : m_data) {
            auto saveCoil = [](std::vector<Coil>& coils) -> QJsonArray {
                QJsonArray array;
                QVariantMap jMap;
                for (auto& coil : coils) {
                    for (int i = 0; i < pod_size_v<Coil>; ++i)
                        visit_at(coil, i, [&val = jMap[coilKeys.at(i)]](auto&& arg) { val = arg; });
                    array.append(QJsonObject::fromVariantMap(jMap));
                }
                return array;
            };
            array.append(QJsonObject {
                { "Name", Name },
                { "NoLoadCurrentConsumptionI", NoLoadCurrentConsumptionI },
                { "PrimaryCoils", saveCoil(PrimaryCoils) },
                { "SecondaryCoils", saveCoil(SecondaryCoils) } });
        }
        file.write(QJsonDocument(array).toJson());
    } else {
        qDebug() << file.errorString();
    }
}
/////////////////////////////////////////////////////////////////////////
/// \brief TransModel::TransModel
/// \param parent
///
TransModel::TransModel(QObject* parent)
    : QAbstractTableModel(parent)
{
    load();
}

TransModel::~TransModel() { save(); }

QVariant TransModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        switch (index.column()) {
        case Name:
            return m_data[index.row()].Name;
        case NoLoadCurrentConsumptionI:
            return m_data[index.row()].NoLoadCurrentConsumptionI;
        case LoadVoltage:
            return m_data[index.row()].PrimaryCoils.back().LoadVoltage;
        case NoLoadVoltage:
            return m_data[index.row()].PrimaryCoils.back().NoLoadVoltage;
        case PrimaryCoils:
            return m_data[index.row()].PrimaryCoils.size();
        case SecondaryCoils:
            return m_data[index.row()].SecondaryCoils.size();
        }
    } else if (role == Qt::ToolTipRole)
        return m_data[index.row()].Name;
    else if (role == Qt::TextAlignmentRole)
        return Qt::AlignCenter;
    return {};
}

bool TransModel::setData([[maybe_unused]] const QModelIndex& index, [[maybe_unused]] const QVariant& value, [[maybe_unused]] int role)
{
    if (role == Qt::EditRole) {
        switch (index.column()) {
        case Name:
            m_data[index.row()].Name = value.toString();
            return true;
        case NoLoadCurrentConsumptionI:
            m_data[index.row()].NoLoadCurrentConsumptionI = value.toDouble();
            return true;
        case LoadVoltage:
            m_data[index.row()].PrimaryCoils.back().LoadVoltage = value.toDouble();
            return true;
        case NoLoadVoltage:
            m_data[index.row()].PrimaryCoils.back().NoLoadVoltage = value.toDouble();
            return true;
        default:
            return {};
        }
    }
    return {};
}

QVariant TransModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    static QString hd[] {
        "Тип",
        "Ток холостого\nхода, А", //|Ток потребления холостого хода I , А
        "Входное напряжение\nпроверки холостого хода, В", //|Ток потребления холостого хода I , А
        "Входное напряжение\nпроверки под нагрузкой, В", //|Ток потребления холостого хода I , А
        "Кол-во первичных\nобмоток",
        "Кол-во вторичных\nобмоток"
    };
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal)
            return hd[section];
        else
            return ++section;
    }
    return {};
}

Qt::ItemFlags TransModel::flags([[maybe_unused]] const QModelIndex& index) const
{
    auto flags { Qt::ItemIsEnabled };
    switch (index.column()) {
    case Name:
        return flags | Qt::ItemIsSelectable;
    case NoLoadCurrentConsumptionI:
    case LoadVoltage:
    case NoLoadVoltage:
        return flags | Qt::ItemIsEditable;
    default:
        return flags;
    }
}
/////////////////////////////////////////////////////////////////////////
/// \brief CoilModel::CoilModel
/// \param data
/// \param parent
///
CoilModel::CoilModel(Trans& data, QObject* parent)
    : QAbstractTableModel(parent)
    , m_data(data)
{
}

CoilModel::~CoilModel() { }

QVariant CoilModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole || role == Qt::EditRole || role == Qt::ToolTipRole) {
        if (size_t row { static_cast<size_t>(index.row()) }; row < m_data.PrimaryCoils.size())
            return get_at(m_data.PrimaryCoils[row], index.column());
        else
            return get_at(m_data.SecondaryCoils[row - m_data.PrimaryCoils.size()], index.column());
    }
    //    else if (role == Qt::ToolTipRole)
    //        return m_data[index.row()].Name;
    else if (role == Qt::TextAlignmentRole)
        return Qt::AlignCenter;
    return {};
}

bool CoilModel::setData([[maybe_unused]] const QModelIndex& index, [[maybe_unused]] const QVariant& value, [[maybe_unused]] int role)
{
    if (role == Qt::DisplayRole || role == Qt::EditRole || role == Qt::ToolTipRole) {
        if (size_t row { static_cast<size_t>(index.row()) }; row < m_data.PrimaryCoils.size())
            visit_at(m_data.PrimaryCoils[row], index.column(), [value](auto&& arg) {
                arg = value.value<std::decay_t<decltype(arg)>>();
            });
        else
            visit_at(m_data.SecondaryCoils[row - m_data.PrimaryCoils.size()], index.column(), [value](auto&& arg) {
                arg = value.value<std::decay_t<decltype(arg)>>();
            });
        return true;
    }
    return {};
}

QVariant CoilModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    static const QString hd[] {
        "Номер\nобмотки",
        "Номера\nвыводов",
        "Напряжение без\nнагрузки, V",
        "Отклонение\n ±V",
        "Напряжение под\nнагрузкой, V",
        "Отклонение\n ±V",
        "Номинальный\nток II , А",
        "№ канала\nМАН",
    };
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal)
            return hd[section];
        else
            return ++section;
    }

    return {};
}

Qt::ItemFlags CoilModel::flags([[maybe_unused]] const QModelIndex& index) const
{
    return Qt::ItemIsEnabled | Qt::ItemIsEditable;
}
