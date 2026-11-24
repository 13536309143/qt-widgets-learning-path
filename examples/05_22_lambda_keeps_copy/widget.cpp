#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include "verify.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    int count = 0;
    connect(ui->runButton, &QPushButton::clicked, this,
            [this, count]() mutable
            {
                ++count;
                writeLine("Lambda 内部 count = " + QString::number(count));
            });
    writeLine("外部局部 count = " + QString::number(count));
    writeLine("连接保存的是副本，不引用这个局部 count。");

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
