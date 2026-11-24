# 2.1 应用入口与事件循环

[教程中的本知识点](../../docs/knowledge/02_01_first_window.md) · [第 2 章](../../docs/chapters/02-create-project.md)

### 本项目要掌握什么

应用入口与事件循环。这一节只围绕这个核心问题阅读和修改程序。

### 关键代码在哪里

关键片段 1：[ main.cpp 第 5 行起](main.cpp#L5)。以下代码从本项目源文件提取，完整上下文见源文件。

```cpp
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
    return QApplication::exec();
}
```

### 逐段讲解

QApplication a 必须先于 QWidget 创建，负责 GUI 公共资源。Widget w 的构造函数建立界面，w.show() 负责显示。

QApplication::exec() 开始处理输入、定时器和重绘，通常直到退出才返回；它不是主动执行每一个按钮槽。

### 按执行顺序阅读

先从 main.cpp 创建应用和窗口进入构造函数，再由 setupUi 根据 .ui 建立对象、属性与布局。对照上面的片段定位本节设置；show 后进入事件循环，界面才会持续响应操作。

### 容易出错的地方与动手验证

把 exec 改成 return 0 观察程序迅速结束，再恢复；不能在创建窗口后才创建 QApplication。

先写下预测，再修改一个条件重新构建。记录实际结果，并用本节代码中的变量、对象和调用顺序解释原因；仅仅没有报错不代表已经验证了所有分支。

### 运行结果

![项目 2.1 运行结果](screenshots/01_initial.png)

### 完整项目与界面文件

- [02_01_first_window.pro](02_01_first_window.pro)
- [main.cpp](main.cpp)
- [verify.h](verify.h)
- [widget.cpp](widget.cpp)
- [widget.h](widget.h)
- [widget.ui](widget.ui)

打开本项目 .pro，选择匹配的 Kit，构建运行；修改界面请打开 .ui，保存后重新构建。verify.h 只在传入 --verify 时执行检查。

## 在 Designer 中重做界面

从空白 Widgets 模板开始时，按下表拖入控件并设置相同的 objectName。已有完整 .ui 可直接使用；geometry 只适用于没有布局管理的控件。

### widget.ui

| objectName | 类型 | 主要设置 |
|---|---|---|
| `Widget` | QWidget | geometry=(0,0,720,540) |
