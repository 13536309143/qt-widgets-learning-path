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
    setText("点击此区域后按 Tab，观察事件被拦截。");
}

bool EventArea::event(QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        auto *key = static_cast<QKeyEvent *>(event);
        if (key->key() == Qt::Key_Tab)
        {
            setText("event 已处理 Tab，阻止默认焦点切换");
            return true;
        }
    }
    return QLabel::event(event);
}
