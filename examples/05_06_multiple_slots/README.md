# 5.6 一个信号连接多个处理函数

[教程中的本知识点](../../docs/knowledge/05_06_multiple_slots.md) · [第 5 章](../../docs/chapters/05-signals-slots-lambda.md)

### 本项目要掌握什么

一个信号连接多个处理函数。这一节只围绕这个核心问题阅读和修改程序。

### 关键代码在哪里

关键片段 1：[ widget.cpp 第 11 行起](widget.cpp#L11)。以下代码从本项目源文件提取，完整上下文见源文件。

```cpp
connect(ui->runButton, &QPushButton::clicked, this, [this]() { writeLine("1 第一个连接"); });
connect(ui->runButton, &QPushButton::clicked, this, [this]() { writeLine("2 第二个连接"); });
```

### 逐段讲解

两个 connect 都监听同一个按钮，每次 clicked 会依次进入两段处理代码。在本例同线程直接连接中，执行顺序与连接建立顺序一致。

这里是一次通知的两个订阅者，不是按钮被按了两次；若无意中重复执行初始化连接，也会出现一次点击重复输出。

### 按执行顺序阅读

构造函数先准备界面与连接；交互发生后才进入回调。阅读时按“发送对象 → 信号或调用 → 回调里的数据 → 被修改对象 → 显示结果”追踪，不要把建立连接当成已经执行操作。

### 容易出错的地方与动手验证

调换两条连接的位置观察输出；不要把这一顺序推导成跨线程任务完成顺序。

先写下预测，再修改一个条件重新构建。记录实际结果，并用本节代码中的变量、对象和调用顺序解释原因；仅仅没有报错不代表已经验证了所有分支。

### 运行结果

![项目 5.6 运行结果](screenshots/02_after_clicks.png)

### 完整项目与界面文件

- [05_06_multiple_slots.pro](05_06_multiple_slots.pro)
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
| `headingLabel` | QLabel | geometry=(24,18,670,32)；text=11 一个信号连接多个处理函数 |
| `outputEdit` | QPlainTextEdit | geometry=(24,140,670,370)；readOnly=true |
| `runButton` | QPushButton | geometry=(24,65,250,40)；text=依次执行两个处理函数 |
