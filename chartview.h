#pragma once

#include <QChartView>

enum { DataCount = 101 };

namespace QtCharts {
class QChart;
class QChartView;
class QLineSeries;
class QValueAxis;
}

class ChartView : public QtCharts::QChartView {
    Q_OBJECT

    // QtCharts
    QVector<QPointF> chartsData[4];
    QtCharts::QLineSeries* const lineSeries[4];
    QtCharts::QValueAxis* const axisX;
    QtCharts::QValueAxis* const axisY;
    QtCharts::QChart* const chart;

public:
    explicit ChartView(QWidget* parent = nullptr);
    void addToCharts(const QVector<float>& val);

signals:
};
