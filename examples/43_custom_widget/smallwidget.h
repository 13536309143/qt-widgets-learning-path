#ifndef SMALLWIDGET_H
#define SMALLWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui
{
class SmallWidget;
}
QT_END_NAMESPACE

class SmallWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SmallWidget(QWidget *parent = nullptr);
    ~SmallWidget() override;
    void setValue(int value);
    int value() const;

private:
    Ui::SmallWidget *ui;
};
#endif // SMALLWIDGET_H
