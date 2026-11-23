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
            []() { qDebug() << "按钮被按下，当前 Lambda 没有捕获外部变量"; });
    // 第二条连接只负责把结果显示到窗口中，不影响上面的空捕获示例。
    connect(ui->runButton, &QPushButton::clicked, this,
            [this]() { writeLine("按钮被按下；空捕获示例也已向调试输出打印信息。"); });

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
