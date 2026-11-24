#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include "verify.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    int n = 10;
    auto increase = [&n]() { ++n; };
    increase();
    writeLine("第一次调用后 n = " + QString::number(n));
    n = 40;
    increase();
    writeLine("第二次调用后 n = " + QString::number(n));

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
