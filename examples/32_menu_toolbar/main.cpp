#include <QtWidgets>
#include <QMouseEvent>
#include <QKeyEvent>

#include "verify.h"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QMainWindow w;
    w.resize(800, 580);
    w.setWindowTitle("32 菜单 动作与工具栏");
    auto *edit = new QTextEdit;
    w.setCentralWidget(edit);
    auto *menu = w.menuBar()->addMenu("文件");
    auto *save = new QAction("保存示例", &w);
    save->setObjectName("action");
    save->setShortcut(QKeySequence::Save);
    menu->addAction(save);
    auto *tools = w.addToolBar("文件工具");
    tools->addAction(save);
    tools->setAllowedAreas(Qt::LeftToolBarArea | Qt::RightToolBarArea | Qt::TopToolBarArea);
    QObject::connect(save, &QAction::triggered, &w, [edit]()
                     { edit->append("同一个 QAction 被触发。这里演示通知，没有写入磁盘。"); });
    w.show();
    scheduleVerification(&w);
    return app.exec();
}
