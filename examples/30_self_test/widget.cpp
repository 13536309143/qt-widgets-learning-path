#include "widget.h"
#include <QPushButton>
#include <QPlainTextEdit>
#include <QLabel>
#include <QDebug>
#include <QString>

// 本步重点：对值捕获、引用捕获和 mutable 先预测再运行。
Widget::Widget(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("30 Lambda 自测结果对照");
    resize(720, 540);
    auto *heading = new QLabel("30 Lambda 自测结果对照", this);
    heading->setGeometry(24, 18, 670, 32);
    QFont font = heading->font();
    font.setPointSize(15);
    font.setBold(true);
    heading->setFont(font);
    output_ = new QPlainTextEdit(this);
    output_->setReadOnly(true);
    output_->setGeometry(24, 140, 670, 370);
    output_->setStyleSheet("font-size: 15px;");
    int x = 5;
    auto value = [x]() { return x + 1; };
    x = 100;
    writeLine("题一 f() = " + QString::number(value()) + ", 外部 x = " + QString::number(x));
    int y = 5;
    auto reference = [&y]() { ++y; };
    reference();
    writeLine("题二外部 y = " + QString::number(y));
    int z = 5;
    auto state = [z]() mutable { return ++z; };
    int first = state();
    int second = state();
    writeLine(QString("题三两次返回 %1、%2，外部 z = %3").arg(first).arg(second).arg(z));
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
