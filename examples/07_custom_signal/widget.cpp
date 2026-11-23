#include "widget.h"
#include <QPushButton>
#include <QPlainTextEdit>
#include <QLabel>
#include <QDebug>
#include <QString>
#include "teacher.h"
#include "student.h"

// 本步重点：按钮先调用老师的方法，老师发出信号，学生处理。
Widget::Widget(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("07 自定义信号与槽");
    resize(720, 540);
    auto *heading = new QLabel("07 自定义信号与槽", this);
    heading->setGeometry(24, 18, 670, 32);
    QFont font = heading->font();
    font.setPointSize(15);
    font.setBold(true);
    heading->setFont(font);
    output_ = new QPlainTextEdit(this);
    output_->setReadOnly(true);
    output_->setGeometry(24, 140, 670, 370);
    output_->setStyleSheet("font-size: 15px;");
    auto *teacher = new Teacher(this);
    auto *student = new Student(this);
    connect(student, &Student::message, this, &Widget::writeLine);
    auto *run = new QPushButton("下课 请老师吃饭", this);
    run->setObjectName("runButton");
    run->setGeometry(24, 65, 250, 40);
    connect(teacher, qOverload<>(&Teacher::hungry), student, qOverload<>(&Student::treat));
    connect(run, &QPushButton::clicked, teacher, &Teacher::finishClass);
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
