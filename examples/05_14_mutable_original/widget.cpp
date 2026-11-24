#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include "verify.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    int m = 10;
    auto fun = [m, this](int a, int b) mutable
    {
        writeLine("lambda is running!");
        m = 300;
        writeLine("内部副本 m = " + QString::number(m));
        return a + b;
    };
    int sum = fun(100, 200);
    writeLine("返回值 sum = " + QString::number(sum));
    writeLine("外部 m = " + QString::number(m));

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
