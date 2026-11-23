#ifndef VERIFY_H
#define VERIFY_H
// 可选运行检查：只在传入 --verify 时执行。
#include <QtWidgets>
#include "dialog.h"
inline QString verificationFolder()
{
    const auto args = QCoreApplication::arguments();
    const int index = args.indexOf("--verify");
    return index >= 0 && index + 1 < args.size() ? args[index + 1] : QString();
}
inline void finishVerification(QWidget *window, const QString &folder, bool ok)
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

inline void scheduleVerification(Widget *instance)
{
    auto &window = *instance;

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
}
#endif // VERIFY_H
