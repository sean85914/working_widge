#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <cstring>
#include <ctime>
#include <cmath>
#include <QMainWindow>
#include <QTimer>

const int PAYDAY = 5;
const int DAY_SECONDS = 60*60*24; // Total seconds in a day
const int HOUR_SECONDS = 60*60;
const int MIN_SECONDS = 60;
const int OFF_HOUR = 17;
const int OFF_MIN = 30;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    int days_to_pay, remain_workdays;
    void count_days(void);
private slots:
    void showTime(void);
};

#endif // MAINWINDOW_H
