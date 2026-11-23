#include "widget.h"
#include <QPushButton>
#include <QPlainTextEdit>
#include <QLabel>
#include <QDebug>
#include <QString>

// 本步重点：验证断开前有通知，断开后没有通知。
Widget::Widget(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("10 保存句柄并断开连接");
    resize(720, 540);
    auto *heading = new QLabel("10 保存句柄并断开连接", this);
    heading->setGeometry(24, 18, 670, 32);
    QFont font = heading->font();
    font.setPointSize(15);
    font.setBold(true);
    heading->setFont(font);
    output_ = new QPlainTextEdit(this);
    output_->setReadOnly(true);
    output_->setGeometry(24, 140, 670, 370);
    output_->setStyleSheet("font-size: 15px;");
    auto *source = new QPushButton("发送一次信号", this);
    source->setGeometry(24, 65, 180, 40);
    auto connection =
        connect(source, &QPushButton::clicked, this, [this]() { writeLine("收到 clicked"); });
    writeLine("第一次调用 click()");
    source->click();
    QObject::disconnect(connection);
    writeLine("已断开；第二次调用 click()");
    source->click();
    writeLine("第二次不会再输出 收到 clicked。");
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
