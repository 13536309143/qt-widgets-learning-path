#include <QtWidgets>
#include <QMouseEvent>
#include <QKeyEvent>
#include "smallwidget.h"

#include "verify.h"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QMainWindow w;
    w.resize(800, 580);
    w.setWindowTitle("43 组合控件与 Designer 提升");
    auto *root = new QWidget;
    auto *layout = new QVBoxLayout(root);
    layout->addWidget(new QLabel("同一个自定义控件类，可以创建多个独立实例。"));
    auto *first = new SmallWidget;
    first->setObjectName("first");
    first->setValue(25);
    auto *second = new SmallWidget;
    second->setValue(70);
    layout->addWidget(first);
    layout->addWidget(second);
    layout->addStretch();
    w.setCentralWidget(root);
    w.show();
    scheduleVerification(&w);
    return app.exec();
}
