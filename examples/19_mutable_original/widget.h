#pragma once
#include <QWidget>
class QPlainTextEdit;
class Widget : public QWidget
{
    Q_OBJECT
  public:
    explicit Widget(QWidget *parent = nullptr);
    void writeLine(const QString &text);

  private:
    void refreshTitle();
    void increase();
    int count_ = 0;
    QPlainTextEdit *output_ = nullptr;
};
