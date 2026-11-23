# 第 36 章 重写 event 拦截 Tab

本章项目：[完整源码](../../examples/47_event_tab)。先运行项目观察行为，再按以下顺序阅读。

## 本章要解决的问题

event(QEvent *) 返回 bool，表示是否处理这个事件。先检查 type 再把通用指针转换到对应事件类型；未经类型判断的强制转换可能访问错误的数据。处理感兴趣的事件后返回 true；其余事件必须 return 基类::event(event)，避免破坏默认行为。

Tab 常被控件系统用来切换焦点，因此只在 keyPressEvent 中写代码可能不够。要改变这个行为，可以在目标控件的 event 中拦截 KeyPress，识别 Qt::Key_Tab。键盘事件发给有焦点的对象，先确保目标控件有适当 focusPolicy 并获得焦点。

## 事件分发 事件处理和事件过滤

特定处理函数适合某一类事件；重写 event 适合介入目标对象的分发；eventFilter 适合在不改被观察对象继承关系时提前观察或消费事件。第 51 步在 QLineEdit 上安装过滤器，识别回车并更新标签，不需要创建 QLineEdit 子类。

## 按执行顺序拆解

事件类型确定后才能安全转换到 QKeyEvent。第 47 步在 event 中识别 KeyPress 和 Tab，返回 true 消费这个事件；其余必须交给 QLabel::event。第 51 步则先进入过滤器；过滤器返回 true 会阻止继续送到被观察对象，返回 false 或基类结果则允许默认流程继续。

## 参数与行为边界

键盘事件以焦点为中心。focusPolicy 决定能否通过 Tab 或点击等方式获得焦点，setFocus 才是当前请求。QEvent::accept 与 event 返回值并不是所有事件中都可机械等同的通用开关。sendEvent 同步送达，postEvent 排队稍后送达且由 Qt 接管事件对象。

## 易错点与排查

返回 true 不是“所有事件继续向下传递”。不要在末尾无条件 return true。QEvent::accept() 与 event() 返回值有关但不能在所有事件种类下简单当成同一开关。原例中 Ctrl、Tab 等专用按键应使用 Qt 枚举。

遇到问题时先区分三个层次：编译错误检查声明、头文件与构建配置；界面无反应检查连接、接收对象与生存期；运行结果不对检查输入、返回值和分支是否真正执行。可以先在连接入口记录一条日志，再在结果处理处记录一条，定位中间哪一步没有到达。

## 完整示例与配套文件

以下为本步完整程序。若存在 .ui、qrc 或额外头文件，必须把同一目录中的文件一起下载。构建期生成的 ui_*.h、moc 文件不需要手动维护。

### main.cpp

```cpp
#include <QtWidgets>
#include <QMouseEvent>
#include <QKeyEvent>
class EventArea : public QLabel
{
  public:
    explicit EventArea(QWidget *p = nullptr) : QLabel(p)
    {
        setMouseTracking(true);
        setFocusPolicy(Qt::StrongFocus);
        setAlignment(Qt::AlignCenter);
        setText("移动鼠标，点击后按键盘；拖动主窗口观察大小变化。");
    }

  protected:
    void resizeEvent(QResizeEvent *event) override
    {
        setText(QString("区域大小 %1 × %2").arg(event->size().width()).arg(event->size().height()));
        QLabel::resizeEvent(event);
    }
    void mouseMoveEvent(QMouseEvent *event) override
    {
        setText(QString("鼠标位置 %1，%2").arg(event->position().x()).arg(event->position().y()));
        QLabel::mouseMoveEvent(event);
    }
    void mousePressEvent(QMouseEvent *event) override
    {
        setFocus();
        setText(event->button() == Qt::LeftButton ? "左键按下" : "其他鼠标键按下");
        QLabel::mousePressEvent(event);
    }
    bool event(QEvent *event) override
    {
        if (event->type() == QEvent::KeyPress)
        {
            auto *key = static_cast<QKeyEvent *>(event);
            if (key->key() == Qt::Key_Tab)
            {
                setText("event 已处理 Tab，阻止默认焦点切换");
                return true;
            }
        }
        return QLabel::event(event);
    }
};
class CloseWindow : public QMainWindow
{
  protected:
    void closeEvent(QCloseEvent *event) override
    {
        auto choice = QMessageBox::question(this, "关闭确认", "确定关闭窗口吗？",
                                            QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        if (choice == QMessageBox::Yes)
            event->accept();
        else
            event->ignore();
    }
};

#include "verify.h"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QMainWindow w;
    w.resize(800, 580);
    w.setWindowTitle("47 重写 event 拦截 Tab");
    w.setCentralWidget(new EventArea);
    w.centralWidget()->setFocus();
    w.statusBar()->showMessage("点击中央区域后按 Tab，观察默认焦点切换被拦截。");
    w.show();
    scheduleVerification(&w);
    return app.exec();
}

```

### 47_event_tab.pro

```qmake
QT += widgets
CONFIG += c++17
TEMPLATE = app
TARGET = 47_event_tab
SOURCES += main.cpp
HEADERS += verify.h

```

自动检查用的 verify.h 已在项目目录中提供；普通运行没有 --verify 参数时，不会执行自动操作。

## 运行截图与观察

![重写 event 拦截 Tab 03_result](../../examples/47_event_tab/screenshots/03_result.png)

## 自己动手

给输入框安装第 51 步过滤器，输入中文后回车观察结果。再把 true 改为基类返回，观察输入框自己的 returnPressed 是否仍可触发。不要同时把所有事件无条件吞掉。

先写下预期行为，再修改代码、重新构建，最后对照实际结果。程序没有报错不等于逻辑已经正确，尤其要测试取消、空输入、重复操作和失败分支。

## 自测与答案

**过滤器能保存传入的 event 指针以后再用吗？**

不能假定它仍存活。需要延后处理时先复制所需数据，例如 key、文本或坐标，再通过信号或排队调用传递这些值。

## 继续阅读

- [Qt 官方文档](https://doc.qt.io/qt-6/eventsandfilters.html)
- [项目构建方法](../getting-started.md)
- [自动检查范围](../validation.md)
