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
    int total = 0;
    auto accumulate = [base, &total](int x) { total += base + x; };
    base = 100;
    accumulate(3);
    writeLine("第一次 total = " + QString::number(total));
    accumulate(8);
    writeLine("第二次 total = " + QString::number(total));
    writeLine("外部 base = " + QString::number(base));

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
