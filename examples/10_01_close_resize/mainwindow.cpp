#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QCloseEvent>
#include "verify.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->statusbar->showMessage("拖动边缘改变尺寸；关闭窗口会询问确认。");

    // 仅带 --verify 参数时安排检查；正常学习时不会执行。
    scheduleVerification(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    auto choice = QMessageBox::question(this, "关闭确认", "确定关闭窗口吗？",
                                        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (choice == QMessageBox::Yes)
        event->accept();
    else
        event->ignore();
}
