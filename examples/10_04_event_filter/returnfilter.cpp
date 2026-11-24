#include "returnfilter.h"
#include <QLabel>
#include <QLineEdit>
#include <QKeyEvent>

ReturnFilter::ReturnFilter(QLabel *result, QObject *parent)
    : QObject(parent)
    , result_(result)
{
}

bool ReturnFilter::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        auto *key = static_cast<QKeyEvent *>(event);
        if (key->key() == Qt::Key_Return || key->key() == Qt::Key_Enter)
        {
            auto *line = qobject_cast<QLineEdit *>(watched);
            if (line)
            {
                result_->setText("过滤器收到：" + line->text());
                return true; // 消费回车，不再交给输入框默认处理。
            }
        }
    }
    return QObject::eventFilter(watched, event);
}
