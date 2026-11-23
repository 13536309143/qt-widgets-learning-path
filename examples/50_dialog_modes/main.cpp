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

class DialogModes : public QWidget
{
  public:
    DialogModes()
    {
        resize(800, 560);
        setWindowTitle("对比 show open exec");
        auto *layout = new QVBoxLayout(this);
        log_ = new QPlainTextEdit;
        log_->setReadOnly(true);
        for (const auto &mode : {QString("show"), QString("open"), QString("exec")})
        {
            auto *button = new QPushButton(mode);
            button->setObjectName(mode);
            layout->addWidget(button);
            connect(button, &QPushButton::clicked, this, [this, mode]() { openMode(mode); });
        }
        layout->addWidget(log_);
    }
    QString log() const
    {
        return log_->toPlainText();
    }

  private:
    void setup(QDialog *dialog, const QString &mode)
    {
        dialog->setObjectName("demoDialog");
        dialog->setWindowTitle(mode + " 对话框");
        auto *layout = new QVBoxLayout(dialog);
        layout->addWidget(new QLabel("确定返回 Accepted，取消返回 Rejected"));
        auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        layout->addWidget(buttons);
        connect(buttons, &QDialogButtonBox::accepted, dialog, &QDialog::accept);
        connect(buttons, &QDialogButtonBox::rejected, dialog, &QDialog::reject);
    }
    void openMode(const QString &mode)
    {
        log_->appendPlainText(mode + " 调用前");
        if (mode == "exec")
        {
            QDialog dialog(this); // exec 返回之前，局部对象一直存在。
            setup(&dialog, mode);
            const int result = dialog.exec();
            log_->appendPlainText("exec 返回结果 " + QString::number(result));
        }
        else
        {
            auto *dialog = new QDialog(this);
            setup(dialog, mode);
            dialog->setAttribute(Qt::WA_DeleteOnClose);
            connect(dialog, &QDialog::finished, this, [this, mode](int result)
                    { log_->appendPlainText(mode + " finished " + QString::number(result)); });
            if (mode == "open")
                dialog->open();
            else
                dialog->show();
        }
        log_->appendPlainText(mode + " 调用后");
    }
    QPlainTextEdit *log_ = nullptr;
};
int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    DialogModes window;
    window.show();
    const auto folder = verificationFolder();
    if (!folder.isEmpty())
        QTimer::singleShot(
            300, &window,
            [&window, folder]()
            {
                bool ok = true;
                window.findChild<QPushButton *>("show")->click();
                auto *dialog = window.findChild<QDialog *>("demoDialog");
                ok = dialog && !dialog->isModal();
                if (dialog)
                {
                    dialog->setObjectName("finishedShow");
                    dialog->accept();
                }
                window.findChild<QPushButton *>("open")->click();
                dialog = window.findChild<QDialog *>("demoDialog");
                ok = dialog && dialog->windowModality() == Qt::WindowModal && ok;
                if (dialog)
                {
                    dialog->setObjectName("finishedOpen");
                    dialog->reject();
                }
                QTimer::singleShot(80, &window,
                                   [&window]()
                                   {
                                       auto *active = window.findChild<QDialog *>("demoDialog");
                                       if (active)
                                           active->accept();
                                   });
                window.findChild<QPushButton *>("exec")->click();
                ok = window.log().contains("exec 返回结果 1") && ok;
                ok = window.log().indexOf("open 调用后") < window.log().indexOf("open finished") &&
                     ok;
                finishVerification(&window, folder, ok);
            });
    return app.exec();
}
