#pragma once
#include <QtWidgets>
class SmallWidget : public QWidget
{
  public:
    explicit SmallWidget(QWidget *parent = nullptr) : QWidget(parent)
    {
        auto *row = new QHBoxLayout(this);
        spin = new QSpinBox;
        auto *slider = new QSlider(Qt::Horizontal);
        spin->setRange(0, 100);
        slider->setRange(0, 100);
        row->addWidget(spin);
        row->addWidget(slider);
        QObject::connect(spin, &QSpinBox::valueChanged, slider, &QSlider::setValue);
        QObject::connect(slider, &QSlider::valueChanged, spin, &QSpinBox::setValue);
    }
    void setValue(int value)
    {
        spin->setValue(value);
    }
    int value() const
    {
        return spin->value();
    }

  private:
    QSpinBox *spin;
};
