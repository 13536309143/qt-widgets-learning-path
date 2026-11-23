#include "widget.h"
#include <QPushButton>
#include <QPlainTextEdit>
#include <QLabel>
#include <QDebug>
#include <QString>

// 本步重点：把计数直接保存到 mutable Lambda 的副本里。
Widget::Widget(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("27 值捕获也能保存回调自身状态");
    resize(720, 540);
    auto *heading = new QLabel("27 值捕获也能保存回调自身状态", this);
    heading->setGeometry(24, 18, 670, 32);
    QFont font = heading->font();
    font.setPointSize(15);
    font.setBold(true);
    heading->setFont(font);
    output_ = new QPlainTextEdit(this);
    output_->setReadOnly(true);
    output_->setGeometry(24, 140, 670, 370);
    output_->setStyleSheet("font-size: 15px;");
    auto *run = new QPushButton("回调内部计数", this);
    run->setObjectName("runButton");
    run->setGeometry(24, 65, 250, 40);
    int count = 0;
    connect(run, &QPushButton::clicked, this,
            [this, count]() mutable
            {
                ++count;
                writeLine("Lambda 内部 count = " + QString::number(count));
            });
    writeLine("外部局部 count = " + QString::number(count));
    writeLine("连接保存的是副本，不引用这个局部 count。");
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
