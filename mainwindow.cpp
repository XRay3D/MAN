#include "mainwindow.h"
#include "hwi/interface.h"
#include "measuremodel.h"
#include "testedmodel.h"
#include "transmodel.h"
#include "ui_mainwindow.h"

#include <QClipboard>
#include <QTimer>
#include <QtCharts>
#include <ranges>

const int fvId = qRegisterMetaType<QVector<float>>("QVector<float>");

using namespace QtCharts;

enum { DataCount = 101 };

class PopupItemDelegate : public QStyledItemDelegate {
public:
    using QStyledItemDelegate::QStyledItemDelegate;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override
    {
        return QStyledItemDelegate::sizeHint(option, index) + QSize(0, 10);
    }
};

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , lineSeries { new QLineSeries, new QLineSeries, new QLineSeries, new QLineSeries }
    , axisX { new QValueAxis }
    , axisY { new QValueAxis }
    , chart { new QChart }
    , chartView { new QChartView(chart) }
{
    ui->setupUi(this);

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
    chartView->setRenderHint(QPainter::Antialiasing);

    chart->legend()->hide();
    chart->setMargins({});
    //m_chart->setTitle("Data from the microphone (" + QString("deviceInfo.deviceName()") + ')');
    ui->vlayMeasure->addWidget(chartView);

    // Available Ports
    auto availablePorts { QSerialPortInfo::availablePorts() };
    std::sort(availablePorts.begin(), availablePorts.end(), [](auto& pil, auto& pir) {
        return pil.portName().mid(3).toInt() < pir.portName().mid(3).toInt();
    });
    for (auto& info : availablePorts) {
        ui->cbxPortI->addItem(info.portName());
        ui->cbxPortMan->addItem(info.portName());
    }

    // Trans Model
    ui->cbxTrans->setModel(transModel = new TransModel(ui->cbxTrans));
    ui->cbxTrans->view()->setItemDelegate(new PopupItemDelegate(ui->cbxTrans));
    ui->cbxTrans->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->cbxTrans, &QTableView::customContextMenuRequested, [this](const QPoint& pos) {
        QMenu menu;
        menu.addAction("Редактировать Обмотки", [this] {
            TransDialog d(ui->cbxTrans->model());
            connect(&d, &QDialog::accepted, [] { exit(0); });
            d.exec(); });
        menu.exec(ui->cbxTrans->mapToGlobal(pos));
    });

    // Tested Model
    ui->tvTested->setModel(testedModel = new TestedModel(ui->tvTested));
    ui->tvTested->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tvTested->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tvTested->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tvTested, &QTableView::customContextMenuRequested, [this](const QPoint& pos) {
        QMenu menu;
        menu.addAction("Копировать данные", [this] {
            QClipboard* clipboard = QGuiApplication::clipboard();
            QString originalText = clipboard->text();
            clipboard->setText(testedModel->toString().replace('.', ','));
        });
        menu.exec(ui->tvTested->viewport()->mapToGlobal(pos));
    });
    // Measure Model
    ui->tvMeasured->setModel(measureModel = new MeasureModel(ui->tvMeasured));
    ui->tvMeasured->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tvMeasured->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tvMeasured->verticalHeader()->setFont(font());
    ui->tvMeasured->horizontalHeader()->setFont(font());
    ui->tvMeasured->setMaximumHeight(100);
    ui->tvMeasured->setMinimumHeight(100);
    ui->tvMeasured->setFont(font());

    // Check Box for Set Load
    auto setupUi = [this](QWidget* Form) {
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        //        sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());

        auto frame = new QFrame(Form);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setSizePolicy(sizePolicy);
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);

        auto checkBox = ui->chbxSetPointLoad;
        sizePolicy.setHeightForWidth(checkBox->sizePolicy().hasHeightForWidth());
        checkBox->setSizePolicy(sizePolicy);
        checkBox->setTristate(false);

        auto verticalLayout = new QVBoxLayout(Form);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(6, 0, 0, 2);
        verticalLayout->addWidget(frame);
        verticalLayout->addWidget(checkBox);

        connect(checkBox, &QCheckBox::clicked, measureModel, &MeasureModel::setLoad, Qt::QueuedConnection);
        connect(checkBox, &QCheckBox::clicked, [checkBox]() {
            checkBox->checkState() == Qt::PartiallyChecked ? checkBox->setCheckState(Qt::Checked)
                                                           : void();
        });

        connect(measureModel, &MeasureModel::updateQCheckBox, [checkBox](bool val[4]) {
            int ctr {};
            for (int i = 0; i < 4; ++i)
                ctr += val[i] ? 1 : 0;
            checkBox->setCheckState(ctr == 4 ? Qt::Checked : (ctr == 0 ? Qt::Unchecked : Qt::PartiallyChecked));
        });
    };
    setupUi(ui->tvMeasured->verticalHeader());

    connect(ui->pbCheckConnection, &QPushButton::clicked, [&]() {
        qDebug() << MI::man()->ping(ui->cbxPortMan->currentText());
        qDebug() << MI::scpi()->ping(ui->cbxPortI->currentText());
    });

    connect(ui->pbMeasure, &QPushButton::clicked, [&](bool checked) {
        if (m_timerId)
            killTimer(m_timerId);
        m_timerId = checked ? startMeasure()
                            : stopMeasure();
    });

    // MI connect
    connect(MI::man(), &Man::Voltage, measureModel, &MeasureModel::setVoltages);
    connect(MI::man(), &Man::Voltage, this, &MainWindow::addToCharts);
    connect(measureModel, &MeasureModel::setCurrent, MI::man(), &Man::setCurrent, Qt::DirectConnection);
    connect(measureModel, &MeasureModel::setEnabLoad, MI::man(), &Man::setEnabledCurrent, Qt::DirectConnection);
    connect(this, &MainWindow::getVoltage, MI::man(), &Man::getVoltage, Qt::QueuedConnection);

    connect(this, &MainWindow::getCurrentI, MI::scpi(), &SCPI::getAcCurrent, Qt::QueuedConnection);
    connect(MI::scpi(), &SCPI::measureReady, this, &MainWindow::setCurrent);

    readSettings();
    QTimer::singleShot(100, [this] { ui->pbCheckConnection->clicked(); });
}

MainWindow::~MainWindow()
{
    if (m_timerId)
        killTimer(m_timerId);
    MI::man()->disconnect();
    writeSettings();
    delete ui;
}

void MainWindow::writeSettings()
{
    QSettings settings;
    settings.beginGroup("MainWindow");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("state", saveState());
    settings.setValue("cbxPortMan", ui->cbxPortMan->currentText());
    settings.setValue("cbxPortI", ui->cbxPortI->currentText());
    settings.setValue("cbxTrans", ui->cbxTrans->currentIndex());
    settings.endGroup();
}

void MainWindow::readSettings()
{
    QSettings settings;
    settings.beginGroup("MainWindow");
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("state").toByteArray());
    ui->cbxPortMan->setCurrentText(settings.value("cbxPortMan").toString());
    ui->cbxPortI->setCurrentText(settings.value("cbxPortI").toString());
    ui->cbxTrans->setCurrentIndex(settings.value("cbxTrans").toInt());
    settings.endGroup();
}

void MainWindow::addToCharts(const QVector<float>& val)
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
    semMan.acquire(semMan.available());
    //m.unlock();
}

void MainWindow::setCurrent(double val)
{
    qDebug() << __FUNCTION__;
    ui->dsbxCurrentIn->setValue(val);
    semScpi.acquire(semScpi.available());
}

int MainWindow::startMeasure()
{
    semMan.release();
    emit getVoltage();
    semScpi.release();
    emit getCurrentI();
    return startTimer(10);
}

int MainWindow::stopMeasure()
{
    semMan.acquire(semMan.available());
    semScpi.acquire(semScpi.available());
    return 0;
}

void MainWindow::setCurrents([[maybe_unused]] const QVector<float>& val)
{
}

void MainWindow::timerEvent(QTimerEvent* event)
{
    if (event->timerId() == m_timerId && /* m.tryLock()*/ !semMan.available()) {
        semMan.release();
        emit getVoltage({ 1 });
    }
    if (event->timerId() == m_timerId && /* m.tryLock()*/ !semScpi.available()) {
        semScpi.release();
        emit getCurrentI();
    }
}

void MainWindow::on_cbxTrans_currentIndexChanged(int index)
{
    if (index < 0 || !measureModel || !transModel)
        return;
    auto& trans = transModel->trans(index);
    measureModel->setTrans(trans);
    ui->dsbxULoad->setValue(trans.PrimaryCoils.front().LoadVoltage);
    ui->dsbxUNoLoad->setValue(trans.PrimaryCoils.front().NoLoadVoltage);
}

void MainWindow::on_pbClear_clicked()
{
    testedModel->clear();
}

void MainWindow::on_pbWrite_clicked()
{
    testedModel->addFromMM(measureModel, ui->chbxSetPointLoad->isChecked());
    testedModel->setCurrent(ui->dsbxCurrentIn->value());
    ui->tvTested->selectRow(testedModel->last());
}
