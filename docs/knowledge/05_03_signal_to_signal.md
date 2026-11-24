# 5.3 信号转发

[所属章节](../chapters/05-signals-slots-lambda.md) · [完整项目](../../examples/05_03_signal_to_signal/README.md)

### 本项目要掌握什么

信号转发。这一节只围绕这个核心问题阅读和修改程序。

### 关键代码在哪里

关键片段 1：[ widget.cpp 第 13 行起](../../examples/05_03_signal_to_signal/widget.cpp#L13)。以下代码从本项目源文件提取，完整上下文见源文件。

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

![项目 5.3 运行结果](../../examples/05_03_signal_to_signal/screenshots/02_after_clicks.png)

### 完整项目与界面文件

- [05_03_signal_to_signal.pro](../../examples/05_03_signal_to_signal/05_03_signal_to_signal.pro)
- [main.cpp](../../examples/05_03_signal_to_signal/main.cpp)
- [student.h](../../examples/05_03_signal_to_signal/student.h)
- [teacher.h](../../examples/05_03_signal_to_signal/teacher.h)
- [verify.h](../../examples/05_03_signal_to_signal/verify.h)
- [widget.cpp](../../examples/05_03_signal_to_signal/widget.cpp)
- [widget.h](../../examples/05_03_signal_to_signal/widget.h)
- [widget.ui](../../examples/05_03_signal_to_signal/widget.ui)

打开本项目 .pro，选择匹配的 Kit，构建运行；修改界面请打开 .ui，保存后重新构建。verify.h 只在传入 --verify 时执行检查。
