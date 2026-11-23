#include "widget.h"
#include <QPushButton>
#include <QPlainTextEdit>
#include <QLabel>
#include <QDebug>
#include <QString>

// 本步重点：先独立调用 Lambda，再接触 Qt 回调。
Widget::Widget(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("12 普通函数改写为 Lambda");
    resize(720, 540);
    auto *heading = new QLabel("12 普通函数改写为 Lambda", this);
    heading->setGeometry(24, 18, 670, 32);
    QFont font = heading->font();
    font.setPointSize(15);
    font.setBold(true);
    heading->setFont(font);
    output_ = new QPlainTextEdit(this);
    output_->setReadOnly(true);
    output_->setGeometry(24, 140, 670, 370);
    output_->setStyleSheet("font-size: 15px;");
    auto add = [](int a, int b) -> int { return a + b; };
    int result = add(3, 4);
    writeLine("add(3,4) = " + QString::number(result));
    writeLine("auto 保存可调用对象；add(3,4) 才真正执行函数体。");
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
