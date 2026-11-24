# 5.17 空捕获回调

[所属章节](../chapters/05-signals-slots-lambda.md) · [完整项目](../../examples/05_17_empty_capture_button/README.md)

### 本项目要掌握什么

空捕获回调。这一节只围绕这个核心问题阅读和修改程序。

### 关键代码在哪里

关键片段 1：[ widget.cpp 第 11 行起](../../examples/05_17_empty_capture_button/widget.cpp#L11)。以下代码从本项目源文件提取，完整上下文见源文件。

```cpp
connect(ui->runButton, &QPushButton::clicked, this,
        []() { qDebug() << "按钮被按下，当前 Lambda 没有捕获外部变量"; });
// 第二条连接只负责把结果显示到窗口中，不影响上面的空捕获示例。
connect(ui->runButton, &QPushButton::clicked, this,
        [this]() { writeLine("按钮被按下；空捕获示例也已向调试输出打印信息。"); });
```

### 逐段讲解

第一条连接使用 []，函数体只向 qDebug 输出固定文字，没有依赖外部局部变量或窗口成员。

第二条 [this] 连接负责窗口显示，是额外观察手段；它没有改变第一条回调的捕获方式，两条连接都会收到 clicked。

### 按执行顺序阅读

构造函数先准备界面与连接；交互发生后才进入回调。阅读时按“发送对象 → 信号或调用 → 回调里的数据 → 被修改对象 → 显示结果”追踪，不要把建立连接当成已经执行操作。

### 容易出错的地方与动手验证

暂时移除第二条连接，到应用程序输出查看第一条日志；不能在 [] 里直接访问 ui。

先写下预测，再修改一个条件重新构建。记录实际结果，并用本节代码中的变量、对象和调用顺序解释原因；仅仅没有报错不代表已经验证了所有分支。

### 运行结果

![项目 5.17 运行结果](../../examples/05_17_empty_capture_button/screenshots/02_after_clicks.png)

### 完整项目与界面文件

- [05_17_empty_capture_button.pro](../../examples/05_17_empty_capture_button/05_17_empty_capture_button.pro)
- [main.cpp](../../examples/05_17_empty_capture_button/main.cpp)
- [verify.h](../../examples/05_17_empty_capture_button/verify.h)
- [widget.cpp](../../examples/05_17_empty_capture_button/widget.cpp)
- [widget.h](../../examples/05_17_empty_capture_button/widget.h)
- [widget.ui](../../examples/05_17_empty_capture_button/widget.ui)

打开本项目 .pro，选择匹配的 Kit，构建运行；修改界面请打开 .ui，保存后重新构建。verify.h 只在传入 --verify 时执行检查。
