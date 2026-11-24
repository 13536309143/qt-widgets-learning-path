# 第五章 信号和槽机制（Lambda）

先理解对象怎样通过信号槽通信，再逐步学习 Lambda。Lambda 是本章内部的一组连续小节，按语法、捕获、状态、生存期、实际按钮回调的顺序展开。

本章对应示例 06—30，项目清单与归属见[总目录](../README.md)。



## 5.1 信号和槽的基本连接

### 5.1.1 把事件和处理代码连起来
信号表示“某件事发生了”，槽或其他可调用对象负责“接到通知后做什么”。按钮点击时发出 clicked 信号，连接到它的处理函数会被调用。发送者不必知道接收者内部怎样实现处理逻辑。
```cpp
auto *button = new QPushButton("关闭窗口", this);
connect(button, &QPushButton::clicked,
        this, &QWidget::close);
```
这行代码可以读成：button 发出 clicked 时，调用当前窗口的 close。connect 建立连接时不会立刻关闭窗口；真正触发 clicked 后才调用处理函数。&QPushButton::clicked 是成员函数指针，写连接时不加 ()，否则就变成调用函数。
### 5.1.2 四个位置怎样记
```cpp
connect(发送对象, &发送类::信号,
        接收对象, &接收类::处理函数);
```
前两个参数回答“谁发出什么通知”，后两个参数回答“由哪个对象执行哪个函数”。在 Widget 成员函数中可以直接写 connect；在普通函数如 main 中通常写 QObject::connect。
clicked 的签名含 bool checked 参数，而 close() 不接收参数。这仍可连接，因为处理函数可以忽略信号末尾的参数。函数指针连接要求对应参数兼容，可存在允许的隐式类型转换；不能把不兼容的参数强行连起来。[参考6](https://doc.qt.io/qt-6/qobject.html)
### 5.1.3 槽函数并不都必须返回 void
自定义信号通常声明为 void；槽是普通 C++ 函数，可以有返回值。但普通信号通知不用于收集槽的返回值。例如 QWidget::close() 返回 bool，也能作为这里的处理函数。入门自定义槽写成 void 最直观。
新式 connect 可以连接普通成员函数，也可以连接 Lambda 等可调用对象。若要声明传统元对象槽，可放在 public slots: 或 private slots: 下；不是所有被连接的成员函数都必须写在 slots 区域。[参考7](https://doc.qt.io/qt-6/signalsandslots.html)
### 5.1.4 一次信号与多个处理函数
可以一对多、多对一，也可以信号连接信号。同线程直接连接时，同一信号连接的多个处理函数按建立连接的顺序调用。跨线程或队列连接涉及事件投递，不应据此推断不同线程中处理完成的先后顺序。[参考6](https://doc.qt.io/qt-6/qobject.html)[参考7](https://doc.qt.io/qt-6/signalsandslots.html)

## 5.2 自定义信号与槽

### 5.2.1 老师与学生的完整声明

```cpp
#pragma once
#include <QObject>
#include <QString>
class Teacher : public QObject
{
    Q_OBJECT
public:
    explicit Teacher(QObject *parent = nullptr)
        : QObject(parent)
    {
    }
    void finishClass()
    {
        emit hungry();
    }
    void requestFood(const QString &food)
    {
        emit hungry(food);
    }
signals:
    void hungry();
    void hungry(const QString &food);
};
```

```cpp
#pragma once
#include <QObject>
#include <QString>
class Student : public QObject
{
    Q_OBJECT
public:
    explicit Student(QObject *parent = nullptr)
        : QObject(parent)
    {
    }
public slots:
    void treat()
    {
        emit message("请老师吃饭");
    }
    void treat(const QString &food)
    {
        emit message("请老师吃饭：" + food);
    }
signals:
    void message(const QString &text);
};
```

Teacher 发出 hungry 信号，Student 的 treat 接收通知，再发出 message 让窗口显示结果。hungry 与 treat 都有重载，连接时需要指定无参数版或 QString 参数版。

### 5.2.2 从按钮到处理函数

打开示例 07 的 widget.cpp，依次找到 Teacher、Student、message 到 writeLine 的连接、hungry 到 treat 的连接，以及按钮 clicked 到 finishClass 的连接。点击时按这条路径追踪；示例 08 改为信号连接信号，示例 09 改为 requestFood 提供 QString 参数。

- [示例 07 custom_signal](../../examples/07_custom_signal/README.md)
- [示例 08 signal_to_signal](../../examples/08_signal_to_signal/README.md)
- [示例 09 overloaded_signal](../../examples/09_overloaded_signal/README.md)

## 5.3 重载信号与断开连接

### 5.3.1 为什么重载必须指定版本
Teacher::hungry 同时存在无参版本和 QString 版本。只写 &Teacher::hungry，编译器不能从这个名字唯一确定函数，因此需要 qOverload 显式选择。
```cpp
// 在拥有 teacher 和 student 的作用域中
connect(teacher,
        qOverload<const QString &>(&Teacher::hungry),
        student,
        qOverload<const QString &>(&Student::treat));
```
qOverload<>() 选择无参版本；qOverload<const QString &>() 选择参数为 const QString & 的版本。这里模板参数必须与函数真实声明相符，不能把 QString 与 const QString & 当成同一个成员函数类型。
原笔记使用成员函数指针变量的思路也成立：
```cpp
void (Teacher::*sig)(const QString &) = &Teacher::hungry;
void (Student::*slot)(const QString &) = &Student::treat;
connect(teacher, sig, student, slot);
```
左边从中间读：sig 是 Teacher 类的成员函数指针，该函数接收 const QString &，返回 void。qOverload 只是让选择版本的表达更紧凑，两个示例是替代写法，不要重复连接。
### 5.3.2 保存连接句柄再断开
```cpp
QMetaObject::Connection c = connect(
    teacher, qOverload<>(&Teacher::hungry),
    student, qOverload<>(&Student::treat));

QObject::disconnect(c);
```
disconnect 是函数，不是 C++ 关键字。保存返回的句柄，能准确断开这一条连接，尤其适合 Lambda。句柄变量离开作用域本身不会自动断开连接。[参考6](https://doc.qt.io/qt-6/qobject.html)
发送者或接收者销毁时，相关连接会被自动清理。Lambda 使用带 context 的 connect 时，context 销毁也会使该连接断开；但 Lambda 捕获的其他对象是否仍然存活，仍要自己保证。
### 5.3.3 重复输出不一定是信号发了两次
同一条连接代码执行两次，默认会建立两条连接，信号发出一次也可能让处理函数运行两次。优先检查初始化函数是否重复调用，避免在反复进入的函数中不断 connect。Qt::UniqueConnection 有适用限制，不能当成所有 Lambda 连接的通用去重开关。[参考6](https://doc.qt.io/qt-6/qobject.html)

## 5.4 Lambda的基本结构与调用

### 5.4.1 它解决什么问题
有时只想在某个位置执行几行代码，专门再给类增加一个函数会显得分散。Lambda 允许把这段行为就地写出来。它属于 C++ 语言功能，不是 Qt 独有语法；可以先独立调用，理解后再交给 connect。
先看一个普通加法函数：
```cpp
int add(int a, int b)
{
    return a + b;
}
int result = add(3, 4);  // result 为 7
```
改写成 Lambda 后：
```cpp
auto add = [](int a, int b) -> int {
    return a + b;
};
int result = add(3, 4);  // 仍然为 7
```
auto 让编译器推导 add 的类型。Lambda 产生的是一个可调用的函数对象，可用 add(...) 调用；它不只是“没有名字的普通函数”。结尾的分号结束变量定义，不能漏掉。
### 5.4.2 一段语法分成五部分
```cpp
[捕获列表](参数列表) mutable -> 返回值类型 {
    函数体
}
```
捕获列表回答“如何使用外部局部变量”；参数列表回答“调用时需要传什么”；mutable 决定是否允许修改值捕获的副本；返回类型回答“返回什么”；函数体写实际执行的语句。[参考8](https://learn.microsoft.com/en-us/cpp/cpp/lambda-expressions-in-cpp)
最开始只用最简单的 []() { ... }。不要把 mutable 和返回类型当成每次必填的内容。返回类型通常可以由 return 推导，上面的 -> int 可以省略。
### 5.4.3 定义和执行是两件事
```cpp
auto say = []() { qDebug() << "你好"; };
// 到这里还没有输出
say();  // 此时才执行函数体并输出
```
写出 Lambda 只是创建可调用对象。调用 say() 才运行它的函数体。connect 中也是先登记这段行为，等信号触发后再执行。

## 5.5 捕获列表和参数列表

### 5.5.1 先看变量是从哪里来的
```cpp
int base = 10;
auto addBase = [base](int x) {
    return base + x;
};
qDebug() << addBase(3);  // 13
qDebug() << addBase(8);  // 18
```
base 在 Lambda 外部定义，创建 Lambda 时通过 [base] 保存一个副本。x 是每次调用时传入的参数，addBase(3) 中 x 是 3，addBase(8) 中 x 是 8。二者都能在函数体里使用，但来源和进入函数体的时间不同。
| 观察点 | 捕获列表 [base] | 参数列表 (int x) |
| --- | --- | --- |
| 数据来源 | 外部作用域中已有的局部变量 | 调用者本次提供的实参 |
| 进入时机 | 创建 Lambda 时建立捕获 | 每次调用时初始化参数 |
| 外面怎样写 | [base] 或 [&base] 等 | addBase(3) 中的 3 |
| 主要用途 | 保留这段行为所需的上下文 | 处理每次调用不同的输入 |
### 5.5.2 没捕获就不能随便用外面的局部变量
```cpp
int base = 10;
// 错误示意 base 是外部自动存储期局部变量
// auto f = [](int x) { return base + x; };

auto f = [base](int x) { return base + x; }; // 正确
```
[] 表示没有捕获，不等于函数体什么都不能做。它仍能使用自己的参数、函数体内新建的局部变量，以及无需捕获就可访问的全局变量或静态变量等。
### 5.5.3 Qt 中两种来源同时出现
```cpp
QString tag = "保存按钮";
connect(button, &QPushButton::clicked, this,
        [tag](bool checked) {
    qDebug() << tag << checked;
});
```
tag 来自创建连接时所在的作用域；checked 来自按钮每次发出的信号。不要把 checked 也写进捕获列表来替代参数，那会改变它的数据来源。普通按钮默认不可勾选，若要观察 checked 切换，可先 button->setCheckable(true)。

## 5.6 值捕获与指针捕获

### 5.6.1 外面的值改变 副本不会跟着改变
```cpp
int n = 10;
auto read = [n]() { return n; };
n = 20;
qDebug() << read();  // 10
qDebug() << n;       // 20
```
执行到 [n] 时，Lambda 保存了当时 n 的值 10。之后 n = 20 修改的是外面的变量。调用 read() 读取的是自己的副本，因此结果仍然是 10。
可以把值捕获理解为“拍下当时的值”，但这只是帮助理解的比喻：实际复制什么、复制后共享什么，由被捕获类型自己的复制语义决定。对 int 而言就是保存一个独立数值。
### 5.6.2 默认不能给值捕获的普通变量赋值
```cpp
int n = 10;
auto read = [n]() {
    // n = 30;  // 编译失败 当前不能修改这个副本
    return n;
};
```
这是 Lambda 默认调用运算符的 const 限制。之后可以用 mutable 允许修改副本，但 mutable 不会把值捕获变成引用捕获。[参考8](https://learn.microsoft.com/en-us/cpp/cpp/lambda-expressions-in-cpp)
### 5.6.3 捕获指针复制的是地址
```cpp
int number = 10;
int *ptr = &number;
auto change = [ptr]() { *ptr = 30; };
change();
qDebug() << number;  // 30
```
副本 ptr 和外面的 ptr 都指向同一个 number。虽然复制了指针，但没有复制它指向的 int。*ptr = 30 改变目标对象，所以外部 number 也变了。这也解释了为什么捕获 this 后可以修改窗口成员。
值捕获不自动延长指针目标的生命。如果捕获的是某个 QWidget*，后面控件先被删除，再使用这个地址仍然危险。学习时要分别问两件事：“复制了什么”和“目标对象还活着吗”。
### 5.6.4 什么时候适合值捕获
希望保存创建连接时的一段文字、一个编号或一个固定配置时，可以显式值捕获。例如 [tag] 比默认 [=] 更容易看清这段代码依赖了什么。需要读取不断变化的状态时，应考虑引用或对象成员，而不是误以为旧副本会自动更新。

## 5.7 引用捕获与生命周期

### 5.7.1 修改会反映到外部
```cpp
int n = 10;
auto increase = [&n]() { ++n; };
increase();
qDebug() << n;  // 11
n = 40;
increase();
qDebug() << n;  // 41
```
[&n] 让函数体通过引用访问外面的 n，因而既能看到外部后续变化，也能修改外部值。这种修改不需要 mutable，因为修改的不是值捕获副本。
### 5.7.2 看懂常见捕获写法
| 写法 | 含义 |
| --- | --- |
| [] | 不捕获外部局部变量 |
| [n] | 按值捕获 n |
| [&n] | 按引用捕获 n |
| [n &m] 的正确写法为 [n, &m] | n 用副本 m 用引用 |
| [=] | 对需要捕获的外部局部变量默认按值捕获 |
| [&] | 对需要捕获的外部局部变量默认按引用捕获 |
| [this] | 保存当前对象的 this 指针 可访问对象成员 |
混合捕获时，逗号分隔每一项。入门优先把所需变量显式写出来，例如 [count, &total]，便于发现外部依赖。默认捕获不等于无条件捕获所在作用域的一切。[参考8](https://learn.microsoft.com/en-us/cpp/cpp/lambda-expressions-in-cpp)
### 5.7.3 最大的风险是变量已经不存在
```cpp
// 错误示意 不要这样给按钮长期保存回调
void Widget::setupButton()
{
    int count = 0;
    auto *button = new QPushButton("计数", this);
    connect(button, &QPushButton::clicked, this,
            [&count]() { ++count; });
}
```
setupButton() 返回后，局部 count 已结束生命周期。按钮以后被点击时才执行 Lambda，这时引用已经悬空，访问它属于未定义行为。即使 connect 写了 this 作为上下文，也不能保护这个局部 count。

## 5.8 mutable与副本状态

### 5.8.1 拆解原来的例子
```cpp
int m = 10;
auto fun = [m](int a, int b) mutable {
    qDebug() << "lambda is running!";
    m = 300;
    return a + b;
};
int sum = fun(100, 200);
qDebug() << sum;
qDebug() << m;
```
依次输出的内容是 lambda is running!、300、10。第一行来自函数体的日志；第二行的 300 是 a + b 的返回值；第三行的 10 来自外部 m。注意这里“内部 m 被赋为 300”和“返回值也是 300”只是数值恰好相同，没有因果关系。
执行过程：① 外部 m 初始化为 10。② [m] 复制 10 进入函数对象。③ mutable 允许给内部副本重新赋值。④ 调用时 a 为 100，b 为 200。⑤ m = 300 改变内部副本。⑥ return a + b 返回 300。⑦ 外部 m 始终没有改变。
### 5.8.2 改写一下 就能看出副本会保留
```cpp
int m = 10;
auto next = [m]() mutable { return ++m; };
qDebug() << next();  // 11
qDebug() << next();  // 12
qDebug() << m;       // 10
```
对同一个 next 对象连续调用，使用的是同一个内部副本，所以从 10 增到 11，再增到 12，并不是每次调用都重新从外部复制 10。创建 Lambda 和调用 Lambda 要分开理解。
### 5.8.3 如果想改变外面的 m
```cpp
int m = 10;
auto next = [&m]() { return ++m; };
qDebug() << next();  // 11
qDebug() << m;       // 11
```
这里使用引用捕获，不需要 mutable。记忆时分开两条：值或引用决定“访问的是副本还是外部变量”；mutable 决定“能否修改值捕获副本”。它们解决的是不同问题。
本笔记采用 C++17，带 mutable 的 Lambda 保留空参数列表 ()，写作 [m]() mutable { ... }。这样也能避免混淆不同 C++ 标准下的省略规则。

## 5.9 this与窗口成员

### 5.9.1 this 本来就是一个指针
在 Widget 的非静态成员函数里，this 指向当前窗口对象。窗口的成员变量属于窗口对象，不属于构造函数的局部变量，因此可以在构造函数返回后继续存在。
```cpp
// widget.h 中 Widget 类的 private 区域
int count_ = 0;
```
```cpp
// Widget 构造函数中
auto *button = new QPushButton("点击计数", this);
connect(button, &QPushButton::clicked, this, [this]() {
    ++count_;
    setWindowTitle(QString("已点击 %1 次").arg(count_));
});
```
count_ 等价于 this->count_，setWindowTitle(...) 等价于 this->setWindowTitle(...)。通过保存的 this 指针访问当前窗口，所以不需要把每个成员都单独放进捕获列表。
### 5.9.2 为什么不用 mutable 也能修改成员
[this] 保存的是 this 指针，修改的是它指向的窗口对象的成员，并不是改写这个指针副本。可以联系上一章 [ptr] 修改 *ptr 的例子理解。
不能因此说“值捕获永远不会改变外部状态”。准确说法是：复制 int 后修改副本不会改变原 int；复制指针后，经指针访问的仍可能是同一个对象。
### 5.9.3 两个 this 扮演不同角色
```cpp
connect(button, &QPushButton::clicked, this, [this]() {
    close();
});
```
第三个参数 this 是连接的上下文对象，参与自动断开和线程上下文选择；[this] 是 C++ Lambda 的捕获，让函数体能访问窗口成员。二者不能相互替代：有 context 不代表自动完成 C++ 捕获；有捕获也不代表自动建立 Qt 生命周期保护。[参考6](https://doc.qt.io/qt-6/qobject.html)
### 5.9.4 不要把 this 当成窗口副本
[this] 不复制窗口，也不让窗口永远存活。对于异步保存的回调，要保证对象还存在；Qt 的带 context 连接会在 context 销毁时断开，但保护范围不包括任意捕获对象。
C++17 还有 [*this]，表示复制当前对象。QObject 及其子类通常不可复制，Widget 也不适合这样捕获。Qt 初学阶段优先使用显式 [this]，不依赖 [=] 隐式捕获 this。

## 5.10 把Lambda交给connect

### 5.10.1 先写出最小可用版本
```cpp
auto *button = new QPushButton("点击", this);
connect(button, &QPushButton::clicked, this, []() {
    qDebug() << "按钮被按下";
});
```
这里不需要外部变量，因此捕获列表是 []。点击后，Qt 调用保存的函数对象；建立连接时不会执行日志。clicked 虽带 bool 参数，但这个 Lambda 不需要，便可以省略这个参数。
### 5.10.2 需要窗口成员时改成显式捕获
```cpp
connect(button, &QPushButton::clicked, this, [this]() {
    setWindowTitle("已经点击");
});
```
判断捕获的方法是读函数体：如果只做独立计算或输出固定文本，可能 [] 就够；如果访问窗口成员，使用 [this]；如果只需要创建连接时的一段局部文字，使用 [text]；如果需要临时引用局部变量，先确认生命周期再写 [&value]。
### 5.10.3 Lambda 为什么适合 Qt 初学
简单处理逻辑可以紧靠连接代码阅读，不必为了两行代码单独声明和实现一个槽函数。当处理流程变长、需要复用、需要单独测试时，应提取成成员函数，再由 Lambda 调用，或直接连接到该成员函数。
```cpp
// 如果已经声明并实现 Widget::saveData()
connect(button, &QPushButton::clicked,
        this, &Widget::saveData);
```
上面是替代写法，不必同时保留两个连接。选择槽成员函数还是 Lambda，依据逻辑是否需要命名和复用，而不是认为某一种语法总是更高级。
### 5.10.4 四个参数版优先
初学时优先使用带 context 的四参数连接。它让连接跟随上下文对象的生命周期，也使回调执行的线程上下文更明确。即使如此，也要检查 Lambda 捕获的每个指针和引用。[参考6](https://doc.qt.io/qt-6/qobject.html)
构造界面时常写“一个 new、一个设置属性、一个 connect”。把这些初始化步骤执行一次即可，不要在每次点击后又给同一个按钮不断追加连接。

## 5.11 参数适配与重载调用

### 5.11.1 信号给出的参数不一定就是业务参数
老师的带参 hungry 需要 QString，按钮 clicked 提供的是 bool。不能把 bool 当作菜名直接连过去。Lambda 可以把这次点击转换成“请老师吃某道菜”的业务操作。
回看 5.2 节 Teacher 已有的 requestFood 方法，它接收菜名并发出 hungry(food)。下面的实现用于对照阅读，不要重复添加：
```cpp
void requestFood(const QString &food)
{
    emit hungry(food);
}
```
在示例 09 或 24 的 Widget 构造函数中，使用下面的连接，让 Lambda 将 clicked 转为带菜名参数的调用。保留本次需要的连接，避免同一按钮重复连接。
```cpp
auto *teacher = new Teacher(this);
auto *student = new Student(this);
auto *button = new QPushButton("请吃饭", this);

connect(teacher,
        qOverload<const QString &>(&Teacher::hungry),
        student,
        qOverload<const QString &>(&Student::treat));

connect(button, &QPushButton::clicked, this, [teacher]() {
    teacher->requestFood(QStringLiteral("红烧排骨"));
});
```
执行链是 clicked → Lambda → requestFood("红烧排骨") → hungry(food) → treat(food)。Lambda 不接收 checked，因为这里用不到它。teacher 被值捕获，复制的是指针，目标仍是那位老师。
本例 teacher 和 button 都由窗口管理，且不提前删除 teacher。若以后加入“单独删除老师”的功能，就需要重新检查连接的上下文和捕获指针，不能原样照搬生命周期假设。
### 5.11.2 Lambda 可以把选择重载变成普通调用
如果需要调用 student 的带参 treat，可以写一个参数明确的 Lambda，再在里面调用 student->treat(food)。调用表达式有具体实参时，编译器可以按普通 C++ 重载规则选中版本。
不过发送端的重载信号仍要选清楚。Lambda 不是取消所有 qOverload 的开关；它主要让接收逻辑、参数转换、默认值填充等处理更直观。

## 5.12 延后执行与循环捕获

### 5.12.1 引用捕获了构造函数的局部变量
```cpp
// 错误示意 构造函数结束后 count 不再存在
int count = 0;
connect(button, &QPushButton::clicked, this,
        [&count]() { ++count; });
```
修正方法：把状态放到 Widget 的成员 count_ 中，再用 [this] 访问。窗口还活着时，成员才存在；窗口销毁时，以它为 context 的连接也会断开。完整实现见下一章。
### 5.12.2 值捕获了循环变量 却以为会读取最终值
```cpp
// 这段放在 Widget 构造函数中
for (int i = 0; i < 3; ++i) {
    auto *b = new QPushButton(QString::number(i), this);
    b->move(20, 20 + i * 50);
    connect(b, &QPushButton::clicked, this, [i]() {
        qDebug() << i;
    });
}
```
三个 Lambda 分别保存创建时的 i，点击按钮依次输出对应的 0、1、2。这正是想保留按钮编号时应有的行为。改成 [&i] 会让回调引用循环局部变量，循环结束后就失效，不能用这种方式实现编号。
### 5.12.3 定义了 Lambda 却没有调用它
```cpp
auto work = []() { qDebug() << "开始处理"; };
work();  // 普通 C++ 场景 自己调用
```
```cpp
connect(button, &QPushButton::clicked, this,
        []() { qDebug() << "开始处理"; });
// Qt 场景 等按钮发出信号时由 Qt 调用
```
把函数对象交给 connect 时，不要在 Lambda 后面加 () 去立即执行它。connect 需要收到“以后可以调用的行为”，不是收到这段行为现在执行后的结果。
### 5.12.4 每写一次捕获 都做一次检查
值捕获：我想保存创建时的值吗？引用捕获：这个变量会不会先消失？指针捕获：目标对象可能提前删除吗？this 捕获：有没有合适的 context，回调是否还被保存到其他地方？先回答这些问题，再选 [] 内的写法。

## 5.13 综合练习 计数与清零

目标是让计数保存在窗口成员中，加一和清零都更新标题。按钮和输出区已在 widget.ui 中配置，完整工程使用示例 28；示例 29 再把加一行为改为普通成员函数。

### widget.h

```cpp
#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui
{
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget() override;
    void writeLine(const QString &text);

private:
    Ui::Widget *ui;
    int count_ = 0;
    void refreshTitle();
};
#endif // WIDGET_H
```

### widget.cpp

```cpp
#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include "verify.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(ui->runButton, &QPushButton::clicked, this,
            [this]()
            {
                ++count_;
                refreshTitle();
                writeLine("加一后 count_ = " + QString::number(count_));
            });
    connect(ui->resetButton, &QPushButton::clicked, this,
            [this]()
            {
                count_ = 0;
                refreshTitle();
                writeLine("清零后 count_ = 0");
            });
    refreshTitle();

    // 仅带 --verify 参数时安排检查；正常学习时不会执行。
    scheduleVerification(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::writeLine(const QString &text)
{
    ui->outputEdit->appendPlainText(text);
    qDebug().noquote() << text;
}

void Widget::refreshTitle()
{
    setWindowTitle(QString("已点击 %1 次").arg(count_));
}
```

先预测点击两次加一、再清零后的标题，然后运行验证。查看 count_ 的存放位置、[this] 的作用，以及 refreshTitle 为什么可以由多个处理函数复用。

- [示例 28 complete_counter](../../examples/28_complete_counter/README.md)
- [示例 29 member_slot](../../examples/29_member_slot/README.md)

## 5.14 Lambda自测与答案

### 5.14.1 先预测输出 再运行
题目一：下面两个输出分别是多少？说明为什么。
```cpp
int x = 5;
auto f = [x]() { return x + 1; };
x = 100;
qDebug() << f();
qDebug() << x;
```
题目二：修改捕获列表并补全函数体，让调用 f() 后外面的 x 变成 6。
```cpp
int x = 5;
auto f = [x]() { /* 在这里让 x 加一 */ };
f();
qDebug() << x;
```
题目三：内部副本会不会每次调用都重新初始化？预测三次输出。
```cpp
int x = 5;
auto f = [x]() mutable { return ++x; };
qDebug() << f();
qDebug() << f();
qDebug() << x;
```
### 5.14.2 把语法放回 Qt 场景
题目四：构造函数里有 int count = 0，然后用 [&count] 连接按钮点击。为什么构造函数执行时不一定出错，点击后却有风险？写一个更合适的状态保存位置。
题目五：下面第三个参数 this 和 [this] 各自负责什么？删除 [this] 后，函数体还能直接访问窗口成员吗？
```cpp
connect(button, &QPushButton::clicked, this, [this]() {
    setWindowTitle("完成");
});
```
题目六：已知 clicked(bool) 的参数是 bool，Teacher::requestFood(const QString &) 需要菜名。请写出用 Lambda 传入“面条”的 connect。
做题时不要求背出所有术语。只要能说清“何时保存数据”“函数体何时执行”“访问的是哪个变量”“它还活着吗”，就掌握了最关键的部分。

### 5.14.3 基础题答案
题目一：依次为 6、100。创建 f 时按值捕获 x = 5；f() 使用副本计算 5 + 1；外部 x 已被赋值为 100。
题目二：使用 [&x]，函数体写 ++x;。引用捕获操作的是外面的 x。若改成 [x]() mutable { ++x; }，只能改变副本，外部仍是 5。
```cpp
int x = 5;
auto f = [&x]() { ++x; };
f();
qDebug() << x;  // 6
```
题目三：依次为 6、7、5。同一个 f 对象保留着自己的 x 副本，两次调用连续修改副本，外部 x 没有改变。
### 5.14.4 Qt 场景答案
题目四：connect 只是保存回调，构造函数返回前不一定触发点击。等用户点击时，局部 count 已结束生命周期，引用失效。把计数改成 Widget 的成员 int count_ = 0;，然后以窗口为 context，使用 [this] 访问。
题目五：第三个 this 是 Qt 的 context，窗口销毁时相关连接会自动断开；[this] 是 C++ 捕获，让函数体能够访问窗口成员。删除捕获而保留 [] 时，这里的 setWindowTitle() 缺少可用的当前对象，不能这样调用。
题目六：在 teacher 和 button 已经创建、并保证 teacher 在回调执行时仍然存活的前提下：
```cpp
connect(button, &QPushButton::clicked, this, [teacher]() {
    teacher->requestFood(QStringLiteral("面条"));
});
```
该 Lambda 忽略信号的 bool 参数，自己提供 QString 业务参数。它捕获的是 teacher 指针的副本，不会复制 Teacher 对象。
### 5.14.5 遇到新 Lambda 时的阅读顺序
先看函数体，确定它要做什么；再看参数列表，确定每次输入来自哪里；然后看捕获列表，确认外部依赖的来源与生命期；最后找调用位置，区分立即调用、保存后调用、由 Qt 信号触发调用。
如果读不懂复杂写法，可以先把它保存为 auto task = ...，再单独写 task(...) 测试普通 C++ 行为。理解后再放回 connect，不必同时理解所有语法和事件机制。

## 5.15 易错点回顾与笔记整理

### 5.15.1 需要记牢的修正
| 常见误记 | 应怎样理解 |
| --- | --- |
| Qt Creator 就是编译器 | 它是开发环境 调用独立编译器和构建工具 |
| .pro 直接生成可执行文件 | qmake 读取配置 再由构建工具完成编译和链接 |
| Qt 会自动释放所有 new 对象 | QObject 父子树只管理有明确归属的子对象 |
| 窗口关闭等于对象已析构 | 关闭和销毁是两个动作 要看存储期及窗口属性 |
| 所有槽返回值必须为 void | 槽可有返回值 普通信号通知通常不收集返回值 |
| 多个槽的顺序完全不确定 | 同线程直接连接按连接顺序调用 跨线程完成顺序另论 |
| disconnect 是关键字 | 它是用于断开连接的函数 |
| [=] 复制窗口所有成员 | 访问成员时涉及 this 不会自动复制整个窗口 |
| mutable 能修改外部变量 | 它允许修改值捕获副本 引用捕获才直接访问原变量 |
| connect 有 this 就不会悬空 | 其他被捕获的指针或引用仍需检查生命期 |
### 5.15.2 以后每个新知识点沿用这个结构
知识点名称：写清要解决什么问题。
基本概念：用自己的话解释它与已有知识的关系。
最小例子：标明代码放在哪个文件、需要哪些头文件。
执行过程：解释从入口到结果发生了什么。
容易出错：记录错误现象、原因、修正办法。
练习与验证：先写预期行为，再记录实际观察和结论。

继续学习时，按第六章主窗口、第七章对话框、第八章布局的顺序展开；同一知识点的新例子归入对应小节。

## 5.16 本章示例对照

| 示例 | 练习重点 |
|---|---|
| 06—11 | 基本连接、自定义信号、重载与断开 |
| 12—20 | Lambda语法、定义与调用、捕获和mutable |
| 21—27 | this、信号参数、适配、循环与延后执行 |
| 28—30 | 计数综合练习、成员函数与自测 |


---

[总目录](../README.md) · [上一章](04-buttons.md) · [下一章](06-mainwindow.md)
