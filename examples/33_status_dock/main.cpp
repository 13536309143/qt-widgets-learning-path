#include <QtWidgets>
#include <QMouseEvent>
#include <QKeyEvent>

#include "verify.h"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QMainWindow w;
    w.resize(800, 580);
    w.setWindowTitle("33 状态栏和停靠面板");
    w.setCentralWidget(new QTextEdit("这是主编辑区域"));
    w.statusBar()->addWidget(new QLabel("普通状态"));
    w.statusBar()->addPermanentWidget(new QLabel("UTF-8"));
    w.statusBar()->showMessage("临时消息 3 秒后消失", 3000);
    auto *dock = new QDockWidget("文档列表", &w);
    auto *list = new QListWidget;
    list->addItems({"第一章", "第二章", "第三章"});
    dock->setWidget(list);
    w.addDockWidget(Qt::LeftDockWidgetArea, dock);
    w.show();
    scheduleVerification(&w);
    return app.exec();
}
