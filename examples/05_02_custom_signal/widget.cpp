#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include "teacher.h"
#include "student.h"
#include "verify.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    auto *teacher = new Teacher(this);
    auto *student = new Student(this);
    connect(student, &Student::message, this, &Widget::writeLine);
    connect(teacher, qOverload<>(&Teacher::hungry), student, qOverload<>(&Student::treat));
    connect(ui->runButton, &QPushButton::clicked, teacher, &Teacher::finishClass);

    // 仅带 --verify 参数时安排检查；正常学习时不会执行。
    scheduleVerification(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::writeLine(const QString &text)
{
    ui->outputEdit->appendPlainText(text);
    qDebug().noquote() << text;
}
