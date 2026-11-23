#include <QtWidgets>
#include <QMouseEvent>
#include <QKeyEvent>

#include "verify.h"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QMainWindow w;
    w.resize(800, 580);
    w.setWindowTitle("44 堆叠页面与窗口切换");
    auto *stack = new QStackedWidget;
    stack->setObjectName("pages");
    auto *login = new QPushButton("进入主页（仅演示导航）");
    login->setObjectName("run");
    auto *home = new QPushButton("返回登录页");
    home->setObjectName("back");
    stack->addWidget(login);
    stack->addWidget(home);
    w.setCentralWidget(stack);
    QObject::connect(login, &QPushButton::clicked, stack, [stack]() { stack->setCurrentIndex(1); });
    QObject::connect(home, &QPushButton::clicked, stack, [stack]() { stack->setCurrentIndex(0); });
    w.show();
    scheduleVerification(&w);
    return app.exec();
}
