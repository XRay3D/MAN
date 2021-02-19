#ifndef MEASUREMODEL_H
#define MEASUREMODEL_H

#include "transmodel.h"
#include <QAbstractTableModel>

class MeasureModel : public QAbstractTableModel {
    Q_OBJECT
    friend class TestedModel;

    double m_data[3][4] {};
    bool m_load[4] {};

    double m_spread[2][4] {};
    double m_voltage[2][4] {};

signals:
    void setCurrent(double, int);
    void setEnabLoad(bool, int);
    void updateQCheckBox(bool[4]);

public:
    explicit MeasureModel(QObject* parent = nullptr);
    void setVoltages(const QVector<float>& val);
    void setCurrents(const QVector<float>& val);
    void setLoad(bool val);
    void setTrans(const Trans& trans);

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex& parent) const override;
    int columnCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
};

#endif // MEASUREMODEL_H
