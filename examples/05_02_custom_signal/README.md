# 5.2 自定义信号槽的调用链

[教程中的本知识点](../../docs/knowledge/05_02_custom_signal.md) · [第 5 章](../../docs/chapters/05-signals-slots-lambda.md)

### 本项目要掌握什么

自定义信号槽的调用链。这一节只围绕这个核心问题阅读和修改程序。

### 关键代码在哪里

关键片段 1：[ widget.cpp 第 13 行起](widget.cpp#L13)。以下代码从本项目源文件提取，完整上下文见源文件。

```cpp
auto *teacher = new Teacher(this);
auto *student = new Student(this);
connect(student, &Student::message, this, &Widget::writeLine);
connect(teacher, qOverload<>(&Teacher::hungry), student, qOverload<>(&Student::treat));
connect(ui->runButton, &QPushButton::clicked, teacher, &Teacher::finishClass);
```

### 逐段讲解

Teacher、Student 都有 Q_OBJECT，hungry 只声明信号，treat 是处理函数；窗口把 message 接到 writeLine 显示结果。

点击按钮先调用 finishClass，函数内 emit hungry，再进入 student 的 treat，最后显示 message。qOverload<> 指明无参数重载。

### 按执行顺序阅读

构造函数先准备界面与连接；交互发生后才进入回调。阅读时按“发送对象 → 信号或调用 → 回调里的数据 → 被修改对象 → 显示结果”追踪，不要把建立连接当成已经执行操作。

### 容易出错的地方与动手验证

按调用顺序画箭头，再暂时断开中间连接，确认哪一步停止；信号声明不需要自己写普通函数体。

先写下预测，再修改一个条件重新构建。记录实际结果，并用本节代码中的变量、对象和调用顺序解释原因；仅仅没有报错不代表已经验证了所有分支。

### 运行结果

![项目 5.2 运行结果](screenshots/02_after_clicks.png)

### 完整项目与界面文件

- [05_02_custom_signal.pro](05_02_custom_signal.pro)
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
| `headingLabel` | QLabel | geometry=(24,18,670,32)；text=07 自定义信号与槽 |
| `outputEdit` | QPlainTextEdit | geometry=(24,140,670,370)；readOnly=true |
| `runButton` | QPushButton | geometry=(24,65,250,40)；text=下课 请老师吃饭 |
