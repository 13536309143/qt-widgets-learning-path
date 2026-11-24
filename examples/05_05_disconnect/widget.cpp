#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include "verify.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    auto connection = connect(ui->sourceButton, &QPushButton::clicked, this,
                              [this]() { writeLine("收到 clicked"); });
    writeLine("第一次调用 click()");
    ui->sourceButton->click();
    QObject::disconnect(connection);
    writeLine("已断开；第二次调用 click()");
    ui->sourceButton->click();
    writeLine("第二次不会再输出 收到 clicked。");

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
