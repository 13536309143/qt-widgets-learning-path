# 02 标题与窗口大小

这是独立可运行的 Qt Creator Widgets 项目，按统一模板组织。打开 [02_window_size.pro](02_window_size.pro)，选择 Kit 后构建运行。

## 阅读顺序

1. [widget.ui](widget.ui)：双击进入设计模式，先观察控件与布局。
2. [widget.h](widget.h)：窗口类、ui 指针和成员声明。
3. [widget.cpp](widget.cpp)：setupUi、连接与本步逻辑。
4. [main.cpp](main.cpp)：只负责启动窗口与事件循环。

## 在 Designer 中重做界面

从空白 Widgets 模板开始时，按下表拖入控件并设置相同的 objectName。已有完整 .ui 可直接使用；geometry 只适用于没有布局管理的控件。

### widget.ui

| objectName | 类型 | 主要设置 |
|---|---|---|
| `Widget` | QWidget | geometry=(0,0,720,540) |
| `headingLabel` | QLabel | geometry=(24,18,670,32)；text=02 标题与窗口大小 |
| `outputEdit` | QPlainTextEdit | geometry=(24,140,670,370)；readOnly=true |
| `runButton` | QPushButton | geometry=(24,65,220,40)；text=固定为 720 × 540 |

## 完整文件

- [02_window_size.pro](02_window_size.pro)
- [main.cpp](main.cpp)
- [verify.h](verify.h)
- [widget.cpp](widget.cpp)
- [widget.h](widget.h)
- [widget.ui](widget.ui)

verify.h 仅在传入 --verify 时执行运行检查，正常学习可以先忽略。

![运行截图 02_after_clicks](screenshots/02_after_clicks.png)

[统一模板说明](../../docs/project-format.md) · [安装与运行](../../docs/getting-started.md)
