#include <QtWidgets>
#include <QMouseEvent>
#include <QKeyEvent>

#include "verify.h"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QMainWindow w;
    w.resize(800, 580);
    w.setWindowTitle("42 QLineEdit 输入与显示模式");
    auto *root = new QWidget;
    auto *form = new QFormLayout(root);
    auto *name = new QLineEdit;
    name->setObjectName("name");
    name->setPlaceholderText("请输入名字");
    name->setTextMargins(8, 0, 8, 0);
    form->addRow("普通输入", name);
    auto *password = new QLineEdit("demo123");
    password->setEchoMode(QLineEdit::Password);
    form->addRow("掩码演示", password);
    auto *result = new QLabel("尚未输入");
    form->addRow("实时结果", result);
    QObject::connect(name, &QLineEdit::textChanged, result, [result](const QString &text)
                     { result->setText(QString("内容 %1，长度 %2").arg(text).arg(text.size())); });
    w.setCentralWidget(root);
    w.show();
    scheduleVerification(&w);
    return app.exec();
}
