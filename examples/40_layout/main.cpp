#include <QtWidgets>
#include <QMouseEvent>
#include <QKeyEvent>

#include "verify.h"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QMainWindow w;
    w.resize(800, 580);
    w.setWindowTitle("40 水平 垂直 网格与表单布局");
    auto *root = new QWidget;
    auto *outer = new QVBoxLayout(root);
    auto *form = new QFormLayout;
    form->addRow("用户名", new QLineEdit);
    form->addRow("备注", new QLineEdit);
    outer->addLayout(form);
    auto *grid = new QGridLayout;
    for (int i = 0; i < 6; ++i)
        grid->addWidget(new QPushButton(QString::number(i + 1)), i / 3, i % 3);
    outer->addLayout(grid);
    auto *row = new QHBoxLayout;
    row->addStretch();
    row->addWidget(new QPushButton("确定"));
    row->addWidget(new QPushButton("取消"));
    outer->addLayout(row);
    outer->setContentsMargins(20, 20, 20, 20);
    outer->setSpacing(14);
    w.setCentralWidget(root);
    w.show();
    scheduleVerification(&w);
    return app.exec();
}
