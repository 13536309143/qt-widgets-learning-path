# 5.11 引用捕获访问原变量

[所属章节](../chapters/05-signals-slots-lambda.md) · [完整项目](../../examples/05_11_reference_capture/README.md)

### 本项目要掌握什么

引用捕获访问原变量。这一节只围绕这个核心问题阅读和修改程序。

### 关键代码在哪里

关键片段 1：[ widget.cpp 第 11 行起](../../examples/05_11_reference_capture/widget.cpp#L11)。以下代码从本项目源文件提取，完整上下文见源文件。

```cpp
int n = 10;
auto increase = [&n]() { ++n; };
increase();
writeLine("第一次调用后 n = " + QString::number(n));
n = 40;
increase();
writeLine("第二次调用后 n = " + QString::number(n));
```

### 逐段讲解

[&n] 不保存独立数值副本，而是引用外部 n。第一次 increase 将 10 改为 11，外部再设为 40，第二次调用得到 41。

两次调用都在 n 的作用域内完成，所以引用仍然有效；同一写法保存到构造结束后的按钮回调里就可能失效。

### 按执行顺序阅读

本项目的主要实验在构造函数里立即执行：变量初始化 → 定义或连接 → 显式调用 → 显示结果。先在代码中圈出实际调用位置，再区分哪些语句只是定义或登记行为。

### 容易出错的地方与动手验证

把本例的立即调用与长期回调画在时间线上比较；不能只看到 & 就认为任何时刻都安全。

先写下预测，再修改一个条件重新构建。记录实际结果，并用本节代码中的变量、对象和调用顺序解释原因；仅仅没有报错不代表已经验证了所有分支。

### 运行结果

![项目 5.11 运行结果](../../examples/05_11_reference_capture/screenshots/01_initial.png)

### 完整项目与界面文件

- [05_11_reference_capture.pro](../../examples/05_11_reference_capture/05_11_reference_capture.pro)
- [main.cpp](../../examples/05_11_reference_capture/main.cpp)
- [verify.h](../../examples/05_11_reference_capture/verify.h)
- [widget.cpp](../../examples/05_11_reference_capture/widget.cpp)
- [widget.h](../../examples/05_11_reference_capture/widget.h)
- [widget.ui](../../examples/05_11_reference_capture/widget.ui)

打开本项目 .pro，选择匹配的 Kit，构建运行；修改界面请打开 .ui，保存后重新构建。verify.h 只在传入 --verify 时执行检查。
