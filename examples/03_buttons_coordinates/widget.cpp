#include "widget.h"
#include <QPushButton>
#include <QPlainTextEdit>
#include <QLabel>
#include <QDebug>
#include <QString>

// 本步重点：把按钮放到父窗口内指定位置。
Widget::Widget(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("03 按钮与父控件坐标");
    resize(720, 540);
    auto *heading = new QLabel("03 按钮与父控件坐标", this);
    heading->setGeometry(24, 18, 670, 32);
    QFont font = heading->font();
    font.setPointSize(15);
    font.setBold(true);
    heading->setFont(font);
    output_ = new QPlainTextEdit(this);
    output_->setReadOnly(true);
    output_->setGeometry(24, 140, 670, 370);
    output_->setStyleSheet("font-size: 15px;");
    auto *first = new QPushButton("按钮一", this);
    first->setGeometry(24, 70, 150, 40);
    auto *second = new QPushButton("按钮二", this);
    second->setGeometry(200, 70, 150, 40);
    writeLine("按钮一左上角为 (24,70)，按钮二为 (200,70)。");
    writeLine("坐标相对父窗口内容区域，x 向右，y 向下。");
}

// 小辅助函数：同时显示在窗口和 Qt Creator 的应用程序输出中。
void Widget::writeLine(const QString &text)
{
    if (output_)
        output_->appendPlainText(text);
    qDebug().noquote() << text;
}
void Widget::refreshTitle()
{
    setWindowTitle(QString("已点击 %1 次").arg(count_));
}
void Widget::increase()
{
    ++count_;
    refreshTitle();
    writeLine("加一后 count_ = " + QString::number(count_));
}
