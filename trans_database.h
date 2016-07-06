#ifndef TRANS_DATABASE_H
#define TRANS_DATABASE_H

#include <QDialog>
#include <QtSql>

namespace Ui {
class TRANS_DATABASE;
}

class TRANS_DATABASE : public QDialog {
    Q_OBJECT

public:
    explicit TRANS_DATABASE(QWidget* parent = 0);
    ~TRANS_DATABASE();

private:
    Ui::TRANS_DATABASE* ui;

    QSqlTableModel *model;
    QSqlDatabase dbase;

    // QWidget interface
protected:
    void showEvent(QShowEvent *);
};

#endif // TRANS_DATABASE_H
