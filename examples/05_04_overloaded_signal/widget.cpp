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
    connect(teacher, qOverload<const QString &>(&Teacher::hungry), student,
            qOverload<const QString &>(&Student::treat));
    connect(ui->runButton, &QPushButton::clicked, this,
            [teacher]() { teacher->requestFood(QStringLiteral("红烧排骨")); });

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
