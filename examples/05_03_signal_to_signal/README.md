# 5.3 信号转发

[教程中的本知识点](../../docs/knowledge/05_03_signal_to_signal.md) · [第 5 章](../../docs/chapters/05-signals-slots-lambda.md)

### 本项目要掌握什么

信号转发。这一节只围绕这个核心问题阅读和修改程序。

### 关键代码在哪里

关键片段 1：[ widget.cpp 第 13 行起](widget.cpp#L13)。以下代码从本项目源文件提取，完整上下文见源文件。

```cpp
auto *teacher = new Teacher(this);
auto *student = new Student(this);
connect(student, &Student::message, this, &Widget::writeLine);
connect(teacher, qOverload<>(&Teacher::hungry), student, qOverload<>(&Student::treat));
connect(ui->runButton, &QPushButton::clicked, teacher, qOverload<>(&Teacher::hungry));
```

### 逐段讲解

按钮的 clicked 直接连接 Teacher 的 hungry 信号，而 hungry 仍连接 Student::treat。目标是另一个信号，也可以成为通知链的一环。

它省去了 finishClass 这个中转函数，但 Teacher 与 Student 的角色没有交换；真正的输出仍由 message 到窗口的连接完成。

### 按执行顺序阅读

构造函数先准备界面与连接；交互发生后才进入回调。阅读时按“发送对象 → 信号或调用 → 回调里的数据 → 被修改对象 → 显示结果”追踪，不要把建立连接当成已经执行操作。

### 容易出错的地方与动手验证

与上一个项目逐行对比，指出唯一变化的按钮连接；不要为了转发再重复连接同一条链。

先写下预测，再修改一个条件重新构建。记录实际结果，并用本节代码中的变量、对象和调用顺序解释原因；仅仅没有报错不代表已经验证了所有分支。

### 运行结果

![项目 5.3 运行结果](screenshots/02_after_clicks.png)

### 完整项目与界面文件

- [05_03_signal_to_signal.pro](05_03_signal_to_signal.pro)
- [main.cpp](main.cpp)
- [student.h](student.h)
- [teacher.h](teacher.h)
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
| `headingLabel` | QLabel | geometry=(24,18,670,32)；text=08 信号连接另一个信号 |
| `outputEdit` | QPlainTextEdit | geometry=(24,140,670,370)；readOnly=true |
| `runButton` | QPushButton | geometry=(24,65,250,40)；text=下课 请老师吃饭 |
