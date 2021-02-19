#pragma once

#include <QtCore/QAbstractTableModel>
#include <QtCore/QVariant>
#include <QtCore/qglobal.h>
#include <QtWidgets>

struct Coil {
    uint WindingNumber; //       Номер обмотки.
    QString PinNumbers; //          Номера выводов.
    double NoLoadVoltage; //        Напряжение без нагрузки, V.
    double NoLoadVoltageSpread; //	Напряжение без нагрузки, V.
    double LoadVoltage; //          Напряжение под нагрузкой, V.
    double LoadVoltageSpread; //	Напряжение под нагрузкой, V.
    double RatedOutputCurrentII; //	Номинальный ток II , А  .
    uint ChannelNumberMAN; //	Номинальный ток II , А  .
};

struct Trans {
    QString Name;
    double NoLoadCurrentConsumptionI; //	Ток потребления холостого хода I , А
    std::vector<Coil> PrimaryCoils;
    std::vector<Coil> SecondaryCoils;
};

class CoilModel final : public QAbstractTableModel {
    Q_OBJECT

    Trans& m_data;

    enum {
        WindingNumber, //	Номер обмотки.
        PinТumbers, //	Номера выводов.
        NoLoadVoltage, //	Напряжение без нагрузки, V.
        NoLoadVoltageSpread, //	Напряжение без нагрузки, V.
        LoadVoltage, //	Напряжение под нагрузкой, V.
        LoadVoltageSpread, //	Напряжение под нагрузкой, V.
        RatedOutputCurrentII, //	Номинальный ток II , А  .
        ChannelNumberMAN,
        ColumnCount
    };

public:
    explicit CoilModel(Trans& data, QObject* parent = nullptr);
    ~CoilModel();
    // QAbstractItemModel interface
    int rowCount(const QModelIndex& = {}) const override { return static_cast<int>(m_data.PrimaryCoils.size() + m_data.SecondaryCoils.size()); }
    int columnCount(const QModelIndex& = {}) const override { return ColumnCount; }

    QVariant data(const QModelIndex& index, int role) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
};

class TransModel final : public QAbstractTableModel {
    Q_OBJECT

    std::vector<Trans> m_data;

    void load();
    void save();

    enum {
        Name,
        NoLoadCurrentConsumptionI, //	Ток потребления холостого хода I , А
        LoadVoltage,
        NoLoadVoltage,
        PrimaryCoils,
        SecondaryCoils,
        ColumnCount
    };

public:
    explicit TransModel(QObject* parent = nullptr);
    ~TransModel();
    // QAbstractItemModel interface
    int rowCount(const QModelIndex& = {}) const override { return static_cast<int>(m_data.size()); }
    int columnCount(const QModelIndex& = {}) const override { return ColumnCount; }

    QVariant data(const QModelIndex& index, int role) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

    Trans& trans(int idx) { return m_data[idx]; }
    const Trans& trans(int idx) const { return m_data[idx]; }
};

class CoilDialog : public QDialog {
    QTableView* tableView;

public:
    CoilDialog(Trans& trans)
    {
        if (objectName().isEmpty())
            setObjectName(QString::fromUtf8("CoilDialog"));
        resize(800, 600);
        tableView = new QTableView(this);
        tableView->setObjectName(QString::fromUtf8("tableView"));
        tableView->setModel(new CoilModel(trans, tableView));

        auto verticalLayout = new QVBoxLayout(this);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->addWidget(tableView);
        verticalLayout->setMargin(6);

        tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
        tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);

        QMetaObject::connectSlotsByName(this);
    }
    ~CoilDialog() { }
};

class TransDialog : public QDialog {
    QTableView* tableView;

public:
    TransDialog(QAbstractItemModel* model)
    {
        if (objectName().isEmpty())
            setObjectName(QString::fromUtf8("TransDialog"));
        resize(1000, 600);
        tableView = new QTableView(this);
        tableView->setObjectName(QString::fromUtf8("tableView"));
        tableView->setModel(model);

        auto verticalLayout = new QVBoxLayout(this);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->addWidget(tableView);
        verticalLayout->setMargin(6);

        tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
        tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
        tableView->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(tableView, &QTableView::customContextMenuRequested, [model, this](const QPoint& pos) {
            QMenu menu;
            menu.addAction("Редактировать Обмотки", [model, pos, this] {
                CoilDialog(static_cast<TransModel*>(model)->trans(tableView->indexAt(pos).row())).exec();
            });
            menu.exec(tableView->viewport()->mapToGlobal(pos));
        });

        //        connect(tableView, &QTableView::doubleClicked, [model](const QModelIndex& index) {
        //            CoilDialog(static_cast<TransModel*>(model)->trans(index.row())).exec();
        //        });

        QMetaObject::connectSlotsByName(this);
    }
    ~TransDialog() { }
};
