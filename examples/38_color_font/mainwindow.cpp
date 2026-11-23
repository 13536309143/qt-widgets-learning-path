#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QColorDialog>
#include <QFontDialog>
#include "verify.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->run, &QPushButton::clicked, this,
            [this]()
            {
                auto *dialog =
                    new QColorDialog(ui->plainTextEdit->palette().color(QPalette::Text), this);
                dialog->setOption(QColorDialog::DontUseNativeDialog);
                dialog->setAttribute(Qt::WA_DeleteOnClose);
                connect(dialog, &QColorDialog::colorSelected, ui->plainTextEdit,
                        [this](const QColor &value)
                        {
                            auto pal = ui->plainTextEdit->palette();
                            pal.setColor(QPalette::Text, value);
                            ui->plainTextEdit->setPalette(pal);
                        });
                dialog->open();
            });
    connect(ui->fontButton, &QPushButton::clicked, this,
            [this]()
            {
                auto *dialog = new QFontDialog(ui->plainTextEdit->font(), this);
                dialog->setOption(QFontDialog::DontUseNativeDialog);
                dialog->setAttribute(Qt::WA_DeleteOnClose);
                connect(dialog, &QFontDialog::fontSelected, ui->plainTextEdit, &QWidget::setFont);
                dialog->open();
            });

    // 仅带 --verify 参数时安排检查；正常学习时不会执行。
    scheduleVerification(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
