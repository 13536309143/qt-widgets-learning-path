# 示例 13 定义与调用的区别

归属：[第五章 信号和槽机制（Lambda）](../../docs/chapters/05-signals-slots-lambda.md)。

这是独立可运行的 Qt Creator Widgets 项目，按统一模板组织。打开 [13_define_and_call.pro](13_define_and_call.pro)，选择 Kit 后构建运行。

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
| `headingLabel` | QLabel | geometry=(24,18,670,32)；text=13 定义与调用的区别 |
| `outputEdit` | QPlainTextEdit | geometry=(24,140,670,370)；readOnly=true |

## 完整文件

- [13_define_and_call.pro](13_define_and_call.pro)
- [main.cpp](main.cpp)
- [verify.h](verify.h)
- [widget.cpp](widget.cpp)
- [widget.h](widget.h)
- [widget.ui](widget.ui)

verify.h 仅在传入 --verify 时执行运行检查，正常学习可以先忽略。

![运行截图 01_initial](screenshots/01_initial.png)

[统一模板说明](../../docs/project-format.md) · [安装与运行](../../docs/getting-started.md)
