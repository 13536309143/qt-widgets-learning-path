#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include "verify.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    int base = 10;
    auto addBase = [base](int x) { return base + x; };
    writeLine("base 的副本 = 10");
    writeLine("addBase(3) = " + QString::number(addBase(3)));
    writeLine("addBase(8) = " + QString::number(addBase(8)));

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
