#pragma once
// 可选自动检查：真实创建窗口、调用按钮、抓取实际 QWidget 图像。
// 无 --verify 参数时直接返回，不影响手动学习。
#include <QApplication>
#include <QTimer>
#include <QPushButton>
#include <QPixmap>
#include <QFile>
#include <QTextStream>
#include <QDir>
inline void scheduleVerification(QWidget *window)
{
    const QStringList args = QCoreApplication::arguments();
    const int index = args.indexOf("--verify");
    if (index < 0 || index + 1 >= args.size())
        return;
    const QString folder = args.at(index + 1);
    QDir().mkpath(folder);
    QTimer::singleShot(400, window,
                       [window, folder]()
                       {
                           bool ok = true;
                           auto capture = [window, folder, &ok](const QString &name)
                           {
                               window->repaint();
                               if (!window->grab().save(folder + "/" + name + ".png"))
                                   ok = false;
                           };
                           capture("01_initial");
                           auto *button = window->findChild<QPushButton *>("runButton");
                           if (!button)
                               ok = false;
                           else
                               for (int i = 0; i < 1; ++i)
                                   button->click();
                           capture("02_after_clicks");
                           QFile status(folder + "/verification.txt");
                           if (status.open(QIODevice::WriteOnly | QIODevice::Text))
                           {
                               QTextStream(&status) << (ok ? "PASS" : "FAIL") << "\n";
                           }
                           QCoreApplication::exit(ok ? 0 : 2);
                       });
}
