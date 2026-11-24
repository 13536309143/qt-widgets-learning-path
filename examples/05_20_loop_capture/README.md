# 5.20 循环回调保存各自编号

[教程中的本知识点](../../docs/knowledge/05_20_loop_capture.md) · [第 5 章](../../docs/chapters/05-signals-slots-lambda.md)

### 本项目要掌握什么

循环回调保存各自编号。这一节只围绕这个核心问题阅读和修改程序。

### 关键代码在哪里

关键片段 1：[ widget.cpp 第 12 行起](widget.cpp#L12)。以下代码从本项目源文件提取，完整上下文见源文件。

```cpp
const QList<QPushButton *> buttons = {ui->number0, ui->number1, ui->number2};
for (int i = 0; i < buttons.size(); ++i)
{
    auto *button = buttons.at(i);
    connect(button, &QPushButton::clicked, this,
            [this, i]() { writeLine("点击的编号 = " + QString::number(i)); });
}
```

### 逐段讲解

buttons 是三个 .ui 按钮的指针集合，循环每次取出一个按钮。[this,i] 在每次建立连接时保存当次 i 的数值。

循环结束后用户才点击，但每个回调都有自己的 0、1、2 副本，因此不会读取构造函数里已经消失的循环变量。

### 按执行顺序阅读

构造函数先准备界面与连接；交互发生后才进入回调。阅读时按“发送对象 → 信号或调用 → 回调里的数据 → 被修改对象 → 显示结果”追踪，不要把建立连接当成已经执行操作。

### 容易出错的地方与动手验证

把循环改为更大的按钮集合；不要用 [&i] 试图让每个按钮保留编号，循环结束后这个引用会失效。

先写下预测，再修改一个条件重新构建。记录实际结果，并用本节代码中的变量、对象和调用顺序解释原因；仅仅没有报错不代表已经验证了所有分支。

### 运行结果

![项目 5.20 运行结果](screenshots/02_after_clicks.png)

### 完整项目与界面文件

- [05_20_loop_capture.pro](05_20_loop_capture.pro)
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
| `headingLabel` | QLabel | geometry=(24,18,670,32)；text=25 循环中保存按钮编号 |
| `outputEdit` | QPlainTextEdit | geometry=(24,140,670,370)；readOnly=true |
| `number0` | QPushButton | geometry=(24,65,160,40)；text=编号 0 |
| `number1` | QPushButton | geometry=(204,65,160,40)；text=编号 1 |
| `number2` | QPushButton | geometry=(384,65,160,40)；text=编号 2 |
