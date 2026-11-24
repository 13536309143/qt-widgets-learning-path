#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSaveFile>
#include <QFileDialog>
#include <QMessageBox>
#include "verify.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionOpen, &QAction::triggered, this,
            [this]()
            {
                if (!confirmDiscard())
                    return;
                const auto path =
                    QFileDialog::getOpenFileName(this, "打开文本", {}, "Text (*.txt);;All (*)");
                if (!path.isEmpty())
                    loadPath(path);
            });
    connect(ui->actionSave, &QAction::triggered, this, [this]() { saveCurrent(); });
    connect(ui->actionSaveAs, &QAction::triggered, this, [this]() { saveCurrent(true); });
    connect(ui->plainTextEdit->document(), &QTextDocument::modificationChanged, this,
            [this]() { updateTitle(); });
    updateTitle();

    // 仅带 --verify 参数时安排检查；正常学习时不会执行。
    scheduleVerification(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::loadPath(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
    {
        statusBar()->showMessage("打开失败：" + file.errorString());
        return false;
    }
    const QByteArray bytes = file.readAll();
    if (file.error() != QFileDevice::NoError)
    {
        statusBar()->showMessage("读取失败：" + file.errorString());
        return false;
    }
    // 这个练习约定 UTF-8。其他编码应先识别或明确指定后再解码。
    ui->plainTextEdit->setPlainText(QString::fromUtf8(bytes));
    path_ = path;
    ui->plainTextEdit->document()->setModified(false);
    updateTitle();
    return true;
}

bool MainWindow::savePath(const QString &path)
{
    QSaveFile file(path);
    if (!file.open(QIODevice::WriteOnly))
    {
        statusBar()->showMessage("无法保存：" + file.errorString());
        return false;
    }
    const auto bytes = ui->plainTextEdit->toPlainText().toUtf8();
    const bool written = file.write(bytes) == bytes.size();
    if (!written || !file.commit())
    {
        statusBar()->showMessage("保存失败：" + file.errorString());
        return false;
    }
    path_ = path;
    ui->plainTextEdit->document()->setModified(false);
    updateTitle();
    statusBar()->showMessage("保存成功", 2000);
    return true;
}

QPlainTextEdit *MainWindow::editor() const
{
    return ui->plainTextEdit;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (confirmDiscard())
        event->accept();
    else
        event->ignore();
}

bool MainWindow::saveCurrent(bool saveAs)
{
    QString destination = path_;
    if (destination.isEmpty() || saveAs)
        destination = QFileDialog::getSaveFileName(this, "保存 UTF-8 文本", path_, "Text (*.txt)");
    return !destination.isEmpty() && savePath(destination);
}

bool MainWindow::confirmDiscard()
{
    if (!ui->plainTextEdit->document()->isModified())
        return true;
    const auto choice = QMessageBox::question(
        this, "未保存的修改", "是否保存后继续？",
        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Cancel);
    if (choice == QMessageBox::Cancel)
        return false;
    if (choice == QMessageBox::No)
        return true;
    return saveCurrent(); // 保存取消或失败时，继续保留当前窗口与文档。
}

void MainWindow::updateTitle()
{
    setWindowTitle((path_.isEmpty() ? QString("未命名") : QFileInfo(path_).fileName()) +
                   (ui->plainTextEdit->document()->isModified() ? " *" : "") +
                   " — UTF-8 文本编辑器");
}
