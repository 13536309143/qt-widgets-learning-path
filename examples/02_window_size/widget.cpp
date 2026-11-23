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
                setFixedSize(720, 540);
                writeLine("已固定大小，不能再拖动改变窗口尺寸。");
            });
    writeLine("当前使用 resize(720, 540)，可拖动窗口边缘调整大小。");

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
