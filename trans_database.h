#pragma once

#include "ui_trans_database.h"
#include <QDialog>
#include <QtSql>
#include <QtWidgets>

namespace Ui {
class TRANS_DATABASE;
}

class TRANS_DATABASE : public QDialog {
    Q_OBJECT

public:
    explicit TRANS_DATABASE(QWidget* parent = 0);
    ~TRANS_DATABASE();
private slots:
    void updateActions();
    void insertRow();
    void deleteRow();
    void on_insertRowAction_triggered();
    void on_deleteRowAction_triggered();
    void on_fieldStrategyAction_triggered();
    void on_rowStrategyAction_triggered();
    void on_manualStrategyAction_triggered();
    void on_submitAction_triggered();
    void on_revertAction_triggered();
    void on_selectAction_triggered();

private:
    Ui::TRANS_DATABASE* ui;

    QTableView* table;

    QSqlTableModel* model;
    QSqlDatabase dbase;

    // QWidget interface
protected:
    void showEvent(QShowEvent*);
};
