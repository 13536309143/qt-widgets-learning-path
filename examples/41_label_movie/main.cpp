#include <QtWidgets>
#include <QMouseEvent>
#include <QKeyEvent>

#include "verify.h"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QMainWindow w;
    w.resize(800, 580);
    w.setWindowTitle("41 QLabel 文本 图片与动画");
    auto *root = new QWidget;
    auto *layout = new QVBoxLayout(root);
    layout->addWidget(new QLabel("普通文字"));
    auto *rich = new QLabel("<h2>Qt 富文本</h2><p><b>粗体</b>与颜色演示</p>");
    layout->addWidget(rich);
    auto *image = new QLabel;
    image->setPixmap(QPixmap(":/images/demo.png").scaledToWidth(360));
    layout->addWidget(image);
    auto *animation = new QLabel;
    auto *movie = new QMovie(":/images/demo.gif", QByteArray(), root);
    animation->setMovie(movie);
    movie->start();
    layout->addWidget(animation);
    auto *stop = new QPushButton("停止动画");
    stop->setObjectName("run");
    layout->addWidget(stop);
    QObject::connect(stop, &QPushButton::clicked, movie, &QMovie::stop);
    w.setCentralWidget(root);
    w.show();
    scheduleVerification(&w);
    return app.exec();
}
