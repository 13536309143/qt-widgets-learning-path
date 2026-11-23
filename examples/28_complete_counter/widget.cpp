#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include "verify.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(ui->runButton, &QPushButton::clicked, this,
            [this]()
            {
                ++count_;
                refreshTitle();
                writeLine("加一后 count_ = " + QString::number(count_));
            });
    connect(ui->resetButton, &QPushButton::clicked, this,
            [this]()
            {
                count_ = 0;
                refreshTitle();
                writeLine("清零后 count_ = 0");
            });
    refreshTitle();

    // 仅带 --verify 参数时安排检查；正常学习时不会执行。
    scheduleVerification(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::writeLine(const QString &text)
{
    ui->outputEdit->appendPlainText(text);
    qDebug().noquote() << text;
}

void Widget::refreshTitle()
{
    setWindowTitle(QString("已点击 %1 次").arg(count_));
}
