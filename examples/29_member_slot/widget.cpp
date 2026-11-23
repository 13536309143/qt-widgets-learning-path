#include "widget.h"
#include <QPushButton>
#include <QPlainTextEdit>
#include <QLabel>
#include <QDebug>
#include <QString>

// 本步重点：比较成员函数连接和 Lambda 连接。
Widget::Widget(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("29 改为连接普通成员函数");
    resize(720, 540);
    auto *heading = new QLabel("29 改为连接普通成员函数", this);
    heading->setGeometry(24, 18, 670, 32);
    QFont font = heading->font();
    font.setPointSize(15);
    font.setBold(true);
    heading->setFont(font);
    output_ = new QPlainTextEdit(this);
    output_->setReadOnly(true);
    output_->setGeometry(24, 140, 670, 370);
    output_->setStyleSheet("font-size: 15px;");
    auto *run = new QPushButton("点击加一", this);
    run->setObjectName("runButton");
    run->setGeometry(24, 65, 180, 40);
    auto *reset = new QPushButton("清零", this);
    reset->setObjectName("resetButton");
    reset->setGeometry(230, 65, 180, 40);
    connect(run, &QPushButton::clicked, this, &Widget::increase);
    connect(reset, &QPushButton::clicked, this,
            [this]()
            {
                count_ = 0;
                refreshTitle();
                writeLine("清零后 count_ = 0");
            });
    refreshTitle();
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
