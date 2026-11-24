# 3.1 初始大小与固定大小

[所属章节](../chapters/03-window-properties.md) · [完整项目](../../examples/03_01_window_size/README.md)

### 本项目要掌握什么

初始大小与固定大小。这一节只围绕这个核心问题阅读和修改程序。

### 关键代码在哪里

关键片段 1：[ widget.cpp 第 11 行起](../../examples/03_01_window_size/widget.cpp#L11)。以下代码从本项目源文件提取，完整上下文见源文件。

```cpp
connect(ui->runButton, &QPushButton::clicked, this,
        [this]()
        {
            setFixedSize(720, 540);
            writeLine("已固定大小，不能再拖动改变窗口尺寸。");
        });
writeLine("当前使用 resize(720, 540)，可拖动窗口边缘调整大小。");
```

### 逐段讲解

初始 geometry 在 widget.ui 中；setupUi 读入它。按钮连接建立时没有执行 setFixedSize，只有 clicked 发出后 Lambda 才设置最小和最大尺寸为同一个值。

[this] 让回调访问当前窗口，第三个参数 this 管理连接上下文；writeLine 同时展示结果并输出调试信息。

### 按执行顺序阅读

构造函数先准备界面与连接；交互发生后才进入回调。阅读时按“发送对象 → 信号或调用 → 回调里的数据 → 被修改对象 → 显示结果”追踪，不要把建立连接当成已经执行操作。

### 容易出错的地方与动手验证

先拖动边缘，再点击固定按钮后重试。固定大小会限制布局伸缩，不能把 resize 与 setFixedSize 当成同义词。

先写下预测，再修改一个条件重新构建。记录实际结果，并用本节代码中的变量、对象和调用顺序解释原因；仅仅没有报错不代表已经验证了所有分支。

### 运行结果

![项目 3.1 运行结果](../../examples/03_01_window_size/screenshots/02_after_clicks.png)

### 完整项目与界面文件

- [03_01_window_size.pro](../../examples/03_01_window_size/03_01_window_size.pro)
- [main.cpp](../../examples/03_01_window_size/main.cpp)
- [verify.h](../../examples/03_01_window_size/verify.h)
- [widget.cpp](../../examples/03_01_window_size/widget.cpp)
- [widget.h](../../examples/03_01_window_size/widget.h)
- [widget.ui](../../examples/03_01_window_size/widget.ui)

打开本项目 .pro，选择匹配的 Kit，构建运行；修改界面请打开 .ui，保存后重新构建。verify.h 只在传入 --verify 时执行检查。
