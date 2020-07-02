#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(420, 390);
    ui->textBrowser->setEnabled(false);
    count_days();

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
    timer->start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showTime(void)
{
    std::time_t curr_time, off_time;
    std::tm *curr_tm, *off_tm = new std::tm{0};
    std::time(&curr_time);
    curr_tm = std::localtime(&curr_time);
    off_tm = curr_tm;
    off_tm->tm_sec = 0;
    off_tm->tm_min = OFF_MIN;
    off_tm->tm_hour = OFF_HOUR;
    off_time = std::mktime(off_tm);
    double time_diff = std::difftime(off_time, curr_time);
    if(time_diff<0){
        ui->timer->display(QString("00:00:00"));
        ui->textBrowser->setEnabled(true);
        ui->textBrowser->setText(QString("Remember to punch in!"));
    }else{
        int hour, min, sec;
        hour = time_diff/HOUR_SECONDS;
        time_diff -= hour*HOUR_SECONDS;
        min  = time_diff/MIN_SECONDS;
        time_diff -= min*MIN_SECONDS;
        sec = time_diff;
        QString time_str = QString::number(hour).rightJustified(2, '0')+":"+
                           QString::number(min).rightJustified(2, '0') +":"+
                           QString::number(sec).rightJustified(2, '0');
        ui->timer->display(time_str);
    }

}

void MainWindow::count_days(void)
{
    // Count day to next payday
    std::time_t curr_time, payday_time;
    std::tm *curr_tm;
    std::time(&curr_time);
    curr_tm = std::localtime(&curr_time);
    if(curr_tm->tm_hour>=17||(curr_tm->tm_hour==17&&curr_tm->tm_min>=30))
        remain_workdays = -1; // Already off
    else
        remain_workdays = 0;
    // Count using midnight
    curr_tm->tm_sec = 0;
    curr_tm->tm_min = 0;
    curr_tm->tm_hour = 0;
    int curr_day = curr_tm->tm_mday, curr_mon = curr_tm->tm_mon, curr_year = curr_tm->tm_year;
    int next_month = (curr_day<5?curr_mon:curr_mon+1), next_year = curr_year;
    // Prevent month overflow
    if(next_month==12){
        next_month-=12;
        next_year+=1;
    }
    std::tm *next_payday_tm = new std::tm{0}; // [next_year]/[next_mon]/5-00:00:00
    next_payday_tm->tm_mday = PAYDAY;
    next_payday_tm->tm_mon = next_month;
    next_payday_tm->tm_year = next_year;
    payday_time = std::mktime(next_payday_tm);
    double seconds_to_pay = std::difftime(payday_time, curr_time);
    days_to_pay = static_cast<int>(std::ceil(seconds_to_pay/DAY_SECONDS));
    ui->days_to_pay_LCD->display(days_to_pay);
    // Count remain workdays
    while((curr_tm->tm_mday!=PAYDAY)||(curr_tm->tm_mon!=next_month)||(curr_tm->tm_year!=next_year)){
        if(curr_tm->tm_wday!=0&&curr_tm->tm_wday!=6)
            ++remain_workdays;
        curr_time+=DAY_SECONDS;
        curr_tm = std::localtime(&curr_time);
    }
    ui->reamin_workdays_LCD->display(remain_workdays);
}
