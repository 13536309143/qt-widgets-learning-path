#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QIcon>
#include "verify.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPixmap picture(":/images/demo.png");
    if (picture.isNull())
        ui->imageLabel->setText("图片加载失败，请检查 qrc 资源路径。");
    else
        ui->imageLabel->setPixmap(
            picture.scaled(600, 360, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    setWindowIcon(QIcon(":/images/demo.png"));

    // 仅带 --verify 参数时安排检查；正常学习时不会执行。
    scheduleVerification(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
