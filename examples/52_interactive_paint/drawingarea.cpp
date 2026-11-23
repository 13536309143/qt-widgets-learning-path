#include "drawingarea.h"
#include <QPainter>

DrawingArea::DrawingArea(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(500, 300);
}

void DrawingArea::setColor(const QColor &color)
{
    color_ = color;
    update();
}

QColor DrawingArea::color() const
{
    return color_;
}

void DrawingArea::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), Qt::white);
    painter.setPen(QPen(color_, 5));
    painter.setBrush(QColor(color_.red(), color_.green(), color_.blue(), 45));
    const QRectF area = QRectF(rect()).adjusted(40, 40, -40, -40);
    painter.drawEllipse(area);
}
