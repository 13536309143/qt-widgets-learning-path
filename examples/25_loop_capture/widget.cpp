#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QList>
#include "verify.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    const QList<QPushButton *> buttons = {ui->number0, ui->number1, ui->number2};
    for (int i = 0; i < buttons.size(); ++i)
    {
        auto *button = buttons.at(i);
        connect(button, &QPushButton::clicked, this,
                [this, i]() { writeLine("点击的编号 = " + QString::number(i)); });
    }

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
