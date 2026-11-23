#ifndef RETURNFILTER_H
#define RETURNFILTER_H
#include <QObject>
class QLabel;
class ReturnFilter : public QObject
{
    Q_OBJECT
public:
    explicit ReturnFilter(QLabel *result, QObject *parent = nullptr);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    QLabel *result_;
};
#endif // RETURNFILTER_H
