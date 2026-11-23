#ifndef VERIFY_H
#define VERIFY_H
// 可选运行检查：只在传入 --verify 时执行。
#include <QtWidgets>
#include "drawingarea.h"
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
    auto *canvas = window.findChild<DrawingArea *>("drawingArea");
    auto *button = window.findChild<QPushButton *>("toggleButton");
    const auto folder = verificationFolder();
    if (!folder.isEmpty())
        QTimer::singleShot(
            300, &window,
            [&window, canvas, button, folder]()
            {
                button->click();
                window.resize(900, 620);
                QTimer::singleShot(
                    100, &window,
                    [&window, canvas, folder]()
                    {
                        const auto image = canvas->grab().toImage();
                        bool hasRed = false;
                        for (int y = 0; y < image.height() && !hasRed; ++y)
                            for (int x = 0; x < image.width(); ++x)
                            {
                                const QColor pixel = image.pixelColor(x, y);
                                if (pixel.red() > 200 && pixel.green() < 60 && pixel.blue() < 60)
                                {
                                    hasRed = true;
                                    break;
                                }
                            }
                        finishVerification(&window, folder,
                                           canvas->color() == QColor(Qt::red) && hasRed);
                    });
            });
}
#endif // VERIFY_H
