#include "trans_database.h"
#include "ui_trans_database.h"

TRANS_DATABASE::TRANS_DATABASE(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::TRANS_DATABASE)
    , dbase(QSqlDatabase::addDatabase("QSQLITE"))

{
    ui->setupUi(this);
    table = ui->tableView;

    table->addAction(ui->insertRowAction);
    table->addAction(ui->deleteRowAction);
    table->addAction(ui->fieldStrategyAction);
    table->addAction(ui->rowStrategyAction);
    table->addAction(ui->manualStrategyAction);
    table->addAction(ui->submitAction);
    table->addAction(ui->revertAction);
    table->addAction(ui->selectAction);

    dbase.setDatabaseName("trans.db");

    if (!dbase.open()) {
        qDebug() << "Что-то пошло не так!";
        return;
    }

    model = new QSqlTableModel(this, dbase);

    QSqlQuery a_query;
    // DDL query
    QString str = "CREATE TABLE trans ("
                  "trId VARCHAR(255) PRIMARY KEY NOT NULL, "
                  "numberForm INTEGER, "
                  "numberOfChannels VARCHAR(255), "
                  "ratedVoltage FLOAT, "
                  "ratedVoltageAdd10P FLOAT, "
                  "inRatedVoltageSub15P FLOAT, "
                  "idlingCurrentAtRatedVoltageAdd10P FLOAT, "
                  "ratedLoadCurrent1 FLOAT, "
                  "ratedLoadCurrent2 FLOAT, "
                  "ratedLoadCurrent3 FLOAT, "
                  "ratedLoadCurrent4 FLOAT, "
                  "outVoltageAtRatedVoltageAdd10P1 VARCHAR(255), "
                  "outVoltageAtRatedVoltageAdd10P2 VARCHAR(255), "
                  "outVoltageAtRatedVoltageAdd10P3 VARCHAR(255), "
                  "outVoltageAtRatedVoltageAdd10P4 VARCHAR(255), "
                  "outVoltageAtLoadAtRatedVoltageSub15P1 VARCHAR(255), "
                  "outVoltageAtLoadAtRatedVoltageSub15P2 VARCHAR(255), "
                  "outVoltageAtLoadAtRatedVoltageSub15P3 VARCHAR(255), "
                  "outVoltageAtLoadAtRatedVoltageSub15P4 VARCHAR(255)"
                  ");";

    bool b = a_query.exec(str);
    if (!b) {
        qDebug() << "Вроде не удается создать таблицу, провертье карманы!";
    }

    // DML
    QString str_insert = "INSERT INTO trans("
                         "trId, "
                         "numberForm, "
                         "numberOfChannels, "
                         "ratedVoltage, "
                         "ratedVoltageAdd10P, "
                         "inRatedVoltageSub15P, "
                         "idlingCurrentAtRatedVoltageAdd10P, "
                         "RatedloadCurrent1, "
                         "RatedloadCurrent2, "
                         "RatedloadCurrent3, "
                         "RatedloadCurrent4, "
                         "outVoltageAtRatedVoltageAdd10P1, "
                         "outVoltageAtRatedVoltageAdd10P2, "
                         "outVoltageAtRatedVoltageAdd10P3, "
                         "outVoltageAtRatedVoltageAdd10P4, "
                         "outVoltageAtLoadAtRatedVoltageSub15P1, "
                         "outVoltageAtLoadAtRatedVoltageSub15P2, "
                         "outVoltageAtLoadAtRatedVoltageSub15P3, "
                         "outVoltageAtLoadAtRatedVoltageSub15P4"
                         ") "
                         "VALUES ('%1', %2, '%3', %4, %5, %6, %7, %8, %9, %10, %11, '%12', '%13', '%14', '%15', '%16', '%17', '%18', '%19');";
    str = str_insert
              .arg("ТП25(2x110/4x31)") //  "trId, "
              .arg(2) //  "numberForm, "
              .arg("1,2,3,4") //  "numberOfChannels, "
              .arg(220) //  "ratedVoltage, "
              .arg(242) //  "ratedVoltageAdd10P, "
              .arg(187) //  "inRatedVoltageSub15P, "
              .arg(0.05) //  "idlingCurrentAtRatedVoltageAdd10P, "
              .arg(0.13) //  "RatedloadCurrent1, "
              .arg(0.13) //  "RatedloadCurrent2, "
              .arg(0.13) //  "RatedloadCurrent3, "
              .arg(0.13) //  "RatedloadCurrent4, "
              .arg("40±1,2") //  "outVoltageAtRatedVoltageAdd10P1, "
              .arg("40±1,2") //  "outVoltageAtRatedVoltageAdd10P2, "
              .arg("40±1,2") //  "outVoltageAtRatedVoltageAdd10P3, "
              .arg("40±1,2") //  "outVoltageAtRatedVoltageAdd10P4, "
              .arg("24,3±0,83") //  "outVoltageAtLoadAtRatedVoltageSub15P1, "
              .arg("24,3±0,83") //  "outVoltageAtLoadAtRatedVoltageSub15P2, "
              .arg("24,3±0,83") //  "outVoltageAtLoadAtRatedVoltageSub15P3, "
              .arg("24,3±0,83"); // "outVoltageAtLoadAtRatedVoltageSub15P4"

    b = a_query.exec(str);
    if (!b) {
        qDebug() << "Кажется данные не вставляются, проверьте дверь, может она закрыта?";
    }
    //.....
    if (!a_query.exec("SELECT * FROM trans")) {
        qDebug() << "Даже селект не получается, я пас.";
        //        return;
    }
    QSqlRecord rec = a_query.record();
    int number = 0, age = 0;
    QString address = "";

    while (a_query.next()) {
        number = a_query.value(rec.indexOf("numberForm")).toInt();
        age = a_query.value(rec.indexOf("ratedVoltage")).toInt();
        address = a_query.value(rec.indexOf("trId")).toString();

        qDebug() << "number is " << number
                 << ". age is " << age
                 << ". address" << address;
    }

    model->setTable("trans");
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    model->select();

    QStringList headerList = QStringList() << "Тип\nтрансформатора"
                                           << "Номер\nформы"
                                           << "Номера\nканалов"
                                           << "Номинальное\nнапряжение\n(Uном), В"
                                           << "Номинальное\nнапряжение\n+10%(Uном+10%), В"
                                           << "Номинальное\nнапряжение\n-15%(Uном-15%), В"
                                           << "Ток при\nUном+15%, А"
                                           << "Ток\nнагрузки\nобмотки 1\n(Iнаг1), А"
                                           << "Ток\nнагрузки\nобмотки 2\n(Iнаг2), А"
                                           << "Ток\nнагрузки\nобмотки 3\n(Iнаг3), А"
                                           << "Ток\nнагрузки\nобмотки 4\n(Iнаг4), А"
                                           << "Напряжение\nобмотки 1\nпри\nUном+10%, В"
                                           << "Напряжение\nобмотки 2\nпри\nUном+10%, В"
                                           << "Напряжение\nобмотки 3\nпри\nUном+10%, В"
                                           << "Напряжение\nобмотки 4\nпри\nUном+10%, В"
                                           << "Напряжение\nобмотки 1\nпри\nUном-15%\nи Iнаг1, В"
                                           << "Напряжение\nобмотки 2\nпри\nUном-15%\nи Iнаг2, В"
                                           << "Напряжение\nобмотки 3\nпри\nUном-15%\nи Iнаг3, В"
                                           << "Напряжение\nобмотки 4\nпри\nUном-15%\nи Iнаг4, В";

    for (int col = 0; col < headerList.size(); ++col) {
        model->setHeaderData(col, Qt::Horizontal, headerList[col]);
    }

    table->setModel(model);
    table->resizeRowsToContents();
    table->resizeColumnsToContents();

    connect(table->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this, SLOT(updateActions()));
    updateActions();
}

TRANS_DATABASE::~TRANS_DATABASE()
{
    dbase.close();
    delete ui;
}

void TRANS_DATABASE::insertRow()
{
    QSqlTableModel* model = qobject_cast<QSqlTableModel*>(table->model());
    if (!model)
        return;

    QModelIndex insertIndex = table->currentIndex();
    int row = insertIndex.row() == -1 ? 0 : insertIndex.row();
    model->insertRow(row);
    insertIndex = model->index(row, 0);
    table->setCurrentIndex(insertIndex);
    table->edit(insertIndex);
}

void TRANS_DATABASE::deleteRow()
{
    QSqlTableModel* model = qobject_cast<QSqlTableModel*>(table->model());
    if (!model)
        return;

    QModelIndexList currentSelection = table->selectionModel()->selectedIndexes();
    for (int i = 0; i < currentSelection.count(); ++i) {
        if (currentSelection.at(i).column() != 0)
            continue;
        model->removeRow(currentSelection.at(i).row());
    }

    updateActions();
}

void TRANS_DATABASE::on_fieldStrategyAction_triggered()
{
    QSqlTableModel* tm = qobject_cast<QSqlTableModel*>(table->model());
    if (tm)
        tm->setEditStrategy(QSqlTableModel::OnFieldChange);
}

void TRANS_DATABASE::on_rowStrategyAction_triggered()
{
    QSqlTableModel* tm = qobject_cast<QSqlTableModel*>(table->model());
    if (tm)
        tm->setEditStrategy(QSqlTableModel::OnRowChange);
}

void TRANS_DATABASE::on_manualStrategyAction_triggered()
{
    QSqlTableModel* tm = qobject_cast<QSqlTableModel*>(table->model());
    if (tm)
        tm->setEditStrategy(QSqlTableModel::OnManualSubmit);
}

void TRANS_DATABASE::on_submitAction_triggered()
{
    QSqlTableModel* tm = qobject_cast<QSqlTableModel*>(table->model());
    if (tm)
        tm->submitAll();
}

void TRANS_DATABASE::on_revertAction_triggered()
{
    QSqlTableModel* tm = qobject_cast<QSqlTableModel*>(table->model());
    if (tm)
        tm->revertAll();
}

void TRANS_DATABASE::on_selectAction_triggered()
{
    QSqlTableModel* tm = qobject_cast<QSqlTableModel*>(table->model());
    if (tm)
        tm->select();
}

void TRANS_DATABASE::updateActions()
{
    QSqlTableModel* tm = qobject_cast<QSqlTableModel*>(table->model());
    bool enableIns = tm;
    bool enableDel = enableIns && table->currentIndex().isValid();

    ui->insertRowAction->setEnabled(enableIns);
    ui->deleteRowAction->setEnabled(enableDel);

    ui->fieldStrategyAction->setEnabled(tm);
    ui->rowStrategyAction->setEnabled(tm);
    ui->manualStrategyAction->setEnabled(tm);
    ui->submitAction->setEnabled(tm);
    ui->revertAction->setEnabled(tm);
    ui->selectAction->setEnabled(tm);

    qDebug() << "tm" << tm;
    if (tm) {
        QSqlTableModel::EditStrategy es = tm->editStrategy();
        ui->fieldStrategyAction->setChecked(es == QSqlTableModel::OnFieldChange);
        ui->rowStrategyAction->setChecked(es == QSqlTableModel::OnRowChange);
        ui->manualStrategyAction->setChecked(es == QSqlTableModel::OnManualSubmit);
    }
}

void TRANS_DATABASE::showEvent(QShowEvent*)
{
}
