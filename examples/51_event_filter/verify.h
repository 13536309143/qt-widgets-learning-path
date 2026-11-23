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

inline void scheduleVerification(Widget *instance)
{
    auto &window = *instance;
    auto *line = window.findChild<QLineEdit *>("lineEdit");
    auto *result = window.findChild<QLabel *>("resultLabel");
    const auto folder = verificationFolder();
    if (!folder.isEmpty())
        QTimer::singleShot(300, &window,
                           [&window, line, result, folder]()
                           {
                               line->setText("Qt 事件过滤器");
                               QKeyEvent key(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                               QCoreApplication::sendEvent(line, &key);
                               finishVerification(&window, folder,
                                                  result->text() ==
                                                      QStringLiteral("过滤器收到：Qt 事件过滤器"));
                           });
}
#endif // VERIFY_H
