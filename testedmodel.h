#pragma once

#include <QtCore/QAbstractTableModel>
#include <QtCore/qglobal.h>

class MeasureModel;

struct TestedTrans {
    double CurrentINoLoad; //	mA

    double VoltageII1NoLoad; //	В
    double VoltageII2NoLoad; //	В
    double VoltageII3NoLoad; //	В
    double VoltageII4NoLoad; //	В

    double VoltageII1Load; //	В
    double VoltageII2Load; //	В
    double VoltageII3Load; //	В
    double VoltageII4Load; //	В
};

class TestedModel final : public QAbstractTableModel {
    Q_OBJECT

    QVector<TestedTrans> m_data;
    int lastLoad {};
    int lastNoLoad {};
    bool m_load {};

public:
    explicit TestedModel(QObject* parent = nullptr);
    ~TestedModel();

    // QAbstractItemModel interface
    int rowCount(const QModelIndex& = {}) const override { return m_data.size(); }
    int columnCount(const QModelIndex& = {}) const override { return ColumnCount; }

    QVariant data(const QModelIndex& index, int role) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

    void addFromMM(MeasureModel* model, bool load);
    void setCurrent(double val);
    void clear();
    int last() const;
    QString toString() const;

    enum {
        CurrentINoLoad, //	mA

        VoltageII1NoLoad, //	В
        VoltageII2NoLoad, //	В
        VoltageII3NoLoad, //	В
        VoltageII4NoLoad, //	В

        VoltageII1Load, //	В
        VoltageII2Load, //	В
        VoltageII3Load, //	В
        VoltageII4Load, //	В
        ColumnCount
    };
};
