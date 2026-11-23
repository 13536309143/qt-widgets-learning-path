#include <QtWidgets>
#include <QMouseEvent>
#include <QKeyEvent>

#include "verify.h"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QMainWindow w;
    w.resize(800, 580);
    w.setWindowTitle("39 消息框与三种选择结果");
    auto *root = new QWidget;
    auto *layout = new QVBoxLayout(root);
    auto *button = new QPushButton("询问是否保存");
    button->setObjectName("run");
    auto *text = new QTextEdit;
    layout->addWidget(button);
    layout->addWidget(text);
    w.setCentralWidget(root);
    QObject::connect(button, &QPushButton::clicked, &w,
                     [&w, text]()
                     {
                         auto *box = new QMessageBox(
                             QMessageBox::Question, "保存选择", "文件已修改，是否保存？",
                             QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, &w);
                         box->setDefaultButton(QMessageBox::Cancel);
                         box->setAttribute(Qt::WA_DeleteOnClose);
                         QObject::connect(box, &QDialog::finished, text,
                                          [text](int result)
                                          {
                                              if (result == QMessageBox::Yes)
                                                  text->append("选择保存（仅演示）");
                                              else if (result == QMessageBox::No)
                                                  text->append("选择不保存");
                                              else
                                                  text->append("取消当前操作");
                                          });
                         box->open();
                     });
    w.show();
    scheduleVerification(&w);
    return app.exec();
}
