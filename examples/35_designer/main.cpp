#include <QtWidgets>
#include <QMouseEvent>
#include <QKeyEvent>
#include "ui_form.h"

#include "verify.h"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QMainWindow w;
    w.resize(800, 580);
    w.setWindowTitle("35 Designer 与 setupUi");
    Ui::Form ui;
    ui.setupUi(&w);
    QObject::connect(ui.runButton, &QPushButton::clicked, &w,
                     [=]() { ui.resultLabel->setText("你好，" + ui.nameEdit->text()); });
    w.show();
    scheduleVerification(&w);
    return app.exec();
}
