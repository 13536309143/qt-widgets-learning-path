#pragma once
// 仅 --verify 时执行；真实运行后抓取控件图像，不影响正常学习。
#include <QtWidgets>
#include <QMouseEvent>
#include <QKeyEvent>
inline void scheduleVerification(QMainWindow *w)
{
    auto args = QCoreApplication::arguments();
    int i = args.indexOf("--verify");
    if (i < 0 || i + 1 >= args.size())
        return;
    QString folder = args[i + 1];
    QDir().mkpath(folder);
    QTimer::singleShot(350, w,
                       [w, folder]()
                       {
                           bool ok = w->grab().save(folder + "/01_initial.png");
                           w->findChild<QLineEdit *>("name")->setText("学习 Qt");
                           QTimer::singleShot(
                               250, w,
                               [w, folder, ok]() mutable
                               {
                                   ok = ok && (w->findChild<QLineEdit *>("name")->text() ==
                                               QStringLiteral("学习 Qt"));
                                   for (auto *dialog : w->findChildren<QDialog *>())
                                       if (dialog->isVisible())
                                       {
                                           ok =
                                               dialog->grab().save(folder + "/02_dialog.png") && ok;
                                           dialog->reject();
                                       }
                                   w->repaint();
                                   ok = w->grab().save(folder + "/03_result.png") && ok;
                                   QFile file(folder + "/verification.txt");
                                   if (file.open(QIODevice::WriteOnly))
                                       file.write(ok ? "PASS" : "FAIL");
                                   QCoreApplication::exit(ok ? 0 : 2);
                               });
                       });
}
