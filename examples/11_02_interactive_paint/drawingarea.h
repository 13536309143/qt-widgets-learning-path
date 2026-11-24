#ifndef DRAWINGAREA_H
#define DRAWINGAREA_H
#include <QWidget>
#include <QColor>
class DrawingArea : public QWidget
{
    Q_OBJECT
public:
    explicit DrawingArea(QWidget *parent = nullptr);
    void setColor(const QColor &color);
    QColor color() const;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QColor color_ = Qt::blue;
};
#endif // DRAWINGAREA_H
