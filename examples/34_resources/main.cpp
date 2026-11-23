#include <QtWidgets>
#include <QMouseEvent>
#include <QKeyEvent>

#include "verify.h"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QMainWindow w;
    w.resize(800, 580);
    w.setWindowTitle("34 资源路径 图片与背景");
    auto *label = new QLabel;
    QPixmap picture(":/images/demo.png");
    Q_ASSERT(!picture.isNull());
    label->setAlignment(Qt::AlignCenter);
    label->setPixmap(picture.scaled(600, 360, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    w.setCentralWidget(label);
    w.setWindowIcon(QIcon(":/images/demo.png"));
    w.show();
    scheduleVerification(&w);
    return app.exec();
}
