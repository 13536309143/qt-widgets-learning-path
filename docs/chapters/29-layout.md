# 第 29 章 水平 垂直 网格与表单布局

本章项目：[完整源码](../../examples/40_layout)。先运行项目观察行为，再按以下顺序阅读。

## 本章要解决的问题

绝对定位要给控件逐个设置坐标和尺寸；布局则描述控件关系，由布局管理器在可用区域改变时重新安排。QHBoxLayout 从左到右，QVBoxLayout 从上到下，QGridLayout 按行列定位，QFormLayout 适合标签与输入框成对排列。

父布局用 addLayout 嵌入子布局，控件用 addWidget 加入。addStretch 放可伸缩空白；setContentsMargins 控制布局与外边界间距；setSpacing 控制相邻项目间距。stretch 是分配额外空间的权重，不是承诺最终宽高严格按比例分配，还会受 sizeHint、最小尺寸和 sizePolicy 约束。

## 布局管理的是空间约束

布局会综合 available space、sizeHint、minimumSize、maximumSize、sizePolicy 和 stretch 来分配空间。它不是简单把窗口按比例切成几个盒子。一个按钮即使 stretch 为 1，也可能由于样式或尺寸策略不愿竖向拉高；多行编辑器通常适合承担更多可伸缩空间。

## 按执行顺序拆解

打开 mainwindow.ui，在 centralwidget 下找到 verticalLayout，内部依次是 formLayout、gridLayout 和 buttonLayout。表单项设置 row/column，网格放置六个按钮，底部水平布局包含伸缩占位和确定/取消按钮。外层边距为 20、spacing 为 14。C++ 不重复创建布局。

## 参数与行为边界

contentsMargins 是容器内缘留白，spacing 是相邻项目间隔，addSpacing 是固定间隔，addStretch 是可伸缩空白。QGridLayout 的 rowSpan 和 columnSpan 允许跨行列。QFormLayout 不等于 QTableWidget：前者安排控件，后者显示可编辑表格数据。

## 易错点与排查

布局不是可见控件。控件加入布局后不要再依赖 move()/resize() 固定摆放。不要给同一 QWidget 安装两个顶层布局；QMainWindow 先设置 centralWidget，再给这个容器安装布局。

遇到问题时先区分三个层次：编译错误检查声明、头文件与构建配置；界面无反应检查连接、接收对象与生存期；运行结果不对检查输入、返回值和分支是否真正执行。可以先在连接入口记录一条日志，再在结果处理处记录一条，定位中间哪一步没有到达。

## 完整示例与配套文件

本步按统一模板拆分文件。先打开 .ui 观察界面，再读窗口类；[项目 README](../../examples/40_layout/README.md) 提供逐项 Designer 设置。

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
    // 四种布局均在 mainwindow.ui 中配置，拖动窗口边缘观察空间分配。

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
   <string>40 水平 垂直 网格与表单布局</string>
  </property>
  <widget class="QWidget" name="centralwidget">
   <layout class="QVBoxLayout" name="verticalLayout">
    <item>
     <layout class="QFormLayout" name="formLayout">
      <item row="0" column="0">
       <widget class="QLabel" name="nameEditLabel">
        <property name="text">
         <string>用户名</string>
        </property>
       </widget>
      </item>
      <item row="0" column="1">
       <widget class="QLineEdit" name="nameEdit" />
      </item>
      <item row="1" column="0">
       <widget class="QLabel" name="noteEditLabel">
        <property name="text">
         <string>备注</string>
        </property>
       </widget>
      </item>
      <item row="1" column="1">
       <widget class="QLineEdit" name="noteEdit" />
      </item>
     </layout>
    </item>
    <item>
     <layout class="QGridLayout" name="gridLayout">
      <item row="0" column="0">
       <widget class="QPushButton" name="number1">
        <property name="text">
         <string>1</string>
        </property>
       </widget>
      </item>
      <item row="0" column="1">
       <widget class="QPushButton" name="number2">
        <property name="text">
         <string>2</string>
        </property>
       </widget>
      </item>
      <item row="0" column="2">
       <widget class="QPushButton" name="number3">
        <property name="text">
         <string>3</string>
        </property>
       </widget>
      </item>
      <item row="1" column="0">
       <widget class="QPushButton" name="number4">
        <property name="text">
         <string>4</string>
        </property>
       </widget>
      </item>
      <item row="1" column="1">
       <widget class="QPushButton" name="number5">
        <property name="text">
         <string>5</string>
        </property>
       </widget>
      </item>
      <item row="1" column="2">
       <widget class="QPushButton" name="number6">
        <property name="text">
         <string>6</string>
        </property>
       </widget>
      </item>
     </layout>
    </item>
    <item>
     <layout class="QHBoxLayout" name="buttonLayout">
      <item>
       <spacer name="horizontalSpacer">
        <property name="orientation">
         <enum>Qt::Horizontal</enum>
        </property>
        <property name="sizeHint" stdset="0">
         <size>
          <width>40</width>
          <height>20</height>
         </size>
        </property>
       </spacer>
      </item>
      <item>
       <widget class="QPushButton" name="okButton">
        <property name="text">
         <string>确定</string>
        </property>
       </widget>
      </item>
      <item>
       <widget class="QPushButton" name="cancelButton">
        <property name="text">
         <string>取消</string>
        </property>
       </widget>
      </item>
     </layout>
    </item>
    <property name="leftMargin">
     <number>20</number>
    </property>
    <property name="topMargin">
     <number>20</number>
    </property>
    <property name="rightMargin">
     <number>20</number>
    </property>
    <property name="bottomMargin">
     <number>20</number>
    </property>
    <property name="spacing">
     <number>14</number>
    </property>
   </layout>
  </widget>
 </widget>
 <resources />
 <connections />
</ui>
```

### 40_layout.pro

```qmake
QT += widgets

CONFIG += c++17

#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h \
    verify.h

FORMS += \
    mainwindow.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
```

[verify.h](../../examples/40_layout/verify.h) 是可选运行检查，普通启动不会执行。构建期生成的 ui_*.h 和 moc 文件不手写。

## 运行截图与观察

![水平 垂直 网格与表单布局 03_result](../../examples/40_layout/screenshots/03_result.png)

## 自己动手

把编辑器放到垂直布局中，设置 addWidget(editor,1)，底部按钮行不设置伸展。把窗口拉宽拉高，观察谁吸收额外空间。再把某个按钮 setFixedSize，检查它如何限制布局，理解固定大小不应滥用。

先写下预期行为，再修改代码、重新构建，最后对照实际结果。程序没有报错不等于逻辑已经正确，尤其要测试取消、空输入、重复操作和失败分支。

## 自测与答案

**一个控件可以同时加入两个布局吗？**

同一个控件只有一个实际父层级与几何位置，不能被两个布局当作两个副本使用。需要重复显示时应创建多个控件实例并同步数据。

## 继续阅读

- [Qt 官方文档](https://doc.qt.io/qt-6/layout.html)
- [项目构建方法](../getting-started.md)
- [自动检查范围](../validation.md)
