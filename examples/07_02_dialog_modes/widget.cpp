#include "widget.h"
#include "ui_widget.h"
#include "dialog.h"
#include "verify.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(ui->show, &QPushButton::clicked, this, [this]() { openMode("show"); });
    connect(ui->open, &QPushButton::clicked, this, [this]() { openMode("open"); });
    connect(ui->exec, &QPushButton::clicked, this, [this]() { openMode("exec"); });

    // 仅带 --verify 参数时安排检查；正常学习时不会执行。
    scheduleVerification(this);
}

Widget::~Widget()
{
    delete ui;
}

QString Widget::log() const
{
    return ui->logEdit->toPlainText();
}

void Widget::openMode(const QString &mode)
{
    ui->logEdit->appendPlainText(mode + " 调用前");
    if (mode == "exec")
    {
        Dialog dialog(this); // exec 返回之前，局部对象一直存在。
        dialog.setObjectName("demoDialog");
        dialog.setWindowTitle(mode + " 对话框");
        const int result = dialog.exec();
        ui->logEdit->appendPlainText("exec 返回结果 " + QString::number(result));
    }
    else
    {
        auto *dialog = new Dialog(this);
        dialog->setObjectName("demoDialog");
        dialog->setWindowTitle(mode + " 对话框");
        dialog->setAttribute(Qt::WA_DeleteOnClose);
        connect(dialog, &QDialog::finished, this, [this, mode](int result)
                { ui->logEdit->appendPlainText(mode + " finished " + QString::number(result)); });
        if (mode == "open")
            dialog->open();
        else
            dialog->show();
    }
    ui->logEdit->appendPlainText(mode + " 调用后");
}
