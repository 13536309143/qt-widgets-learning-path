#include "widget.h"
#include <QPushButton>
#include <QPlainTextEdit>
#include <QLabel>
#include <QDebug>
#include <QString>

// 本步重点：在变量仍然存活时，立即调用引用捕获的 Lambda。
Widget::Widget(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("16 引用捕获修改原变量");
    resize(720, 540);
    auto *heading = new QLabel("16 引用捕获修改原变量", this);
    heading->setGeometry(24, 18, 670, 32);
    QFont font = heading->font();
    font.setPointSize(15);
    font.setBold(true);
    heading->setFont(font);
    output_ = new QPlainTextEdit(this);
    output_->setReadOnly(true);
    output_->setGeometry(24, 140, 670, 370);
    output_->setStyleSheet("font-size: 15px;");
    int n = 10;
    auto increase = [&n]() { ++n; };
    increase();
    writeLine("第一次调用后 n = " + QString::number(n));
    n = 40;
    increase();
    writeLine("第二次调用后 n = " + QString::number(n));
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
