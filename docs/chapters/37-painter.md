# 第 37 章 QPainter 与绘图设备

本章项目：[完整源码](../../examples/48_painter)。先运行项目观察行为，再按以下顺序阅读。

## 本章要解决的问题

QPainter 执行画线、矩形、椭圆、图片和文本等操作；QPaintDevice 是可被绘制的设备基类；QPaintEngine 提供设备对应的绘制实现，入门通常不用直接操作。原笔记中的 QPainterDevice 应改为 QPaintDevice。

对 QWidget 绘制应在 paintEvent 或其调用链中完成。先创建 QPainter(this)，设置 QPen 控制边线颜色宽度，设置 QBrush 控制填充，再调用 drawLine、drawRect、drawEllipse。要改变图形，更新成员状态后调用 update() 请求重新绘制，而不是把绘图只放进按钮槽临时画一次。

QPixmap 常用于界面显示，QImage 便于像素访问，QBitmap 是一位深度位图，QPicture 记录与重放绘图命令。它们不是全部绘图设备，QWidget 也是 QPaintDevice 的子类。

## 图形应该从持久状态重建

绘制结果是窗口某一时刻的像素，不是可靠的应用状态。遮挡、缩放、主题变化等都可能触发重绘。把“画一个红椭圆”只写在按钮槽里而不保留颜色状态，下一次重绘可能消失。第 52 步将 color_ 放在 DrawingArea 成员中，按钮只更新颜色并 update，paintEvent 每次根据状态重建图形。

## 按执行顺序拆解

mainwindow.ui 中 QWidget 已提升为 Canvas；mainwindow.cpp 只负责装载界面和状态提示。绘制逻辑独立在 canvas.cpp 的 paintEvent：创建局部 QPainter、设置画笔和画刷、绘制形状与文字，再绘制离屏 QImage。第 52 步继续让绘制由成员状态驱动。

## 参数与行为边界

QWidget 是屏幕控件，QImage 便于像素操作，QPixmap 为界面显示优化，QBitmap 是一位位图，QPicture 记录绘制命令。逻辑坐标、像素坐标与设备像素比需要区分。painter.save/restore 可隔离画笔与坐标变换状态，复杂绘制中比反复手动恢复属性更可靠。

## 易错点与排查

QPicture 不是普通像素图片。QImage、QPixmap 上的离屏绘制可以在适当代码位置进行，不要求它们自己的 paintEvent。一个设备同时只应有一个活动画家；使用局部 QPainter 作用域可自动结束绘制。

遇到问题时先区分三个层次：编译错误检查声明、头文件与构建配置；界面无反应检查连接、接收对象与生存期；运行结果不对检查输入、返回值和分支是否真正执行。可以先在连接入口记录一条日志，再在结果处理处记录一条，定位中间哪一步没有到达。

## 完整示例与配套文件

本步按统一模板拆分文件。先打开 .ui 观察界面，再读窗口类；[项目 README](../../examples/48_painter/README.md) 提供逐项 Designer 设置。

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
    ui->statusbar->showMessage("红矩形 蓝椭圆 灰直线；下方是 QImage 离屏绘图。");

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
   <string>48 QPainter 与绘图设备</string>
  </property>
  <widget class="Canvas" name="canvas" />
  <widget class="QStatusBar" name="statusbar" />
 </widget>
 <customwidgets>
  <customwidget>
   <class>Canvas</class>
   <extends>QWidget</extends>
   <header>canvas.h</header>
   <container>1</container>
  </customwidget>
 </customwidgets>
 <resources />
 <connections />
</ui>
```

### canvas.cpp

```cpp
#include "canvas.h"
#include <QPainter>

Canvas::Canvas(QWidget *parent)
    : QWidget(parent)
{
}

void Canvas::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), Qt::white);
    painter.setPen(QPen(Qt::darkGray, 2));
    painter.drawLine(35, 35, 500, 270);
    painter.setPen(QPen(Qt::red, 3));
    painter.drawRect(60, 70, 360, 180);
    painter.setPen(QPen(Qt::blue, 5));
    painter.drawEllipse(60, 70, 360, 180);
    painter.setPen(Qt::black);
    painter.drawText(50, 320, "QPainter 在 paintEvent 中绘制，缩放和遮挡后仍可重绘。");
    QImage image(180, 80, QImage::Format_ARGB32);
    image.fill(QColor("#dbeafe"));
    {
        QPainter imagePainter(&image);
        imagePainter.setPen(Qt::darkBlue);
        imagePainter.drawText(image.rect(), Qt::AlignCenter, "QImage 离屏绘图");
    }
    painter.drawImage(50, 360, image);
}
```

### canvas.h

```cpp
#ifndef CANVAS_H
#define CANVAS_H
#include <QWidget>
class Canvas : public QWidget
{
    Q_OBJECT
public:
    explicit Canvas(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
};
#endif // CANVAS_H
```

### 48_painter.pro

```qmake
QT += widgets

CONFIG += c++17

#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

SOURCES += \
    canvas.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    canvas.h \
    mainwindow.h \
    verify.h

FORMS += \
    mainwindow.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
```

[verify.h](../../examples/48_painter/verify.h) 是可选运行检查，普通启动不会执行。构建期生成的 ui_*.h 和 moc 文件不手写。

## 运行截图与观察

![QPainter 与绘图设备 03_result](../../examples/48_painter/screenshots/03_result.png)

## 自己动手

运行第 52 步，切换颜色后缩放窗口并遮挡再恢复，确认颜色仍保持。把椭圆位置从固定 QRect 改为基于当前 rect 调整后的区域。再把画刷改为 NoBrush，比较只画轮廓与填充的区别。

先写下预期行为，再修改代码、重新构建，最后对照实际结果。程序没有报错不等于逻辑已经正确，尤其要测试取消、空输入、重复操作和失败分支。

## 自测与答案

**为什么 QImage 上绘制不需要 paintEvent？**

paintEvent 是 QWidget 的屏幕重绘入口。离屏图像本身是绘图设备，可以在适当函数中创建局部 QPainter 绘制；结束后再交给 QLabel 或其他控件显示。

## 继续阅读

- [Qt 官方文档](https://doc.qt.io/qt-6/qpainter.html)
- [项目构建方法](../getting-started.md)
- [自动检查范围](../validation.md)
