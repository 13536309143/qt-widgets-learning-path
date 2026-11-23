#include <QtWidgets>
// --verify 只在自动检查时运行，正常打开窗口不会执行。
QString verificationFolder()
{
    const auto args = QCoreApplication::arguments();
    const int index = args.indexOf("--verify");
    return index >= 0 && index + 1 < args.size() ? args[index + 1] : QString();
}
void finishVerification(QWidget *window, const QString &folder, bool ok)
{
    QDir().mkpath(folder);
    ok = window->grab().save(folder + "/result.png") && ok;
    QFile status(folder + "/verification.txt");
    if (!status.open(QIODevice::WriteOnly))
        ok = false;
    else
        status.write(ok ? "PASS" : "FAIL");
    QCoreApplication::exit(ok ? 0 : 2);
}

#include <QSaveFile>
class TextEditor : public QMainWindow
{
  public:
    TextEditor()
    {
        resize(800, 560);
        editor_ = new QPlainTextEdit;
        setCentralWidget(editor_);
        auto *menu = menuBar()->addMenu("文件");
        auto *open = menu->addAction("打开 UTF-8 文本");
        auto *save = menu->addAction("保存");
        auto *saveAs = menu->addAction("另存为");
        open->setShortcut(QKeySequence::Open);
        save->setShortcut(QKeySequence::Save);
        connect(open, &QAction::triggered, this,
                [this]()
                {
                    if (!confirmDiscard())
                        return;
                    const auto path =
                        QFileDialog::getOpenFileName(this, "打开文本", {}, "Text (*.txt);;All (*)");
                    if (!path.isEmpty())
                        loadPath(path);
                });
        connect(save, &QAction::triggered, this, [this]() { saveCurrent(); });
        connect(saveAs, &QAction::triggered, this, [this]() { saveCurrent(true); });
        connect(editor_->document(), &QTextDocument::modificationChanged, this,
                [this]() { updateTitle(); });
        updateTitle();
    }
    bool loadPath(const QString &path)
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
        editor_->setPlainText(QString::fromUtf8(bytes));
        path_ = path;
        editor_->document()->setModified(false);
        updateTitle();
        return true;
    }
    bool savePath(const QString &path)
    {
        QSaveFile file(path);
        if (!file.open(QIODevice::WriteOnly))
        {
            statusBar()->showMessage("无法保存：" + file.errorString());
            return false;
        }
        const auto bytes = editor_->toPlainText().toUtf8();
        const bool written = file.write(bytes) == bytes.size();
        if (!written || !file.commit())
        {
            statusBar()->showMessage("保存失败：" + file.errorString());
            return false;
        }
        path_ = path;
        editor_->document()->setModified(false);
        updateTitle();
        statusBar()->showMessage("保存成功", 2000);
        return true;
    }
    QPlainTextEdit *editor() const
    {
        return editor_;
    }

  protected:
    void closeEvent(QCloseEvent *event) override
    {
        if (confirmDiscard())
            event->accept();
        else
            event->ignore();
    }

  private:
    bool saveCurrent(bool saveAs = false)
    {
        QString destination = path_;
        if (destination.isEmpty() || saveAs)
            destination =
                QFileDialog::getSaveFileName(this, "保存 UTF-8 文本", path_, "Text (*.txt)");
        return !destination.isEmpty() && savePath(destination);
    }
    bool confirmDiscard()
    {
        if (!editor_->document()->isModified())
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
    void updateTitle()
    {
        setWindowTitle((path_.isEmpty() ? QString("未命名") : QFileInfo(path_).fileName()) +
                       (editor_->document()->isModified() ? " *" : "") + " — UTF-8 文本编辑器");
    }
    QPlainTextEdit *editor_ = nullptr;
    QString path_;
};
int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    TextEditor window;
    window.show();
    const auto folder = verificationFolder();
    if (!folder.isEmpty())
        QTimer::singleShot(300, &window,
                           [&window, folder]()
                           {
                               QTemporaryDir directory;
                               const QString file = directory.filePath("roundtrip.txt");
                               window.editor()->setPlainText("Qt 文件读写验证\n第二行 UTF-8");
                               bool ok = directory.isValid() && window.savePath(file);
                               window.editor()->clear();
                               ok = window.loadPath(file) && ok;
                               ok = window.editor()->toPlainText() ==
                                        QStringLiteral("Qt 文件读写验证\n第二行 UTF-8") &&
                                    ok;
                               ok = !window.editor()->document()->isModified() && ok;
                               finishVerification(&window, folder, ok);
                           });
    return app.exec();
}
