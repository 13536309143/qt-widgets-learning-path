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
    QTimer::singleShot(
        350, w,
        [w, folder]()
        {
            bool ok = w->grab().save(folder + "/01_initial.png");
            w->resize(920, 620);
            QTimer::singleShot(
                250, w,
                [w, folder, ok]() mutable
                {
                    ok = ok && (w->size() == QSize(920, 620));
                    // 实际执行 No 和 Yes 两个关闭分支。
                    auto answer = [w](QMessageBox::StandardButton choice)
                    {
                        QTimer::singleShot(80, w,
                                           [w, choice]()
                                           {
                                               for (auto *box : w->findChildren<QMessageBox *>())
                                                   if (box->isVisible())
                                                       box->button(choice)->click();
                                           });
                    };
                    answer(QMessageBox::No);
                    w->close();
                    ok = ok && w->isVisible();
                    answer(QMessageBox::Yes);
                    w->close();
                    ok = ok && !w->isVisible();
                    for (auto *dialog : w->findChildren<QDialog *>())
                        if (dialog->isVisible())
                        {
                            ok = dialog->grab().save(folder + "/02_dialog.png") && ok;
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
