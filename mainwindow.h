#pragma once

#include <QMainWindow>
#include <QMutex>
#include <QSemaphore>

class TestedModel;
namespace Ui {
class MainWindow;
}

namespace QtCharts {
class QChart;
class QChartView;
class QLineSeries;
class QValueAxis;
}

class MeasureModel;
class TransModel;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

private:
    void writeSettings();
    void readSettings();

    void addToCharts(const QVector<float>& val);
    void setCurrents(const QVector<float>& val);
    void setCurrent(double val);
    int startMeasure();
    int stopMeasure();

signals:
    void getVoltage(const QVector<int>& = { 1, 2, 3, 4 });
    void getCurrentI();

private slots:
    void on_cbxTrans_currentIndexChanged(int index);
    void on_pbClear_clicked();
    void on_pbWrite_clicked();

private:
    Ui::MainWindow* ui;
    //    TRANS_DATABASE TrDataBase;
    QSemaphore semMan;
    QSemaphore semScpi;
    //QMutex m;
    int m_timerId = 0;

    TransModel* transModel;
    MeasureModel* measureModel;
    TestedModel* testedModel;

    // QtCharts
    QVector<QPointF> chartsData[4];
    QtCharts::QLineSeries* const lineSeries[4];
    QtCharts::QValueAxis* const axisX;
    QtCharts::QValueAxis* const axisY;
    QtCharts::QChart* const chart;
    QtCharts::QChartView* const chartView;
    // QObject interface
protected:
    void timerEvent(QTimerEvent* event) override;
};
