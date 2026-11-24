#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include "verify.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->run, &QPushButton::clicked, this,
            [this]()
            {
                auto *dialog = new QFileDialog(this, "选择文件",
                                               QStringLiteral(QT_LESSON_SOURCE_DIR "/samples"));
                dialog->setOption(QFileDialog::DontUseNativeDialog);
                dialog->setFileMode(QFileDialog::ExistingFiles);
                dialog->setNameFilter("Text (*.txt);;Images (*.png *.jpg)");
                dialog->setAttribute(Qt::WA_DeleteOnClose);
                connect(dialog, &QFileDialog::filesSelected, ui->resultEdit,
                        [this](const QStringList &files)
                        { ui->resultEdit->setPlainText(files.join('\n')); });
                dialog->open();
            });

    // 仅带 --verify 参数时安排检查；正常学习时不会执行。
    scheduleVerification(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
