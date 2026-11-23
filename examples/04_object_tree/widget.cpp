#include "widget.h"
#include <QPushButton>
#include <QPlainTextEdit>
#include <QLabel>
#include <QDebug>
#include <QString>

// 本步重点：观察父对象删除子对象，同时区分局部指针与堆对象。
Widget::Widget(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("04 对象树与析构日志");
    resize(720, 540);
    auto *heading = new QLabel("04 对象树与析构日志", this);
    heading->setGeometry(24, 18, 670, 32);
    QFont font = heading->font();
    font.setPointSize(15);
    font.setBold(true);
    heading->setFont(font);
    output_ = new QPlainTextEdit(this);
    output_->setReadOnly(true);
    output_->setGeometry(24, 140, 670, 370);
    output_->setStyleSheet("font-size: 15px;");
    auto *run = new QPushButton("创建父对象并删除", this);
    run->setObjectName("runButton");
    run->setGeometry(24, 65, 240, 40);
    connect(run, &QPushButton::clicked, this,
            [this]()
            {
                auto *owner = new QObject(this);
                auto *child = new QObject(owner);
                connect(child, &QObject::destroyed, this,
                        [this]() { writeLine("子对象 destroyed：父对象负责回收子对象。"); });
                writeLine("子对象数量 = " + QString::number(owner->children().size()));
                delete owner; // 删除父对象，会删除 child；不要再访问 child
                writeLine("delete owner 已返回。");
            });
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
