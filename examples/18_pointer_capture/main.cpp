#include "widget.h"
#include "verify.h"
#include <QApplication>
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Widget window;
    window.show();
    // 普通运行不会执行自动检查。学习时先看 widget.cpp。
    scheduleVerification(&window);
    return app.exec();
}
