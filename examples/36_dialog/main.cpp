#include <QtWidgets>
#include <QMouseEvent>
#include <QKeyEvent>

#include "verify.h"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QMainWindow w;
    w.resize(800, 580);
    w.setWindowTitle("36 模态 非模态与对象生命期");
    auto *button = new QPushButton("打开窗口模态对话框");
    button->setObjectName("run");
    w.setCentralWidget(button);
    QObject::connect(button, &QPushButton::clicked, &w,
                     [&w]()
                     {
                         auto *dialog = new QDialog(&w);
                         dialog->setAttribute(Qt::WA_DeleteOnClose);
                         dialog->setWindowTitle("open 立即返回");
                         auto *layout = new QVBoxLayout(dialog);
                         layout->addWidget(new QLabel("父窗口暂不能输入，事件循环仍在工作。"));
                         auto *ok = new QPushButton("确定");
                         layout->addWidget(ok);
                         QObject::connect(ok, &QPushButton::clicked, dialog, &QDialog::accept);
                         dialog->open();
                     });
    w.show();
    scheduleVerification(&w);
    return app.exec();
}
