# 5.13 指针捕获复制地址

[所属章节](../chapters/05-signals-slots-lambda.md) · [完整项目](../../examples/05_13_pointer_capture/README.md)

### 本项目要掌握什么

指针捕获复制地址。这一节只围绕这个核心问题阅读和修改程序。

### 关键代码在哪里

关键片段 1：[ widget.cpp 第 11 行起](../../examples/05_13_pointer_capture/widget.cpp#L11)。以下代码从本项目源文件提取，完整上下文见源文件。

```cpp
int number = 10;
int *ptr = &number;
auto change = [ptr]() { *ptr = 30; };
change();
writeLine("number = " + QString::number(number));
writeLine("[ptr] 没有复制 number，只复制了地址。");
```

### 逐段讲解

ptr 指向 number，[ptr] 复制的是指针值。副本仍指向同一个 number，所以 *ptr=30 会改变外部整数。

这没有复制整个目标对象，也不延长目标对象的生命周期；本例立即调用，number 尚未离开作用域。

### 按执行顺序阅读

本项目的主要实验在构造函数里立即执行：变量初始化 → 定义或连接 → 显式调用 → 显示结果。先在代码中圈出实际调用位置，再区分哪些语句只是定义或登记行为。

### 容易出错的地方与动手验证

比较 ptr=nullptr 与 *ptr=30 的含义；指针不空也不能证明目标仍然存活。

先写下预测，再修改一个条件重新构建。记录实际结果，并用本节代码中的变量、对象和调用顺序解释原因；仅仅没有报错不代表已经验证了所有分支。

### 运行结果

![项目 5.13 运行结果](../../examples/05_13_pointer_capture/screenshots/01_initial.png)

### 完整项目与界面文件

- [05_13_pointer_capture.pro](../../examples/05_13_pointer_capture/05_13_pointer_capture.pro)
- [main.cpp](../../examples/05_13_pointer_capture/main.cpp)
- [verify.h](../../examples/05_13_pointer_capture/verify.h)
- [widget.cpp](../../examples/05_13_pointer_capture/widget.cpp)
- [widget.h](../../examples/05_13_pointer_capture/widget.h)
- [widget.ui](../../examples/05_13_pointer_capture/widget.ui)

打开本项目 .pro，选择匹配的 Kit，构建运行；修改界面请打开 .ui，保存后重新构建。verify.h 只在传入 --verify 时执行检查。
