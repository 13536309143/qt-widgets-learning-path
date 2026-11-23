#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include "verify.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    int x = 5;
    auto value = [x]() { return x + 1; };
    x = 100;
    writeLine("题一 f() = " + QString::number(value()) + ", 外部 x = " + QString::number(x));
    int y = 5;
    auto reference = [&y]() { ++y; };
    reference();
    writeLine("题二外部 y = " + QString::number(y));
    int z = 5;
    auto state = [z]() mutable { return ++z; };
    int first = state();
    int second = state();
    writeLine(QString("题三两次返回 %1、%2，外部 z = %3").arg(first).arg(second).arg(z));

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
