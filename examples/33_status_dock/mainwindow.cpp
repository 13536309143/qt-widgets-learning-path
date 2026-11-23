#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include "verify.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->statusbar->addWidget(new QLabel("普通状态", this));
    ui->statusbar->addPermanentWidget(new QLabel("UTF-8", this));
    ui->statusbar->showMessage("临时消息 3 秒后消失", 3000);

    // 仅带 --verify 参数时安排检查；正常学习时不会执行。
    scheduleVerification(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
