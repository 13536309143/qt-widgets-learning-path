#ifndef VERIFY_H
#define VERIFY_H
// 可选运行检查：只在传入 --verify 时执行。
#include <QtWidgets>

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

inline void scheduleVerification(MainWindow *instance)
{
    auto &window = *instance;

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
}
#endif // VERIFY_H
