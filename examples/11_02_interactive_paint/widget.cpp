#include "widget.h"
#include "ui_widget.h"
#include "drawingarea.h"
#include "verify.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(ui->toggleButton, &QPushButton::clicked, this,
            [this]()
            {
                auto *canvas = ui->drawingArea;
                canvas->setColor(canvas->color() == QColor(Qt::blue) ? QColor(Qt::red)
                                                                     : QColor(Qt::blue));
            });

    // 仅带 --verify 参数时安排检查；正常学习时不会执行。
    scheduleVerification(this);
}

Widget::~Widget()
{
    delete ui;
}
