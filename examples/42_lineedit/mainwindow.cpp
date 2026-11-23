#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "verify.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->name->setTextMargins(8, 0, 8, 0);
    connect(ui->name, &QLineEdit::textChanged, this, [this](const QString &text)
            { ui->resultLabel->setText(QString("内容 %1，长度 %2").arg(text).arg(text.size())); });

    // 仅带 --verify 参数时安排检查；正常学习时不会执行。
    scheduleVerification(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
