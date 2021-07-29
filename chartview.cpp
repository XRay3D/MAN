#include "chartview.h"

#include <QLineSeries>
#include <QValueAxis>

QT_CHARTS_USE_NAMESPACE

ChartView::ChartView(QWidget* parent)
    : QChartView(parent)
    , lineSeries { new QLineSeries, new QLineSeries, new QLineSeries, new QLineSeries }
    , axisX { new QValueAxis }
    , axisY { new QValueAxis }
    , chart { new QChart }
{
    setChart(chart);
    // QChart
    for (auto& cd : chartsData)
        cd.reserve(DataCount);

    axisX->setLabelFormat("%g");
    axisY->setTitleText("Напряжение, В.");
    chart->addAxis(axisX, Qt::AlignBottom);

    for (auto ls : lineSeries) {
        chart->addSeries(ls);
        ls->attachAxis(axisX);
        chart->addAxis(axisY, Qt::AlignLeft);
        ls->attachAxis(axisY);
    }
    setRenderHint(QPainter::Antialiasing);

    chart->legend()->hide();
    chart->setMargins({});
}

void ChartView::addToCharts(const QVector<float>& val)
{
    if (val.size() == 4) {
        static uint ctr;
        double min = +std::numeric_limits<double>::max(),
               max = -std::numeric_limits<double>::max();
        for (int i = 0; i < 4; ++i) {
            DataCount > ctr ? (chartsData[i].append(QPointF { static_cast<qreal>(ctr % DataCount), val[i] }), QPointF {})
                            : chartsData[i][ctr % DataCount] = QPointF { static_cast<qreal>(ctr % DataCount), val[i] };
            lineSeries[i]->replace(chartsData[i]);
            auto [min_, max_] = std::ranges::minmax(chartsData[i], [](QPointF p1, QPointF p2) { return p1.y() < p2.y(); });
            min = std::min(min, min_.y());
            max = std::max(max, max_.y());
        }
        axisX->setRange(0, DataCount < ctr ? DataCount - 1 : ctr - 1);
        axisY->setRange(min, max);
        ++ctr;
    }
}
