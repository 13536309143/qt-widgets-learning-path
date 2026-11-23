#include "widget.h"
#include <QPushButton>
#include <QPlainTextEdit>
#include <QLabel>
#include <QDebug>
#include <QString>

// 本步重点：没有外部依赖时使用 []。
Widget::Widget(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("22 空捕获的按钮回调");
    resize(720, 540);
    auto *heading = new QLabel("22 空捕获的按钮回调", this);
    heading->setGeometry(24, 18, 670, 32);
    QFont font = heading->font();
    font.setPointSize(15);
    font.setBold(true);
    heading->setFont(font);
    output_ = new QPlainTextEdit(this);
    output_->setReadOnly(true);
    output_->setGeometry(24, 140, 670, 370);
    output_->setStyleSheet("font-size: 15px;");
    auto *run = new QPushButton("执行独立 Lambda", this);
    run->setObjectName("runButton");
    run->setGeometry(24, 65, 250, 40);
    connect(run, &QPushButton::clicked, this,
            []() { qDebug() << "按钮被按下，当前 Lambda 没有捕获外部变量"; });
    // 第二条连接只负责把结果显示到窗口中，不影响上面的空捕获示例。
    connect(run, &QPushButton::clicked, this,
            [this]() { writeLine("按钮被按下；空捕获示例也已向调试输出打印信息。"); });
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
