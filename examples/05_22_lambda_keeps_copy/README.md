# 5.22 回调内部保存独立计数

[教程中的本知识点](../../docs/knowledge/05_22_lambda_keeps_copy.md) · [第 5 章](../../docs/chapters/05-signals-slots-lambda.md)

### 本项目要掌握什么

回调内部保存独立计数。这一节只围绕这个核心问题阅读和修改程序。

### 关键代码在哪里

关键片段 1：[ widget.cpp 第 11 行起](widget.cpp#L11)。以下代码从本项目源文件提取，完整上下文见源文件。

```cpp
int count = 0;
connect(ui->runButton, &QPushButton::clicked, this,
        [this, count]() mutable
        {
            ++count;
            writeLine("Lambda 内部 count = " + QString::number(count));
        });
writeLine("外部局部 count = " + QString::number(count));
writeLine("连接保存的是副本，不引用这个局部 count。");
```

### 逐段讲解

count 在 connect 时按值捕获，mutable 允许每次 clicked 修改回调自己的副本。外部 count 输出为 0，构造结束后它也不再需要。

Qt 保存的那个回调对象会持续保留内部状态，因此连续点击得到 1、2。这个计数不是 Widget 的成员。

### 按执行顺序阅读

构造函数先准备界面与连接；交互发生后才进入回调。阅读时按“发送对象 → 信号或调用 → 回调里的数据 → 被修改对象 → 显示结果”追踪，不要把建立连接当成已经执行操作。

### 容易出错的地方与动手验证

比较本例与上例的状态归属，考虑多个按钮是否需要共享同一计数；相互独立的闭包不能自动同步。

先写下预测，再修改一个条件重新构建。记录实际结果，并用本节代码中的变量、对象和调用顺序解释原因；仅仅没有报错不代表已经验证了所有分支。

### 运行结果

![项目 5.22 运行结果](screenshots/02_after_clicks.png)

### 完整项目与界面文件

- [05_22_lambda_keeps_copy.pro](05_22_lambda_keeps_copy.pro)
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
| `headingLabel` | QLabel | geometry=(24,18,670,32)；text=27 值捕获也能保存回调自身状态 |
| `outputEdit` | QPlainTextEdit | geometry=(24,140,670,370)；readOnly=true |
| `runButton` | QPushButton | geometry=(24,65,250,40)；text=回调内部计数 |
