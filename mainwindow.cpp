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

{
    ui->setupUi(this);

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

    // Tested Model
    ui->tvTested->setModel(testedModel = new TestedModel(ui->tvTested));
    ui->tvTested->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tvTested->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

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
        bool ok = true;
        for (int address : { 1, 2, 3, 4 })
            ok &= MI::man()->ping(ui->cbxPortMan->currentText(), 19200, address);
        ui->label_5->setStyleSheet(ok ?
                                      R"(QFrame, QLabel, QToolTip {
border: 2px solid green;
border-radius: 4px;
padding: 2px;
})"
                                      :
                                      R"(QFrame, QLabel, QToolTip {
border: 2px solid red;
border-radius: 4px;
padding: 2px;
})");
        ok &= MI::scpi()->ping(ui->cbxPortI->currentText());
        ui->label_6->setStyleSheet(ok ?
                                      R"(QFrame, QLabel, QToolTip {
border: 2px solid green;
border-radius: 4px;
padding: 2px;
})"
                                      :
                                      R"(QFrame, QLabel, QToolTip {
border: 2px solid red;
border-radius: 4px;
padding: 2px;
})");
    });

    connect(ui->pbMeasure, &QPushButton::clicked, [&](bool checked) {
        if (m_timerId)
            killTimer(m_timerId);
        m_timerId = checked ? startMeasure()
                            : stopMeasure();
        ui->pbMeasure->setText(checked ? "Остановить измерение" : "Запустить измерение");
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
    settings.setValue("splitter", ui->splitter->saveState());
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
    ui->splitter->restoreState(settings.value("splitter").toByteArray());
    settings.endGroup();
}

void MainWindow::addToCharts(const QVector<float>& val)
{
    ui->chartView->addToCharts(val);
    semMan.acquire(semMan.available());
}

void MainWindow::setCurrent(double val)
{
    ui->dsbxCurrentIn->setValue(val);
    semScpi.acquire(semScpi.available());
}

int MainWindow::startMeasure()
{
    semMan.release();
    semScpi.release();
    emit getCurrentI();
    emit getVoltage();
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
        emit getVoltage();
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
    if (QMessageBox::question(this, "", "", QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes)
        testedModel->clear();
}

void MainWindow::on_pbWrite_clicked()
{
    testedModel->addFromMM(measureModel, ui->chbxSetPointLoad->isChecked());
    testedModel->setCurrent(ui->dsbxCurrentIn->value());
    ui->tvTested->selectRow(testedModel->last());
}

void MainWindow::on_pbTransList_clicked()
{
    TransDialog d(ui->cbxTrans->model());
    connect(&d, &QDialog::accepted, [] { exit(0); });
    d.exec();
}

void MainWindow::on_pbCopy_clicked()
{
    QClipboard* clipboard = QGuiApplication::clipboard();
    QString originalText = clipboard->text();
    clipboard->setText(testedModel->toString().replace('.', ','));
}
