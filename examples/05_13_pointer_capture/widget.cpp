#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include "verify.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    int number = 10;
    int *ptr = &number;
    auto change = [ptr]() { *ptr = 30; };
    change();
    writeLine("number = " + QString::number(number));
    writeLine("[ptr] 没有复制 number，只复制了地址。");

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
