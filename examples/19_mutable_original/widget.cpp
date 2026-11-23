#include "widget.h"
#include <QPushButton>
#include <QPlainTextEdit>
#include <QLabel>
#include <QDebug>
#include <QString>

// 本步重点：区分内部 m、返回值 sum、外部 m。
Widget::Widget(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("19 逐步复现原笔记的 mutable");
    resize(720, 540);
    auto *heading = new QLabel("19 逐步复现原笔记的 mutable", this);
    heading->setGeometry(24, 18, 670, 32);
    QFont font = heading->font();
    font.setPointSize(15);
    font.setBold(true);
    heading->setFont(font);
    output_ = new QPlainTextEdit(this);
    output_->setReadOnly(true);
    output_->setGeometry(24, 140, 670, 370);
    output_->setStyleSheet("font-size: 15px;");
    int m = 10;
    auto fun = [m, this](int a, int b) mutable
    {
        writeLine("lambda is running!");
        m = 300;
        writeLine("内部副本 m = " + QString::number(m));
        return a + b;
    };
    int sum = fun(100, 200);
    writeLine("返回值 sum = " + QString::number(sum));
    writeLine("外部 m = " + QString::number(m));
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
