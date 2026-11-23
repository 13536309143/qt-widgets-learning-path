#include "widget.h"
#include <QPushButton>
#include <QPlainTextEdit>
#include <QLabel>
#include <QDebug>
#include <QString>

// 本步重点：区分创建时保留的 base 和调用时传入的 x。
Widget::Widget(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("14 捕获列表与参数列表");
    resize(720, 540);
    auto *heading = new QLabel("14 捕获列表与参数列表", this);
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
    auto addBase = [base](int x) { return base + x; };
    writeLine("base 的副本 = 10");
    writeLine("addBase(3) = " + QString::number(addBase(3)));
    writeLine("addBase(8) = " + QString::number(addBase(8)));
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
