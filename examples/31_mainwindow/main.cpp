#include <QtWidgets>
#include <QMouseEvent>
#include <QKeyEvent>

#include "verify.h"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QMainWindow w;
    w.resize(800, 580);
    w.setWindowTitle("31 主窗口与中心部件");
    auto *editor = new QTextEdit;
    editor->setPlainText("中心部件就是主窗口的工作区。\n试着拖动窗口边缘。");
    w.setCentralWidget(editor);
    w.menuBar()->addMenu("文件");
    w.statusBar()->showMessage("主窗口已就绪");
    w.show();
    scheduleVerification(&w);
    return app.exec();
}
