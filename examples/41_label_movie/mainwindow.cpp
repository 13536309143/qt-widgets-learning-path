#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMovie>
#include "verify.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->imageLabel->setPixmap(QPixmap(":/images/demo.png").scaledToWidth(360));
    auto *movie = new QMovie(":/images/demo.gif", QByteArray(), this);
    ui->animationLabel->setMovie(movie);
    movie->start();
    connect(ui->run, &QPushButton::clicked, movie, &QMovie::stop);

    // 仅带 --verify 参数时安排检查；正常学习时不会执行。
    scheduleVerification(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
