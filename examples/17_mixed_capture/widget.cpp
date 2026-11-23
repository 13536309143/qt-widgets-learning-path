#include "widget.h"
#include <QPushButton>
#include <QPlainTextEdit>
#include <QLabel>
#include <QDebug>
#include <QString>

// 本步重点：同时使用一个值副本和一个引用。
Widget::Widget(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("17 显式混合捕获");
    resize(720, 540);
    auto *heading = new QLabel("17 显式混合捕获", this);
    heading->setGeometry(24, 18, 670, 32);
    QFont font = heading->font();
    font.setPointSize(15);
    font.setBold(true);
    heading->setFont(font);
    output_ = new QPlainTextEdit(this);
    output_->setReadOnly(true);
    output_->setGeometry(24, 140, 670, 370);
    output_->setStyleSheet("font-size: 15px;");
    int base = 10;
    int total = 0;
    auto accumulate = [base, &total](int x) { total += base + x; };
    base = 100;
    accumulate(3);
    writeLine("第一次 total = " + QString::number(total));
    accumulate(8);
    writeLine("第二次 total = " + QString::number(total));
    writeLine("外部 base = " + QString::number(base));
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
