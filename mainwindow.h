#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "man.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

private:
    void writeSettings();
    void readSettings();

private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
    void on_pushButton_3_clicked();
    void on_comboBox_3_currentIndexChanged(int index);
    void on_checkBox_clicked(bool checked);

    void on_pushButtonLoad_clicked();
    void on_pushButtonLoad_clicked(bool checked);
    void on_pushButtonPing_clicked();
    void on_pushButtonWrite_clicked();

private:
    Ui::MainWindow* ui;

    MAN man;
};

#endif // MAINWINDOW_H
