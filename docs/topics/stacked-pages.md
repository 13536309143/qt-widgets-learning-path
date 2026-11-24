# 9.4 堆叠页面与窗口切换

对应示例：[完整源码](../../examples/44_stacked_pages)。先运行项目观察行为，再按以下顺序阅读。

## 本章要解决的问题

QStackedWidget 在同一容器里管理多个页面，一次显示其中一页。addWidget 返回该页索引；setCurrentIndex(0) 切到第一页，索引从 0 开始，也可用 setCurrentWidget 按指针切换。切换不会删除其他页面，输入状态通常仍保留。

这与 hide 一个顶层窗口、show 另一个顶层窗口不同。用堆叠控件实现“登录页面—主页”能够共享同一个顶层窗口，方便统一管理退出和生命期。多窗口方案也可用，但要管理返回路径、对象所有权，以及隐藏主窗口后子窗口关闭导致程序无可见窗口的情况。

## 页面属于一个窗口 生命周期仍然存在

QStackedWidget 在视觉上一次显示一页，但其他页面的对象通常仍存在，因此输入状态也可保留。这适合向导、设置页和简单导航。切换页面不会自动执行登录验证，也不会自动清除敏感信息或刷新数据；这些都需要明确的进入与退出逻辑。

## 按执行顺序拆解

mainwindow.ui 的 pages 包含 loginPage 与 homePage，两个按钮分别位于各自页面。mainwindow.cpp 只连接导航按钮与 setCurrentIndex。页面对象保持存活，切换时不会自动清除控件状态。

## 参数与行为边界

currentIndex 从零开始，空容器可能没有有效当前页。setCurrentWidget 比硬编码数字更能表达意图，但仍要求页面已经加入。removeWidget 不等于 delete：移除后对象是否继续使用、重新设 parent 还是释放，需要自己决定。

## 易错点与排查

改变当前页不等于验证了身份。此例只演示页面导航，没有账号认证。每页有自己的布局，页面必须先 addWidget 再切换到相应索引。

遇到问题时先区分三个层次：编译错误检查声明、头文件与构建配置；界面无反应检查连接、接收对象与生存期；运行结果不对检查输入、返回值和分支是否真正执行。可以先在连接入口记录一条日志，再在结果处理处记录一条，定位中间哪一步没有到达。

## 完整示例与配套文件

本步按统一模板拆分文件。先打开 .ui 观察界面，再读窗口类；[项目 README](../../examples/44_stacked_pages/README.md) 提供逐项 Designer 设置。

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
    connect(ui->run, &QPushButton::clicked, this, [this]() { ui->pages->setCurrentIndex(1); });
    connect(ui->back, &QPushButton::clicked, this, [this]() { ui->pages->setCurrentIndex(0); });

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
   <string>44 堆叠页面与窗口切换</string>
  </property>
  <widget class="QWidget" name="centralwidget">
   <layout class="QVBoxLayout" name="verticalLayout">
    <item>
     <widget class="QStackedWidget" name="pages">
      <property name="currentIndex">
       <number>0</number>
      </property>
      <widget class="QWidget" name="loginPage">
       <layout class="QVBoxLayout" name="loginPageLayout">
        <item>
         <widget class="QPushButton" name="run">
          <property name="text">
           <string>进入主页（仅演示导航）</string>
          </property>
         </widget>
        </item>
       </layout>
      </widget>
      <widget class="QWidget" name="homePage">
       <layout class="QVBoxLayout" name="homePageLayout">
        <item>
         <widget class="QPushButton" name="back">
          <property name="text">
           <string>返回登录页</string>
          </property>
         </widget>
        </item>
       </layout>
      </widget>
     </widget>
    </item>
   </layout>
  </widget>
 </widget>
 <resources />
 <connections />
</ui>
```

### 44_stacked_pages.pro

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

[verify.h](../../examples/44_stacked_pages/verify.h) 是可选运行检查，普通启动不会执行。构建期生成的 ui_*.h 和 moc 文件不手写。

## 运行截图与观察

![堆叠页面与窗口切换 03_result](../../examples/44_stacked_pages/screenshots/03_result.png)

## 自己动手

增加第三页“关于”，提供返回按钮。让第一页有一个 QLineEdit，切走再回来观察内容是否保持。再为切换后“清空输入”写一段明确代码，理解状态变化与显示切换并非自动绑定。

先写下预期行为，再修改代码、重新构建，最后对照实际结果。程序没有报错不等于逻辑已经正确，尤其要测试取消、空输入、重复操作和失败分支。

## 自测与答案

**什么时候应该使用多个顶层窗口？**

当任务确实需要独立并行工作区时可以选择多窗口；同时要设计返回与退出路径，避免主窗口隐藏后只剩已经关闭的子窗口。简单内部导航通常先用堆叠页面。

## 继续阅读

- [Qt 官方文档](https://doc.qt.io/qt-6/qstackedwidget.html)
- [项目构建方法](../getting-started.md)
- [自动检查范围](../validation.md)
