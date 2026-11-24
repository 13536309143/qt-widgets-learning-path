# 5.16 通过this访问窗口成员

[教程中的本知识点](../../docs/knowledge/05_16_this_member.md) · [第 5 章](../../docs/chapters/05-signals-slots-lambda.md)

### 本项目要掌握什么

通过this访问窗口成员。这一节只围绕这个核心问题阅读和修改程序。

### 关键代码在哪里

关键片段 1：[ widget.cpp 第 11 行起](widget.cpp#L11)。以下代码从本项目源文件提取，完整上下文见源文件。

```cpp
connect(ui->runButton, &QPushButton::clicked, this,
        [this]()
        {
            ++count_;
            setWindowTitle(QString("5.16 已点击 %1 次").arg(count_));
            writeLine("count_ = " + QString::number(count_));
        });
```

### 逐段讲解

[this] 保存当前窗口地址，count_ 是窗口成员，所以回调在构造函数结束后仍能访问它，并更新标题与输出。

它不需要 mutable，因为修改的是 this 指向对象的成员，不是给保存的 this 指针副本重新赋值。

### 按执行顺序阅读

构造函数先准备界面与连接；交互发生后才进入回调。阅读时按“发送对象 → 信号或调用 → 回调里的数据 → 被修改对象 → 显示结果”追踪，不要把建立连接当成已经执行操作。

### 容易出错的地方与动手验证

比较 this->count_ 与局部 count 的生命周期；窗口作为 context 断开连接，并不保护其他随意捕获的对象。

先写下预测，再修改一个条件重新构建。记录实际结果，并用本节代码中的变量、对象和调用顺序解释原因；仅仅没有报错不代表已经验证了所有分支。

### 运行结果

![项目 5.16 运行结果](screenshots/02_after_clicks.png)

### 完整项目与界面文件

- [05_16_this_member.pro](05_16_this_member.pro)
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
| `headingLabel` | QLabel | geometry=(24,18,670,32)；text=21 this 与窗口成员 |
| `outputEdit` | QPlainTextEdit | geometry=(24,140,670,370)；readOnly=true |
| `runButton` | QPushButton | geometry=(24,65,220,40)；text=成员计数加一 |
