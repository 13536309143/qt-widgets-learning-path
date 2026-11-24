# 5.4 选择带参数的重载

[所属章节](../chapters/05-signals-slots-lambda.md) · [完整项目](../../examples/05_04_overloaded_signal/README.md)

### 本项目要掌握什么

选择带参数的重载。这一节只围绕这个核心问题阅读和修改程序。

### 关键代码在哪里

关键片段 1：[ widget.cpp 第 13 行起](../../examples/05_04_overloaded_signal/widget.cpp#L13)。以下代码从本项目源文件提取，完整上下文见源文件。

```cpp
auto *teacher = new Teacher(this);
auto *student = new Student(this);
connect(student, &Student::message, this, &Widget::writeLine);
connect(teacher, qOverload<const QString &>(&Teacher::hungry), student,
        qOverload<const QString &>(&Student::treat));
connect(ui->runButton, &QPushButton::clicked, this,
        [teacher]() { teacher->requestFood(QStringLiteral("红烧排骨")); });
```

### 逐段讲解

hungry 和 treat 同名但签名不同，qOverload<const QString &> 明确选中接收菜名的版本，模板参数是类型而不是实参。

按钮给出的是 bool，Lambda 忽略它，调用 requestFood 并传入 QStringLiteral 菜名；随后 hungry(food) 把数据交给 treat(food)。

### 按执行顺序阅读

构造函数先准备界面与连接；交互发生后才进入回调。阅读时按“发送对象 → 信号或调用 → 回调里的数据 → 被修改对象 → 显示结果”追踪，不要把建立连接当成已经执行操作。

### 容易出错的地方与动手验证

改变菜名并追踪输出；不要把 bool 当作 QString 参数硬接，也不要省略有歧义的重载选择。

先写下预测，再修改一个条件重新构建。记录实际结果，并用本节代码中的变量、对象和调用顺序解释原因；仅仅没有报错不代表已经验证了所有分支。

### 运行结果

![项目 5.4 运行结果](../../examples/05_04_overloaded_signal/screenshots/02_after_clicks.png)

### 完整项目与界面文件

- [05_04_overloaded_signal.pro](../../examples/05_04_overloaded_signal/05_04_overloaded_signal.pro)
- [main.cpp](../../examples/05_04_overloaded_signal/main.cpp)
- [student.h](../../examples/05_04_overloaded_signal/student.h)
- [teacher.h](../../examples/05_04_overloaded_signal/teacher.h)
- [verify.h](../../examples/05_04_overloaded_signal/verify.h)
- [widget.cpp](../../examples/05_04_overloaded_signal/widget.cpp)
- [widget.h](../../examples/05_04_overloaded_signal/widget.h)
- [widget.ui](../../examples/05_04_overloaded_signal/widget.ui)

打开本项目 .pro，选择匹配的 Kit，构建运行；修改界面请打开 .ui，保存后重新构建。verify.h 只在传入 --verify 时执行检查。
