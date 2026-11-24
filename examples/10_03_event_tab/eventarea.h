#ifndef EVENTAREA_H
#define EVENTAREA_H
#include <QLabel>
class EventArea : public QLabel
{
    Q_OBJECT
public:
    explicit EventArea(QWidget *parent = nullptr);

protected:
    bool event(QEvent *event) override;
};
#endif // EVENTAREA_H
