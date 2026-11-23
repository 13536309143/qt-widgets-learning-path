#include "widget.h"
#include <QPushButton>
#include <QPlainTextEdit>
#include <QLabel>
#include <QDebug>
#include <QString>

// 本步重点：对照错误原因，运行使用窗口成员的安全版本。
Widget::Widget(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("26 避免引用已消失的局部变量");
    resize(720, 540);
    auto *heading = new QLabel("26 避免引用已消失的局部变量", this);
    heading->setGeometry(24, 18, 670, 32);
    QFont font = heading->font();
    font.setPointSize(15);
    font.setBold(true);
    heading->setFont(font);
    output_ = new QPlainTextEdit(this);
    output_->setReadOnly(true);
    output_->setGeometry(24, 140, 670, 370);
    output_->setStyleSheet("font-size: 15px;");
    // 错误思路（仅说明，不执行）：
    // int localCount = 0;
    // 把 [&localCount] 保存到按钮回调，构造结束后引用就会失效。
    auto *run = new QPushButton("安全的长期计数", this);
    run->setObjectName("runButton");
    run->setGeometry(24, 65, 250, 40);
    connect(run, &QPushButton::clicked, this,
            [this]()
            {
                ++count_;
                writeLine("窗口成员仍然存活，count_ = " + QString::number(count_));
            });
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
