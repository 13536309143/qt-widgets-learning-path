#include "widget.h"
#include <QPushButton>
#include <QPlainTextEdit>
#include <QLabel>
#include <QDebug>
#include <QString>

// 本步重点：先建立父对象，再建立子对象，离开作用域时逆序析构。
Widget::Widget(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("05 栈对象安全的构造顺序");
    resize(720, 540);
    auto *heading = new QLabel("05 栈对象安全的构造顺序", this);
    heading->setGeometry(24, 18, 670, 32);
    QFont font = heading->font();
    font.setPointSize(15);
    font.setBold(true);
    heading->setFont(font);
    output_ = new QPlainTextEdit(this);
    output_->setReadOnly(true);
    output_->setGeometry(24, 140, 670, 370);
    output_->setStyleSheet("font-size: 15px;");
    {
        QWidget parent;
        QPushButton child("栈按钮", &parent);
        connect(&child, &QObject::destroyed, this,
                [this]() { writeLine("1 子按钮先析构，从父对象列表移除。"); });
        connect(&parent, &QObject::destroyed, this,
                [this]() { writeLine("2 父窗口后析构，不会重复删除子按钮。"); });
    }
    writeLine("3 已安全离开局部作用域。");
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
