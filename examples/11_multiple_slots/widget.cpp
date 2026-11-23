#include "widget.h"
#include <QPushButton>
#include <QPlainTextEdit>
#include <QLabel>
#include <QDebug>
#include <QString>

// 本步重点：观察同线程直接连接按连接顺序调用。
Widget::Widget(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("11 一个信号连接多个处理函数");
    resize(720, 540);
    auto *heading = new QLabel("11 一个信号连接多个处理函数", this);
    heading->setGeometry(24, 18, 670, 32);
    QFont font = heading->font();
    font.setPointSize(15);
    font.setBold(true);
    heading->setFont(font);
    output_ = new QPlainTextEdit(this);
    output_->setReadOnly(true);
    output_->setGeometry(24, 140, 670, 370);
    output_->setStyleSheet("font-size: 15px;");
    auto *run = new QPushButton("依次执行两个处理函数", this);
    run->setObjectName("runButton");
    run->setGeometry(24, 65, 250, 40);
    connect(run, &QPushButton::clicked, this, [this]() { writeLine("1 第一个连接"); });
    connect(run, &QPushButton::clicked, this, [this]() { writeLine("2 第二个连接"); });
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
