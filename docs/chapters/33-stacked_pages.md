# 第 33 章 堆叠页面与窗口切换

本章项目：[完整源码](../../examples/44_stacked_pages)。先运行项目观察行为，再按以下顺序阅读。

## 本章要解决的问题

QStackedWidget 在同一容器里管理多个页面，一次显示其中一页。addWidget 返回该页索引；setCurrentIndex(0) 切到第一页，索引从 0 开始，也可用 setCurrentWidget 按指针切换。切换不会删除其他页面，输入状态通常仍保留。

这与 hide 一个顶层窗口、show 另一个顶层窗口不同。用堆叠控件实现“登录页面—主页”能够共享同一个顶层窗口，方便统一管理退出和生命期。多窗口方案也可用，但要管理返回路径、对象所有权，以及隐藏主窗口后子窗口关闭导致程序无可见窗口的情况。

## 页面属于一个窗口 生命周期仍然存在

QStackedWidget 在视觉上一次显示一页，但其他页面的对象通常仍存在，因此输入状态也可保留。这适合向导、设置页和简单导航。切换页面不会自动执行登录验证，也不会自动清除敏感信息或刷新数据；这些都需要明确的进入与退出逻辑。

## 按执行顺序拆解

第 44 步用两个按钮充当最小页面，便于看清索引。先 addWidget，后 connect 切换。学习后可以把按钮换成各自具有布局的 QWidget 页面，再把导航按钮留在容器外，保持导航与页面内容的职责分离。

## 参数与行为边界

currentIndex 从零开始，空容器可能没有有效当前页。setCurrentWidget 比硬编码数字更能表达意图，但仍要求页面已经加入。removeWidget 不等于 delete：移除后对象是否继续使用、重新设 parent 还是释放，需要自己决定。

## 易错点与排查

改变当前页不等于验证了身份。此例只演示页面导航，没有账号认证。每页有自己的布局，页面必须先 addWidget 再切换到相应索引。

遇到问题时先区分三个层次：编译错误检查声明、头文件与构建配置；界面无反应检查连接、接收对象与生存期；运行结果不对检查输入、返回值和分支是否真正执行。可以先在连接入口记录一条日志，再在结果处理处记录一条，定位中间哪一步没有到达。

## 完整示例与配套文件

以下为本步完整程序。若存在 .ui、qrc 或额外头文件，必须把同一目录中的文件一起下载。构建期生成的 ui_*.h、moc 文件不需要手动维护。

### main.cpp

```cpp
#include <QtWidgets>
#include <QMouseEvent>
#include <QKeyEvent>

#include "verify.h"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QMainWindow w;
    w.resize(800, 580);
    w.setWindowTitle("44 堆叠页面与窗口切换");
    auto *stack = new QStackedWidget;
    stack->setObjectName("pages");
    auto *login = new QPushButton("进入主页（仅演示导航）");
    login->setObjectName("run");
    auto *home = new QPushButton("返回登录页");
    home->setObjectName("back");
    stack->addWidget(login);
    stack->addWidget(home);
    w.setCentralWidget(stack);
    QObject::connect(login, &QPushButton::clicked, stack, [stack]() { stack->setCurrentIndex(1); });
    QObject::connect(home, &QPushButton::clicked, stack, [stack]() { stack->setCurrentIndex(0); });
    w.show();
    scheduleVerification(&w);
    return app.exec();
}

```

### 44_stacked_pages.pro

```qmake
QT += widgets
CONFIG += c++17
TEMPLATE = app
TARGET = 44_stacked_pages
SOURCES += main.cpp
HEADERS += verify.h

```

自动检查用的 verify.h 已在项目目录中提供；普通运行没有 --verify 参数时，不会执行自动操作。

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
