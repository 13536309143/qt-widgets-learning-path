#pragma once
#include <QObject>
#include <QString>
class Teacher : public QObject
{
    Q_OBJECT
public:
    explicit Teacher(QObject *parent = nullptr)
        : QObject(parent)
    {
    }
    void finishClass()
    {
        emit hungry();
    }
    void requestFood(const QString &food)
    {
        emit hungry(food);
    }
signals:
    void hungry();
    void hungry(const QString &food);
};
