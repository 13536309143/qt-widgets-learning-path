# 33 状态栏和停靠面板

这是独立可运行的 Qt Creator Widgets 项目，按统一模板组织。打开 [33_status_dock.pro](33_status_dock.pro)，选择 Kit 后构建运行。

## 阅读顺序

1. [mainwindow.ui](mainwindow.ui)：双击进入设计模式，先观察控件与布局。
2. [mainwindow.h](mainwindow.h)：窗口类、ui 指针和成员声明。
3. [mainwindow.cpp](mainwindow.cpp)：setupUi、连接与本步逻辑。
4. [main.cpp](main.cpp)：只负责启动窗口与事件循环。

## 在 Designer 中重做界面

从空白 Widgets 模板开始时，按下表拖入控件并设置相同的 objectName。已有完整 .ui 可直接使用；geometry 只适用于没有布局管理的控件。

### mainwindow.ui

| objectName | 类型 | 主要设置 |
|---|---|---|
| `MainWindow` | QMainWindow | geometry=(0,0,800,580) |
| `centralwidget` | QWidget | 按 .ui 中的默认属性 |
| `textEdit` | QTextEdit | plainText=这是主编辑区域 |
| `statusbar` | QStatusBar | 按 .ui 中的默认属性 |
| `dockWidget` | QDockWidget | 按 .ui 中的默认属性 |
| `dockWidgetContents` | QWidget | 按 .ui 中的默认属性 |
| `documentList` | QListWidget | 按 .ui 中的默认属性 |
| `verticalLayout` | QVBoxLayout | 布局内的控件交给布局分配空间 |
| `dockLayout` | QVBoxLayout | 布局内的控件交给布局分配空间 |

## 完整文件

- [33_status_dock.pro](33_status_dock.pro)
- [main.cpp](main.cpp)
- [mainwindow.cpp](mainwindow.cpp)
- [mainwindow.h](mainwindow.h)
- [mainwindow.ui](mainwindow.ui)
- [verify.h](verify.h)

verify.h 仅在传入 --verify 时执行运行检查，正常学习可以先忽略。

![运行截图 03_result](screenshots/03_result.png)

[统一模板说明](../../docs/project-format.md) · [安装与运行](../../docs/getting-started.md)
