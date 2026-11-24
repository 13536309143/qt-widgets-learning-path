#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "verify.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->run, &QPushButton::clicked, this,
            [this]()
            {
                auto *box =
                    new QMessageBox(QMessageBox::Question, "保存选择", "文件已修改，是否保存？",
                                    QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, this);
                box->setDefaultButton(QMessageBox::Cancel);
                box->setAttribute(Qt::WA_DeleteOnClose);
                connect(box, &QDialog::finished, ui->resultEdit,
                        [this](int result)
                        {
                            if (result == QMessageBox::Yes)
                                ui->resultEdit->append("选择保存（仅演示）");
                            else if (result == QMessageBox::No)
                                ui->resultEdit->append("选择不保存");
                            else
                                ui->resultEdit->append("取消当前操作");
                        });
                box->open();
            });

    // 仅带 --verify 参数时安排检查；正常学习时不会执行。
    scheduleVerification(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
