# 5.18 信号实参与捕获数据同时使用

[教程中的本知识点](../../docs/knowledge/05_18_signal_parameter.md) · [第 5 章](../../docs/chapters/05-signals-slots-lambda.md)

### 本项目要掌握什么

信号实参与捕获数据同时使用。这一节只围绕这个核心问题阅读和修改程序。

### 关键代码在哪里

关键片段 1：[ widget.cpp 第 11 行起](widget.cpp#L11)。以下代码从本项目源文件提取，完整上下文见源文件。

```cpp
QString tag = "保存按钮";
connect(ui->runButton, &QPushButton::clicked, this, [this, tag](bool checked)
        { writeLine(tag + " checked = " + (checked ? "true" : "false")); });
```

### 逐段讲解

tag 由 [this,tag] 在连接建立时保存副本；bool checked 由按钮每次 clicked 通知传入。两类数据的来源和进入时机不同。

按钮在 .ui 中启用 checkable，连续点击改变状态，因此 checked 显示 true、false；this 让回调调用 writeLine。

### 按执行顺序阅读

构造函数先准备界面与连接；交互发生后才进入回调。阅读时按“发送对象 → 信号或调用 → 回调里的数据 → 被修改对象 → 显示结果”追踪，不要把建立连接当成已经执行操作。

### 容易出错的地方与动手验证

取消 checkable 后观察 checked，再把 tag 的外部值改掉比较；不要把 checked 当作某个被捕获的局部变量。

先写下预测，再修改一个条件重新构建。记录实际结果，并用本节代码中的变量、对象和调用顺序解释原因；仅仅没有报错不代表已经验证了所有分支。

### 运行结果

![项目 5.18 运行结果](screenshots/02_after_clicks.png)

### 完整项目与界面文件

- [05_18_signal_parameter.pro](05_18_signal_parameter.pro)
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
| `headingLabel` | QLabel | geometry=(24,18,670,32)；text=23 Lambda 接收信号参数 |
| `outputEdit` | QPlainTextEdit | geometry=(24,140,670,370)；readOnly=true |
| `runButton` | QPushButton | geometry=(24,65,220,40)；text=可勾选按钮；checkable=true |
