#include "widget.h"
#include <QPushButton>
#include <QPlainTextEdit>
#include <QLabel>
#include <QDebug>
#include <QString>

// 本步重点：每个按钮的 Lambda 按值保存自己创建时的 i。
Widget::Widget(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("25 循环中保存按钮编号");
    resize(720, 540);
    auto *heading = new QLabel("25 循环中保存按钮编号", this);
    heading->setGeometry(24, 18, 670, 32);
    QFont font = heading->font();
    font.setPointSize(15);
    font.setBold(true);
    heading->setFont(font);
    output_ = new QPlainTextEdit(this);
    output_->setReadOnly(true);
    output_->setGeometry(24, 140, 670, 370);
    output_->setStyleSheet("font-size: 15px;");
    for (int i = 0; i < 3; ++i)
    {
        auto *button = new QPushButton(QString("编号 %1").arg(i), this);
        button->setObjectName(QString("number%1").arg(i));
        button->setGeometry(24 + i * 180, 65, 160, 40);
        connect(button, &QPushButton::clicked, this,
                [this, i]() { writeLine("点击的编号 = " + QString::number(i)); });
    }
}

// 小辅助函数：同时显示在窗口和 Qt Creator 的应用程序输出中。
void Widget::writeLine(const QString &text)
{
    if (output_)
        output_->appendPlainText(text);
    qDebug().noquote() << text;
}
void Widget::refreshTitle()
{
    setWindowTitle(QString("已点击 %1 次").arg(count_));
}
void Widget::increase()
{
    ++count_;
    refreshTitle();
    writeLine("加一后 count_ = " + QString::number(count_));
}
