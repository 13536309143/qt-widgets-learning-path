#include "widget.h"
#include <QPushButton>
#include <QPlainTextEdit>
#include <QLabel>
#include <QDebug>
#include <QString>

// 本步重点：连续调用同一个函数对象，观察内部状态。
Widget::Widget(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("20 同一个 Lambda 会保留副本");
    resize(720, 540);
    auto *heading = new QLabel("20 同一个 Lambda 会保留副本", this);
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
    auto next = [m]() mutable { return ++m; };
    writeLine("next 第一次 = " + QString::number(next()));
    writeLine("next 第二次 = " + QString::number(next()));
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
