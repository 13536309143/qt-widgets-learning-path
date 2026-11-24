# 5.24 连接普通成员函数

[所属章节](../chapters/05-signals-slots-lambda.md) · [完整项目](../../examples/05_24_member_slot/README.md)

### 本项目要掌握什么

连接普通成员函数。这一节只围绕这个核心问题阅读和修改程序。

### 关键代码在哪里

关键片段 1：[ widget.cpp 第 11 行起](../../examples/05_24_member_slot/widget.cpp#L11)。以下代码从本项目源文件提取，完整上下文见源文件。

```cpp
connect(ui->runButton, &QPushButton::clicked, this, &Widget::increase);
connect(ui->resetButton, &QPushButton::clicked, this,
        [this]()
        {
            count_ = 0;
            refreshTitle();
            writeLine("清零后 count_ = 0");
        });
refreshTitle();
```

关键片段 2：[ widget.cpp 第 41 行起](../../examples/05_24_member_slot/widget.cpp#L41)。以下代码从本项目源文件提取，完整上下文见源文件。

```cpp
void Widget::increase()
{
    ++count_;
    refreshTitle();
    writeLine("加一后 count_ = " + QString::number(count_));
}
```

### 逐段讲解

加一按钮直接连接 &Widget::increase，窗口 this 是实际执行该成员函数的对象；这里不需要把同样的逻辑再包进 Lambda。

increase 更新 count_ 并调用 refreshTitle，与清零回调使用同一成员状态。新式 connect 可以连接匹配的普通成员函数。

### 按执行顺序阅读

构造函数先准备界面与连接；交互发生后才进入回调。阅读时按“发送对象 → 信号或调用 → 回调里的数据 → 被修改对象 → 显示结果”追踪，不要把建立连接当成已经执行操作。

### 容易出错的地方与动手验证

把加一逻辑同时接到另一个按钮，验证两入口共享状态；不要同时保留旧 Lambda 和新连接导致一次点击加两次。

先写下预测，再修改一个条件重新构建。记录实际结果，并用本节代码中的变量、对象和调用顺序解释原因；仅仅没有报错不代表已经验证了所有分支。

### 运行结果

![项目 5.24 运行结果](../../examples/05_24_member_slot/screenshots/03_after_reset.png)

### 完整项目与界面文件

- [05_24_member_slot.pro](../../examples/05_24_member_slot/05_24_member_slot.pro)
- [main.cpp](../../examples/05_24_member_slot/main.cpp)
- [verify.h](../../examples/05_24_member_slot/verify.h)
- [widget.cpp](../../examples/05_24_member_slot/widget.cpp)
- [widget.h](../../examples/05_24_member_slot/widget.h)
- [widget.ui](../../examples/05_24_member_slot/widget.ui)

打开本项目 .pro，选择匹配的 Kit，构建运行；修改界面请打开 .ui，保存后重新构建。verify.h 只在传入 --verify 时执行检查。
