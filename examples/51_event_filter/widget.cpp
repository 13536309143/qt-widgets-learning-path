#include "widget.h"
#include "ui_widget.h"
#include "returnfilter.h"
#include "verify.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    auto *filter = new ReturnFilter(ui->resultLabel, this);
    ui->lineEdit->installEventFilter(filter);

    // 仅带 --verify 参数时安排检查；正常学习时不会执行。
    scheduleVerification(this);
}

Widget::~Widget()
{
    delete ui;
}
