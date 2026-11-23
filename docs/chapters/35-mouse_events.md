# 第 35 章 鼠标按键 坐标和跟踪

本章项目：[完整源码](../../examples/46_mouse_events)。先运行项目观察行为，再按以下顺序阅读。

## 本章要解决的问题

mousePressEvent 处理按下，mouseReleaseEvent 处理释放，mouseMoveEvent 处理移动。button() 描述本次发生变化的按键；buttons() 是当前仍按下的按键集合，例如移动时判断是否正在拖动要查看 buttons()。

默认不按键移动时不一定收到移动事件，setMouseTracking(true) 允许无按键移动跟踪。Qt 6 推荐 position() 获取相对接收控件的浮点坐标，用 globalPosition() 获取屏幕坐标。坐标属于接收事件的控件，不是任意父窗口。

## 本次变化的按键和当前按下集合

鼠标 button() 表示触发本次按下或释放的按键，buttons() 表示当前仍保持按下的集合。mouseMoveEvent 的 button 常是 NoButton，因此拖拽判断应使用 buttons() & Qt::LeftButton。position 是相对接收控件的坐标，globalPosition 是屏幕坐标，二者不能混用。

## 按执行顺序拆解

mainwindow.ui 将中央 QLabel 提升为 EventArea。eventarea.cpp 中启用 mouseTracking，重写 mouseMoveEvent 和 mousePressEvent，读取 position/button 并设置焦点。只保留本步鼠标处理，键盘事件的实现留到下一步。

## 参数与行为边界

默认把某些事件交给基类处理，有助于保留原控件行为。事件传播不是任意祖先都会收到一份副本；是否处理、是否接受、控件默认行为都会影响链路。需要观察已有子控件而不改继承时，下一节的事件过滤器更合适。

## 易错点与排查

鼠标位于某个子控件上时，事件通常发给那个子控件。若想观察 QLabel 上的事件，应继承 QLabel 重写，而不是只在主窗口上重写后期待捕获所有子控件事件。

遇到问题时先区分三个层次：编译错误检查声明、头文件与构建配置；界面无反应检查连接、接收对象与生存期；运行结果不对检查输入、返回值和分支是否真正执行。可以先在连接入口记录一条日志，再在结果处理处记录一条，定位中间哪一步没有到达。

## 完整示例与配套文件

本步按统一模板拆分文件。先打开 .ui 观察界面，再读窗口类；[项目 README](../../examples/46_mouse_events/README.md) 提供逐项 Designer 设置。

### main.cpp

```cpp
#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return QApplication::exec();
}
```

### mainwindow.h

```cpp
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
```

### mainwindow.cpp

```cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "verify.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->statusbar->showMessage("本步用 QLabel 子类显示鼠标位置，Qt 6 使用 position()。");

    // 仅带 --verify 参数时安排检查；正常学习时不会执行。
    scheduleVerification(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
```

### mainwindow.ui

```xml
<?xml version="1.0" encoding="UTF-8"?>
<ui version="4.0">
 <class>MainWindow</class>
 <widget class="QMainWindow" name="MainWindow">
  <property name="geometry">
   <rect>
    <x>0</x>
    <y>0</y>
    <width>800</width>
    <height>580</height>
   </rect>
  </property>
  <property name="windowTitle">
   <string>46 鼠标按键 坐标和跟踪</string>
  </property>
  <widget class="EventArea" name="eventArea" />
  <widget class="QStatusBar" name="statusbar" />
 </widget>
 <customwidgets>
  <customwidget>
   <class>EventArea</class>
   <extends>QLabel</extends>
   <header>eventarea.h</header>
  </customwidget>
 </customwidgets>
 <resources />
 <connections />
</ui>
```

### eventarea.cpp

```cpp
#include "eventarea.h"
#include <QMouseEvent>
#include <QResizeEvent>
#include <QKeyEvent>

EventArea::EventArea(QWidget *parent)
    : QLabel(parent)
{
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    setAlignment(Qt::AlignCenter);
    setText("移动鼠标或按下鼠标键，观察位置和按键。");
}

void EventArea::mouseMoveEvent(QMouseEvent *event)
{
    setText(QString("鼠标位置 %1，%2").arg(event->position().x()).arg(event->position().y()));
    QLabel::mouseMoveEvent(event);
}

void EventArea::mousePressEvent(QMouseEvent *event)
{
    setFocus();
    setText(event->button() == Qt::LeftButton ? "左键按下" : "其他鼠标键按下");
    QLabel::mousePressEvent(event);
}
```

### eventarea.h

```cpp
#ifndef EVENTAREA_H
#define EVENTAREA_H
#include <QLabel>
class EventArea : public QLabel
{
    Q_OBJECT
public:
    explicit EventArea(QWidget *parent = nullptr);

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
};
#endif // EVENTAREA_H
```

### 46_mouse_events.pro

```qmake
QT += widgets

CONFIG += c++17

#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

SOURCES += \
    eventarea.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    eventarea.h \
    mainwindow.h \
    verify.h

FORMS += \
    mainwindow.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
```

[verify.h](../../examples/46_mouse_events/verify.h) 是可选运行检查，普通启动不会执行。构建期生成的 ui_*.h 和 moc 文件不手写。

## 运行截图与观察

![鼠标按键 坐标和跟踪 03_result](../../examples/46_mouse_events/screenshots/03_result.png)

## 自己动手

在 mouseMoveEvent 中只有左键拖动时显示“拖动”，无按键时显示“悬停”。用 position 记录相对位置，再把标签放进带边距的布局中，确认坐标原点跟随标签而非主窗口。

先写下预期行为，再修改代码、重新构建，最后对照实际结果。程序没有报错不等于逻辑已经正确，尤其要测试取消、空输入、重复操作和失败分支。

## 自测与答案

**Qt 5 示例中的 x/y 可以照搬吗？**

部分旧接口仍可能存在但不适合新代码。Qt 6 使用 position 的浮点位置更准确，必要时再有意识转换成整数 QPoint。

## 继续阅读

- [Qt 官方文档](https://doc.qt.io/qt-6/qmouseevent.html)
- [项目构建方法](../getting-started.md)
- [自动检查范围](../validation.md)
