#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QPushButton>
#include "verify.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    {
        QWidget parent;
        QPushButton child("栈按钮", &parent);
        connect(&child, &QObject::destroyed, this,
                [this]() { writeLine("1 子按钮先析构，从父对象列表移除。"); });
        connect(&parent, &QObject::destroyed, this,
                [this]() { writeLine("2 父窗口后析构，不会重复删除子按钮。"); });
    }
    writeLine("3 已安全离开局部作用域。");

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
