#include <QtWidgets>
#include <QMouseEvent>
#include <QKeyEvent>
class Canvas : public QWidget
{
  protected:
    void paintEvent(QPaintEvent *) override
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.fillRect(rect(), Qt::white);
        painter.setPen(QPen(Qt::darkGray, 2));
        painter.drawLine(35, 35, 500, 270);
        painter.setPen(QPen(Qt::red, 3));
        painter.drawRect(60, 70, 360, 180);
        painter.setPen(QPen(Qt::blue, 5));
        painter.drawEllipse(60, 70, 360, 180);
        painter.setPen(Qt::black);
        painter.drawText(50, 320, "QPainter 在 paintEvent 中绘制，缩放和遮挡后仍可重绘。");
        QImage image(180, 80, QImage::Format_ARGB32);
        image.fill(QColor("#dbeafe"));
        {
            QPainter imagePainter(&image);
            imagePainter.setPen(Qt::darkBlue);
            imagePainter.drawText(image.rect(), Qt::AlignCenter, "QImage 离屏绘图");
        }
        painter.drawImage(50, 360, image);
    }
};

#include "verify.h"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QMainWindow w;
    w.resize(800, 580);
    w.setWindowTitle("48 QPainter 与绘图设备");
    w.setCentralWidget(new Canvas);
    w.statusBar()->showMessage("红矩形 蓝椭圆 灰直线；下方是 QImage 离屏绘图。");
    w.show();
    scheduleVerification(&w);
    return app.exec();
}
