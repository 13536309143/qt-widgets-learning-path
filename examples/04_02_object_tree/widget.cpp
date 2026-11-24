#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include "verify.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(ui->runButton, &QPushButton::clicked, this,
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
