#include "eventarea.h"
#include <QMouseEvent>
#include <QResizeEvent>
#include <QKeyEvent>

EventArea::EventArea(QWidget *parent)
    : QLabel(parent)
{
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    setAlignment(Qt::AlignCenter);
    setText("拖动主窗口边缘观察尺寸变化。");
}

void EventArea::resizeEvent(QResizeEvent *event)
{
    setText(QString("区域大小 %1 × %2").arg(event->size().width()).arg(event->size().height()));
    QLabel::resizeEvent(event);
}
