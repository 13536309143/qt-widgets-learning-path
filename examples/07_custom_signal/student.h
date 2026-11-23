#pragma once
#include <QObject>
#include <QString>
class Student : public QObject
{
    Q_OBJECT
  public:
    explicit Student(QObject *parent = nullptr) : QObject(parent) {}
  public slots:
    void treat()
    {
        emit message("请老师吃饭");
    }
    void treat(const QString &food)
    {
        emit message("请老师吃饭：" + food);
    }
  signals:
    void message(const QString &text);
};
