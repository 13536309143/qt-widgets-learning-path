#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include "verify.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    // 错误思路（仅说明，不执行）：
    // int localCount = 0;
    // 把 [&localCount] 保存到按钮回调，构造结束后引用就会失效。
    connect(ui->runButton, &QPushButton::clicked, this,
            [this]()
            {
                ++count_;
                writeLine("窗口成员仍然存活，count_ = " + QString::number(count_));
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
