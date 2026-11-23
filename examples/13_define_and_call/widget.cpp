#include "widget.h"
#include <QPushButton>
#include <QPlainTextEdit>
#include <QLabel>
#include <QDebug>
#include <QString>

// 本步重点：创建函数对象不会自动执行函数体。
Widget::Widget(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("13 定义与调用的区别");
    resize(720, 540);
    auto *heading = new QLabel("13 定义与调用的区别", this);
    heading->setGeometry(24, 18, 670, 32);
    QFont font = heading->font();
    font.setPointSize(15);
    font.setBold(true);
    heading->setFont(font);
    output_ = new QPlainTextEdit(this);
    output_->setReadOnly(true);
    output_->setGeometry(24, 140, 670, 370);
    output_->setStyleSheet("font-size: 15px;");
    writeLine("1 开始定义 Lambda");
    auto say = [this]() { writeLine("3 函数体此时执行"); };
    writeLine("2 定义完成，函数体还没有执行");
    say();
    writeLine("4 调用完成");
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
