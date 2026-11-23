#include <QtWidgets>
// --verify 只在自动检查时运行，正常打开窗口不会执行。
QString verificationFolder()
{
    const auto args = QCoreApplication::arguments();
    const int index = args.indexOf("--verify");
    return index >= 0 && index + 1 < args.size() ? args[index + 1] : QString();
}
void finishVerification(QWidget *window, const QString &folder, bool ok)
{
    QDir().mkpath(folder);
    ok = window->grab().save(folder + "/result.png") && ok;
    QFile status(folder + "/verification.txt");
    if (!status.open(QIODevice::WriteOnly))
        ok = false;
    else
        status.write(ok ? "PASS" : "FAIL");
    QCoreApplication::exit(ok ? 0 : 2);
}

class DrawingArea : public QWidget
{
  public:
    explicit DrawingArea(QWidget *parent = nullptr) : QWidget(parent)
    {
        setMinimumSize(500, 300);
    }
    void setColor(const QColor &color)
    {
        color_ = color;
        update();
    }
    QColor color() const
    {
        return color_;
    }

  protected:
    void paintEvent(QPaintEvent *) override
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.fillRect(rect(), Qt::white);
        painter.setPen(QPen(color_, 5));
        painter.setBrush(QColor(color_.red(), color_.green(), color_.blue(), 45));
        const QRectF area = QRectF(rect()).adjusted(40, 40, -40, -40);
        painter.drawEllipse(area);
    }

  private:
    QColor color_ = Qt::blue; // 图形状态保存在对象中，重绘后不会丢失。
};
int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QWidget window;
    window.resize(800, 560);
    window.setWindowTitle("状态驱动的交互绘图");
    auto *layout = new QVBoxLayout(&window);
    auto *button = new QPushButton("蓝色与红色切换");
    auto *canvas = new DrawingArea;
    layout->addWidget(button);
    layout->addWidget(canvas, 1);
    QObject::connect(button, &QPushButton::clicked, canvas,
                     [canvas]()
                     {
                         canvas->setColor(canvas->color() == QColor(Qt::blue) ? QColor(Qt::red)
                                                                              : QColor(Qt::blue));
                     });
    window.show();
    const auto folder = verificationFolder();
    if (!folder.isEmpty())
        QTimer::singleShot(
            300, &window,
            [&window, canvas, button, folder]()
            {
                button->click();
                window.resize(900, 620);
                QTimer::singleShot(
                    100, &window,
                    [&window, canvas, folder]()
                    {
                        const auto image = canvas->grab().toImage();
                        bool hasRed = false;
                        for (int y = 0; y < image.height() && !hasRed; ++y)
                            for (int x = 0; x < image.width(); ++x)
                            {
                                const QColor pixel = image.pixelColor(x, y);
                                if (pixel.red() > 200 && pixel.green() < 60 && pixel.blue() < 60)
                                {
                                    hasRed = true;
                                    break;
                                }
                            }
                        finishVerification(&window, folder,
                                           canvas->color() == QColor(Qt::red) && hasRed);
                    });
            });
    return app.exec();
}
