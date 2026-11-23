#include "widget.h"
#include <QPushButton>
#include <QPlainTextEdit>
#include <QLabel>
#include <QDebug>
#include <QString>

// 本步重点：捕获的是地址副本，指向的对象仍是同一个。
Widget::Widget(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("18 复制指针仍可改变目标");
    resize(720, 540);
    auto *heading = new QLabel("18 复制指针仍可改变目标", this);
    heading->setGeometry(24, 18, 670, 32);
    QFont font = heading->font();
    font.setPointSize(15);
    font.setBold(true);
    heading->setFont(font);
    output_ = new QPlainTextEdit(this);
    output_->setReadOnly(true);
    output_->setGeometry(24, 140, 670, 370);
    output_->setStyleSheet("font-size: 15px;");
    int number = 10;
    int *ptr = &number;
    auto change = [ptr]() { *ptr = 30; };
    change();
    writeLine("number = " + QString::number(number));
    writeLine("[ptr] 没有复制 number，只复制了地址。");
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
