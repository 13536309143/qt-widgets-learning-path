# Qt Widgets 入门学习笔记
## 第一阶段 从创建项目到 Lambda
这份笔记围绕一个问题展开：怎样用 C++ 创建一个窗口，让按钮触发自己的代码，并把对象的生命周期管理清楚。前半部分建立 Qt Widgets 的基本概念，后半部分专门拆解 Lambda，适合边看边敲代码。
本阶段使用 Qt 6、Qt Widgets 和 C++17，构建配置沿用 qmake。前面的短代码通常放在 Widget 构造函数中；需要单独文件的例子会标明文件名。Lambda 的纯 C++ 例子可放入已有 Qt 项目中运行，使用 qDebug() 时包含 <QDebug>。
## 阅读路线
1　开发工具与第一个项目
2　项目文件与程序入口
3　窗口 按钮与坐标
4　对象树与内存管理
5　信号与槽的基本连接
6　自定义信号与槽
7　重载连接与断开连接
8—16　Lambda 从语法到 Qt 中的实际使用
17　综合练习 点击计数窗口
18　自测与参考答案
19　易错点回顾与后续笔记结构
## 学完后要能做到
能解释 QApplication、Widget、show() 和 exec() 的关系；能判断一个 new 出来的控件由谁释放；能独立写出 connect；能根据 Lambda 的捕获方式预测输出，并避免捕获已经失效的局部变量。
使用方法：先预测代码结果，再运行验证。遇到问题时，优先区分“编译失败”“程序运行但没有反应”“程序崩溃”，这三类问题的排查方向不同。

# 1 开发工具与第一个项目
## 1.1 先分清几个名字
Qt 是提供窗口、按钮、对象通信等能力的 C++ 框架。Qt Creator 是集成开发环境，负责编辑、组织项目、调用构建工具和调试器。MinGW 或 MSVC 中的编译器负责把 C++ 源码编译成目标文件，链接器再把目标文件与库链接成程序。
qmake 读取 .pro 项目配置，生成 Makefile 等构建文件；make 等工具再按规则调用编译器和链接器。Qt Creator 的项目向导可以生成初始 .pro，但 .pro 本身不是可执行文件。[1][2]
Kit 可以理解为一套配好的开发工具组合，包含 Qt 版本、编译器、调试器及目标设备等。选择 Kit 时，要让编译器和 Qt 库匹配，例如使用 MinGW 版 Qt 时配套选择相应 MinGW 工具链。
## 1.2 创建项目时各项选择的含义
在新建项目向导中选择 Qt Widgets Application，填写项目名称和位置，构建系统选择 qmake。类名设为 Widget，基类选择 QWidget。为了先理解代码，可取消 Generate form；这样暂时不生成 .ui 文件，直接在构造函数里创建控件。
| 基类 | 适用场景 | 当前怎样选择 |
| --- | --- | --- |
| QWidget | 普通窗口及各种可视控件的基础 | 入门空白窗口选它 |
| QMainWindow | 带菜单 工具栏 状态栏和中心区域的主窗口框架 | 做完整主界面时再用 |
| QDialog | 用于设置 确认等交互的对话框 | 不只是外观不同 还涉及对话流程 |
继续选择可用 Kit 和 Debug 配置，完成后先构建并运行。项目向导的页面顺序和名称可能随 Qt Creator 版本变化，应以选项含义为准。[1]
## 1.3 构建 运行和调试
构建负责生成程序；运行负责启动程序；调试是在调试器控制下运行，可以设断点、单步执行和查看变量。运行按钮通常会先检查是否需要构建。Debug 便于调试，Release 通常启用更多优化。
编译错误先看 Issues 与编译输出中的第一条相关错误；qDebug() 的输出通常看 Application Output。不要把程序输出和编译器报错混在一起。

# 2 项目文件与程序入口
## 2.1 每个文件负责什么
| 文件 | 主要职责 |
| --- | --- |
| 项目名.pro | 声明 Qt 模块 C++ 标准 源文件及头文件等构建信息 |
| main.cpp | 创建应用对象 创建并显示窗口 启动事件循环 |
| widget.h | 声明 Widget 类 构造函数 成员及需要的接口 |
| widget.cpp | 实现 Widget 的函数 初始界面通常在构造函数中建立 |
| widget.ui | 可选的界面设计文件 由界面设计工具编辑 |
最小 qmake 配置示意如下。加入新类后，确认相应 .h 和 .cpp 已加入项目，并重新运行 qmake。生成的 moc 文件属于构建产物，不手动修改。[2]
```qmake
QT += widgets
CONFIG += c++17
TEMPLATE = app
TARGET = qt_notes
SOURCES += main.cpp widget.cpp
HEADERS += widget.h
```
## 2.2 main.cpp 的执行顺序
```cpp
#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Widget w;
    w.show();
    return app.exec();
}
```
① QApplication 必须在创建 QWidget 对象之前建立，它管理 GUI 应用的公共资源与事件处理。② Widget w 创建自定义窗口，这时会执行构造函数。③ show() 让窗口显示出来。④ exec() 启动事件循环，程序才会持续处理点击、键盘输入和重绘等事件。[3]
exec() 通常会等到应用退出才返回。如果直接 return 0，程序很快结束，窗口也就无法正常停留和交互。事件循环也不意味着需要自己写一个不断检测按钮的 while 循环。

# 3 窗口 按钮与坐标
## 3.1 一个最小的 Widget 类
widget.h 声明类；widget.cpp 实现构造函数。下面的按钮可以在后续章节继续添加 connect。
```cpp
// widget.h
#pragma once
#include <QWidget>

class Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Widget(QWidget *parent = nullptr);
};
```
```cpp
// widget.cpp
#include "widget.h"
#include <QPushButton>

Widget::Widget(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("我的第一个窗口");
    resize(600, 400);
    auto *button = new QPushButton("第一个按钮", this);
    button->move(100, 80);
    button->resize(140, 40);
}
```
this 指向当前 Widget 对象。在成员函数中，setWindowTitle(...) 与 this->setWindowTitle(...) 含义相同。new QPushButton("文字", this) 同时设置按钮文字和父控件；构造时传父对象比先创建再 setParent(this) 更直接。
## 3.2 大小和位置分别控制什么
resize(w, h) 设置当前大小，通常仍允许后续调整；setFixedSize(w, h) 把最小和最大尺寸限制为同一个大小，用户无法继续拖动改变尺寸。move(x, y) 改变位置，setGeometry(x, y, w, h) 同时改变位置和大小。[4]
子控件坐标以父控件内容区域左上角为原点，x 向右增加，y 向下增加。因此 move(100, 80) 表示按钮左上角距离父控件左侧 100、上侧 80 个逻辑像素。高 DPI 环境下，逻辑像素不一定等于屏幕物理像素。
顶层窗口和子控件的定位参考系不同。不要用桌面左上角去理解子按钮的 move()。固定坐标适合初学；后续学习布局管理器后，应让布局处理窗口缩放、字体变化和不同屏幕尺寸。

# 4 对象树与内存管理
## 4.1 谁负责释放 new 出来的对象
QObject 的父子关系构成对象树。父对象析构时，会删除仍属于自己的子对象；子对象先析构时，会把自己从父对象的子对象列表移除。QWidget 在此基础上还提供可视控件的父子关系。[5]
```cpp
auto *button = new QPushButton("按钮", this);
// this 析构时负责删除 button 指向的对象
// 局部指针 button 离开作用域 不会删除按钮对象
```
局部指针变量和它指向的对象是两回事。构造函数结束后，button 这个局部变量消失了，但堆上的按钮继续存在。后面还要访问它，可以把指针保存为成员变量。
没有 parent 的 new QObject 或 new Teacher 不会因为“使用了 Qt”就自动回收。初学时，让属于窗口的 QObject 在创建时接收 this 作为父对象。这个机制不管理所有 C++ 类型，例如 QString 是值类型，按自己的 C++ 生命周期管理。
## 4.2 栈对象必须注意构造顺序
```cpp
// 安全示例 先构造父对象 再构造子对象
QWidget window;
QPushButton button("退出", &window);
```
离开作用域时逆序析构：先析构 button，并从 window 的子列表移除；再析构 window。此时 window 不会重复删除 button。
```cpp
// 错误示例 不要运行这段来作为正常写法
QPushButton quit("Quit");
QWidget window;
quit.setParent(&window);
```
这里 window 后构造、先析构，会试图删除作为子对象的 quit；但 quit 的存储属于栈，不能由父对象用 delete 释放，随后还存在自动析构问题，属于未定义行为，可能崩溃。Qt 的父子管理不能替代 C++ 的存储期规则。[5]
## 4.3 用日志观察析构
可以定义继承 QPushButton 的 MyButton，在构造函数打印 qDebug() << "MyButton"，在析构函数打印 qDebug() << "~MyButton"。把它 new 出来并传 this，正常结束程序时观察构造与析构输出。
关闭窗口不一定立刻删除窗口对象。main 中的栈窗口通常在 exec() 返回后析构；堆窗口是否随关闭删除还受 WA_DeleteOnClose 等设置影响。判断释放时机时，应追踪对象析构，不能只看窗口是否消失。[4]

# 5 信号与槽的基本连接
## 5.1 把事件和处理代码连起来
信号表示“某件事发生了”，槽或其他可调用对象负责“接到通知后做什么”。按钮点击时发出 clicked 信号，连接到它的处理函数会被调用。发送者不必知道接收者内部怎样实现处理逻辑。
```cpp
auto *button = new QPushButton("关闭窗口", this);
connect(button, &QPushButton::clicked,
        this, &QWidget::close);
```
这行代码可以读成：button 发出 clicked 时，调用当前窗口的 close。connect 建立连接时不会立刻关闭窗口；真正触发 clicked 后才调用处理函数。&QPushButton::clicked 是成员函数指针，写连接时不加 ()，否则就变成调用函数。
## 5.2 四个位置怎样记
```cpp
connect(发送对象, &发送类::信号,
        接收对象, &接收类::处理函数);
```
前两个参数回答“谁发出什么通知”，后两个参数回答“由哪个对象执行哪个函数”。在 Widget 成员函数中可以直接写 connect；在普通函数如 main 中通常写 QObject::connect。
clicked 的签名含 bool checked 参数，而 close() 不接收参数。这仍可连接，因为处理函数可以忽略信号末尾的参数。函数指针连接要求对应参数兼容，可存在允许的隐式类型转换；不能把不兼容的参数强行连起来。[6]
## 5.3 槽函数并不都必须返回 void
自定义信号通常声明为 void；槽是普通 C++ 函数，可以有返回值。但普通信号通知不用于收集槽的返回值。例如 QWidget::close() 返回 bool，也能作为这里的处理函数。入门自定义槽写成 void 最直观。
新式 connect 可以连接普通成员函数，也可以连接 Lambda 等可调用对象。若要声明传统元对象槽，可放在 public slots: 或 private slots: 下；不是所有被连接的成员函数都必须写在 slots 区域。[7]
## 5.4 一次信号与多个处理函数
可以一对多、多对一，也可以信号连接信号。同线程直接连接时，同一信号连接的多个处理函数按建立连接的顺序调用。跨线程或队列连接涉及事件投递，不应据此推断不同线程中处理完成的先后顺序。[6][7]

# 6 自定义信号与槽
## 6.1 先定义老师和学生
沿用“老师饿了，学生请吃饭”的例子。把原笔记中的 hungury 统一改为 hungry，修改时声明、连接和调用必须一起改。以下类均继承 QObject，使用 Q_OBJECT，并让构造函数接收父对象。
```cpp
// teacher.h
#pragma once
#include <QObject>
#include <QString>

class Teacher : public QObject
{
    Q_OBJECT
public:
    explicit Teacher(QObject *parent = nullptr)
        : QObject(parent) {}
    void finishClass() { emit hungry(); }
signals:
    void hungry();
    void hungry(const QString &food);
};
```
```cpp
// student.h
#pragma once
#include <QObject>
#include <QString>
#include <QDebug>

class Student : public QObject
{
    Q_OBJECT
public:
    explicit Student(QObject *parent = nullptr)
        : QObject(parent) {}
public slots:
    void treat() { qDebug() << "请老师吃饭"; }
    void treat(const QString &food)
    { qDebug() << "请老师吃饭" << food; }
};
```
这是为了阅读紧凑而把普通函数实现放在头文件中的写法，也可把实现移到 .cpp。signals 下的函数只声明，不自行实现，构建时由 moc 生成相关代码。把两个头文件加入 .pro 的 HEADERS，重新运行 qmake 后构建。[7]

# 6 自定义信号与槽的触发过程
## 6.2 在窗口里把三者连起来
下面代码替换第 3 章 widget.cpp 中的构造函数，并增加相应 include。因为 Teacher 和 Student 已声明了重载函数，连接时需要明确选中无参版本。
```cpp
#include "widget.h"
#include "teacher.h"
#include "student.h"
#include <QPushButton>

Widget::Widget(QWidget *parent) : QWidget(parent)
{
    resize(600, 400);
    auto *button = new QPushButton("下课", this);
    auto *teacher = new Teacher(this);
    auto *student = new Student(this);

    connect(teacher, qOverload<>(&Teacher::hungry),
            student, qOverload<>(&Student::treat));
    connect(button, &QPushButton::clicked,
            teacher, &Teacher::finishClass);
}
```
执行顺序：点击按钮 → clicked 发出 → Teacher::finishClass() 被调用 → emit hungry() → Student::treat() 被调用 → 输出“请老师吃饭”。这里的 emit 是 Qt 提供的宏，用来清楚表达发出信号的意图，不是创建新线程的指令。
teacher 和 student 都传入 this，由窗口负责释放。原来只写 new Teacher()、new Student() 而不另行释放的写法，应补上明确的生命周期管理。
## 6.3 信号也能连接另一个信号
```cpp
// 另一种接法 替换上面第二个 connect
connect(button, &QPushButton::clicked,
        teacher, qOverload<>(&Teacher::hungry));
```
按钮 clicked 发出时会触发老师的无参 hungry。两种接法选一种即可；同时保留会让一次点击走两条路径，学生可能被调用两次。
较复杂的业务更适合先进入 Teacher 的普通方法，再由 Teacher 自己判断和发出信号。这样“什么时候算饿了”的规则集中在 Teacher 中，窗口只负责组织界面。
## 6.4 没有反应时怎样检查
先确认按钮存在且没有被遮挡；再确认 connect 在触发之前执行；确认发送者、接收者仍然存活；最后在中间函数和槽里分别加日志，判断链路停在哪里。加入 Q_OBJECT 后出现 vtable 或信号的链接错误时，检查类是否加入构建、moc 是否生成，以及是否需要重新运行 qmake。[7]

# 7 重载连接与断开连接
## 7.1 为什么重载必须指定版本
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
## 7.2 保存连接句柄再断开
```cpp
QMetaObject::Connection c = connect(
    teacher, qOverload<>(&Teacher::hungry),
    student, qOverload<>(&Student::treat));

QObject::disconnect(c);
```
disconnect 是函数，不是 C++ 关键字。保存返回的句柄，能准确断开这一条连接，尤其适合 Lambda。句柄变量离开作用域本身不会自动断开连接。[6]
发送者或接收者销毁时，相关连接会被自动清理。Lambda 使用带 context 的 connect 时，context 销毁也会使该连接断开；但 Lambda 捕获的其他对象是否仍然存活，仍要自己保证。
## 7.3 重复输出不一定是信号发了两次
同一条连接代码执行两次，默认会建立两条连接，信号发出一次也可能让处理函数运行两次。优先检查初始化函数是否重复调用，避免在反复进入的函数中不断 connect。Qt::UniqueConnection 有适用限制，不能当成所有 Lambda 连接的通用去重开关。[6]

# 8 Lambda 先从普通函数开始
## 8.1 它解决什么问题
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
## 8.2 一段语法分成五部分
```cpp
[捕获列表](参数列表) mutable -> 返回值类型 {
    函数体
}
```
捕获列表回答“如何使用外部局部变量”；参数列表回答“调用时需要传什么”；mutable 决定是否允许修改值捕获的副本；返回类型回答“返回什么”；函数体写实际执行的语句。[8]
最开始只用最简单的 []() { ... }。不要把 mutable 和返回类型当成每次必填的内容。返回类型通常可以由 return 推导，上面的 -> int 可以省略。
## 8.3 定义和执行是两件事
```cpp
auto say = []() { qDebug() << "你好"; };
// 到这里还没有输出
say();  // 此时才执行函数体并输出
```
写出 Lambda 只是创建可调用对象。调用 say() 才运行它的函数体。connect 中也是先登记这段行为，等信号触发后再执行。

# 9 捕获列表和参数列表
## 9.1 先看变量是从哪里来的
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
## 9.2 没捕获就不能随便用外面的局部变量
```cpp
int base = 10;
// 错误示意 base 是外部自动存储期局部变量
// auto f = [](int x) { return base + x; };

auto f = [base](int x) { return base + x; }; // 正确
```
[] 表示没有捕获，不等于函数体什么都不能做。它仍能使用自己的参数、函数体内新建的局部变量，以及无需捕获就可访问的全局变量或静态变量等。
## 9.3 Qt 中两种来源同时出现
```cpp
QString tag = "保存按钮";
connect(button, &QPushButton::clicked, this,
        [tag](bool checked) {
    qDebug() << tag << checked;
});
```
tag 来自创建连接时所在的作用域；checked 来自按钮每次发出的信号。不要把 checked 也写进捕获列表来替代参数，那会改变它的数据来源。普通按钮默认不可勾选，若要观察 checked 切换，可先 button->setCheckable(true)。

# 10 值捕获 保存创建时的副本
## 10.1 外面的值改变 副本不会跟着改变
```cpp
int n = 10;
auto read = [n]() { return n; };
n = 20;
qDebug() << read();  // 10
qDebug() << n;       // 20
```
执行到 [n] 时，Lambda 保存了当时 n 的值 10。之后 n = 20 修改的是外面的变量。调用 read() 读取的是自己的副本，因此结果仍然是 10。
可以把值捕获理解为“拍下当时的值”，但这只是帮助理解的比喻：实际复制什么、复制后共享什么，由被捕获类型自己的复制语义决定。对 int 而言就是保存一个独立数值。
## 10.2 默认不能给值捕获的普通变量赋值
```cpp
int n = 10;
auto read = [n]() {
    // n = 30;  // 编译失败 当前不能修改这个副本
    return n;
};
```
这是 Lambda 默认调用运算符的 const 限制。之后可以用 mutable 允许修改副本，但 mutable 不会把值捕获变成引用捕获。[8]
## 10.3 捕获指针复制的是地址
```cpp
int number = 10;
int *ptr = &number;
auto change = [ptr]() { *ptr = 30; };
change();
qDebug() << number;  // 30
```
副本 ptr 和外面的 ptr 都指向同一个 number。虽然复制了指针，但没有复制它指向的 int。*ptr = 30 改变目标对象，所以外部 number 也变了。这也解释了为什么捕获 this 后可以修改窗口成员。
值捕获不自动延长指针目标的生命。如果捕获的是某个 QWidget*，后面控件先被删除，再使用这个地址仍然危险。学习时要分别问两件事：“复制了什么”和“目标对象还活着吗”。
## 10.4 什么时候适合值捕获
希望保存创建连接时的一段文字、一个编号或一个固定配置时，可以显式值捕获。例如 [tag] 比默认 [=] 更容易看清这段代码依赖了什么。需要读取不断变化的状态时，应考虑引用或对象成员，而不是误以为旧副本会自动更新。

# 11 引用捕获 使用外面的变量本身
## 11.1 修改会反映到外部
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
## 11.2 看懂常见捕获写法
| 写法 | 含义 |
| --- | --- |
| [] | 不捕获外部局部变量 |
| [n] | 按值捕获 n |
| [&n] | 按引用捕获 n |
| [n &m] 的正确写法为 [n, &m] | n 用副本 m 用引用 |
| [=] | 对需要捕获的外部局部变量默认按值捕获 |
| [&] | 对需要捕获的外部局部变量默认按引用捕获 |
| [this] | 保存当前对象的 this 指针 可访问对象成员 |
混合捕获时，逗号分隔每一项。入门优先把所需变量显式写出来，例如 [count, &total]，便于发现外部依赖。默认捕获不等于无条件捕获所在作用域的一切。[8]
## 11.3 最大的风险是变量已经不存在
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

# 12 mutable 修改的是哪个变量
## 12.1 拆解原来的例子
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
## 12.2 改写一下 就能看出副本会保留
```cpp
int m = 10;
auto next = [m]() mutable { return ++m; };
qDebug() << next();  // 11
qDebug() << next();  // 12
qDebug() << m;       // 10
```
对同一个 next 对象连续调用，使用的是同一个内部副本，所以从 10 增到 11，再增到 12，并不是每次调用都重新从外部复制 10。创建 Lambda 和调用 Lambda 要分开理解。
## 12.3 如果想改变外面的 m
```cpp
int m = 10;
auto next = [&m]() { return ++m; };
qDebug() << next();  // 11
qDebug() << m;       // 11
```
这里使用引用捕获，不需要 mutable。记忆时分开两条：值或引用决定“访问的是副本还是外部变量”；mutable 决定“能否修改值捕获副本”。它们解决的是不同问题。
本笔记采用 C++17，带 mutable 的 Lambda 保留空参数列表 ()，写作 [m]() mutable { ... }。这样也能避免混淆不同 C++ 标准下的省略规则。

# 13 this 和窗口成员
## 13.1 this 本来就是一个指针
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
## 13.2 为什么不用 mutable 也能修改成员
[this] 保存的是 this 指针，修改的是它指向的窗口对象的成员，并不是改写这个指针副本。可以联系上一章 [ptr] 修改 *ptr 的例子理解。
不能因此说“值捕获永远不会改变外部状态”。准确说法是：复制 int 后修改副本不会改变原 int；复制指针后，经指针访问的仍可能是同一个对象。
## 13.3 两个 this 扮演不同角色
```cpp
connect(button, &QPushButton::clicked, this, [this]() {
    close();
});
```
第三个参数 this 是连接的上下文对象，参与自动断开和线程上下文选择；[this] 是 C++ Lambda 的捕获，让函数体能访问窗口成员。二者不能相互替代：有 context 不代表自动完成 C++ 捕获；有捕获也不代表自动建立 Qt 生命周期保护。[6]
## 13.4 不要把 this 当成窗口副本
[this] 不复制窗口，也不让窗口永远存活。对于异步保存的回调，要保证对象还存在；Qt 的带 context 连接会在 context 销毁时断开，但保护范围不包括任意捕获对象。
C++17 还有 [*this]，表示复制当前对象。QObject 及其子类通常不可复制，Widget 也不适合这样捕获。Qt 初学阶段优先使用显式 [this]，不依赖 [=] 隐式捕获 this。

# 14 把 Lambda 交给 connect
## 14.1 先写出最小可用版本
```cpp
auto *button = new QPushButton("点击", this);
connect(button, &QPushButton::clicked, this, []() {
    qDebug() << "按钮被按下";
});
```
这里不需要外部变量，因此捕获列表是 []。点击后，Qt 调用保存的函数对象；建立连接时不会执行日志。clicked 虽带 bool 参数，但这个 Lambda 不需要，便可以省略这个参数。
## 14.2 需要窗口成员时改成显式捕获
```cpp
connect(button, &QPushButton::clicked, this, [this]() {
    setWindowTitle("已经点击");
});
```
判断捕获的方法是读函数体：如果只做独立计算或输出固定文本，可能 [] 就够；如果访问窗口成员，使用 [this]；如果只需要创建连接时的一段局部文字，使用 [text]；如果需要临时引用局部变量，先确认生命周期再写 [&value]。
## 14.3 Lambda 为什么适合 Qt 初学
简单处理逻辑可以紧靠连接代码阅读，不必为了两行代码单独声明和实现一个槽函数。当处理流程变长、需要复用、需要单独测试时，应提取成成员函数，再由 Lambda 调用，或直接连接到该成员函数。
```cpp
// 如果已经声明并实现 Widget::saveData()
connect(button, &QPushButton::clicked,
        this, &Widget::saveData);
```
上面是替代写法，不必同时保留两个连接。选择槽成员函数还是 Lambda，依据逻辑是否需要命名和复用，而不是认为某一种语法总是更高级。
## 14.4 四个参数版优先
初学时优先使用带 context 的四参数连接。它让连接跟随上下文对象的生命周期，也使回调执行的线程上下文更明确。即使如此，也要检查 Lambda 捕获的每个指针和引用。[6]
构造界面时常写“一个 new、一个设置属性、一个 connect”。把这些初始化步骤执行一次即可，不要在每次点击后又给同一个按钮不断追加连接。

# 15 参数适配与重载的实际用法
## 15.1 信号给出的参数不一定就是业务参数
老师的带参 hungry 需要 QString，按钮 clicked 提供的是 bool。不能把 bool 当作菜名直接连过去。Lambda 可以把这次点击转换成“请老师吃某道菜”的业务操作。
先在第 6 章 Teacher 的 public 区域增加：
```cpp
void finishClass(const QString &food)
{
    emit hungry(food);
}
```
将第 6 章 Widget 构造函数内创建对象与连接的代码替换为下面内容，不保留原来的连接。这样也避免 finishClass 增加重载后，原来的函数指针写法产生歧义。
```cpp
auto *teacher = new Teacher(this);
auto *student = new Student(this);
auto *button = new QPushButton("请吃饭", this);

connect(teacher,
        qOverload<const QString &>(&Teacher::hungry),
        student,
        qOverload<const QString &>(&Student::treat));

connect(button, &QPushButton::clicked, this, [teacher]() {
    teacher->finishClass(QStringLiteral("红烧排骨"));
});
```
执行链是 clicked → Lambda → finishClass("红烧排骨") → hungry(food) → treat(food)。Lambda 不接收 checked，因为这里用不到它。teacher 被值捕获，复制的是指针，目标仍是那位老师。
本例 teacher 和 button 都由窗口管理，且不提前删除 teacher。若以后加入“单独删除老师”的功能，就需要重新检查连接的上下文和捕获指针，不能原样照搬生命周期假设。
## 15.2 Lambda 可以把选择重载变成普通调用
如果需要调用 student 的带参 treat，可以写一个参数明确的 Lambda，再在里面调用 student->treat(food)。调用表达式有具体实参时，编译器可以按普通 C++ 重载规则选中版本。
不过发送端的重载信号仍要选清楚。Lambda 不是取消所有 qOverload 的开关；它主要让接收逻辑、参数转换、默认值填充等处理更直观。

# 16 延后执行时的三个常见坑
## 16.1 引用捕获了构造函数的局部变量
```cpp
// 错误示意 构造函数结束后 count 不再存在
int count = 0;
connect(button, &QPushButton::clicked, this,
        [&count]() { ++count; });
```
修正方法：把状态放到 Widget 的成员 count_ 中，再用 [this] 访问。窗口还活着时，成员才存在；窗口销毁时，以它为 context 的连接也会断开。完整实现见下一章。
## 16.2 值捕获了循环变量 却以为会读取最终值
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
## 16.3 定义了 Lambda 却没有调用它
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
## 16.4 每写一次捕获 都做一次检查
值捕获：我想保存创建时的值吗？引用捕获：这个变量会不会先消失？指针捕获：目标对象可能提前删除吗？this 捕获：有没有合适的 context，回调是否还被保存到其他地方？先回答这些问题，再选 [] 内的写法。

# 17 综合练习 点击计数窗口
## 17.1 目标与文件组织
做一个窗口，包含“点击加一”和“清零”两个按钮。次数显示在窗口标题中，关闭后由对象树回收按钮。通过这个小例子同时练习成员状态、this 捕获、connect 与函数提取。
创建 qmake Widgets 项目，关闭 Generate form，使用本节给出的 widget.h、widget.cpp，以及第 2 章的 main.cpp。.pro 使用第 2 章的最小配置即可。这个练习不需要 Teacher 或 Student。
```cpp
// widget.h
#pragma once
#include <QWidget>

class Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Widget(QWidget *parent = nullptr);

private:
    void refreshTitle();
    int count_ = 0;
};
```
```cpp
// widget.cpp
#include "widget.h"
#include <QPushButton>
#include <QString>
#include <QDebug>

Widget::Widget(QWidget *parent) : QWidget(parent)
{
    resize(440, 240);
    auto *add = new QPushButton("点击加一", this);
    auto *reset = new QPushButton("清零", this);
    add->setGeometry(50, 80, 140, 40);
    reset->setGeometry(230, 80, 140, 40);

    connect(add, &QPushButton::clicked, this, [this]() {
        ++count_;
        refreshTitle();
        qDebug() << "当前次数" << count_;
    });

    connect(reset, &QPushButton::clicked, this, [this]() {
        count_ = 0;
        refreshTitle();
    });
    refreshTitle();
}

void Widget::refreshTitle()
{
    setWindowTitle(QString("已点击 %1 次").arg(count_));
}
```

# 17 综合练习的执行过程
## 17.2 从创建到退出
① 创建 Widget 时，count_ 初始化为 0，然后执行构造函数。② 构造函数创建两个按钮，父对象都为 this。③ 两次 connect 分别保存两段 Lambda。④ 最后的 refreshTitle() 立即执行，让初始标题显示“已点击 0 次”。
⑤ 点击“点击加一”时，第一个 Lambda 通过 this 访问 count_，先加一再更新标题。⑥ 点击“清零”时，第二个 Lambda 将同一个 count_ 设为 0。两个 Lambda 没有各自复制一份计数，而是在操作同一个窗口成员。
关闭最后一个窗口后，在默认退出行为下，exec() 返回，main 中的 w 随作用域退出而析构，子按钮随它释放。这里没有手动 delete add 或 delete reset。
## 17.3 按顺序做一次手动验证
| 操作 | 预期界面结果 | 观察重点 |
| --- | --- | --- |
| 启动程序 | 标题显示已点击 0 次 | 初始化函数是否执行 |
| 点击加一两次 | 标题依次显示 1 次 2 次 | 状态是否保存在成员中 |
| 点击清零 | 标题回到 0 次 | 两个回调是否操作同一状态 |
| 再点击加一 | 标题显示 1 次 | 清零后的状态是否继续使用 |
| 关闭窗口 | 程序正常退出 | 是否仍有未退出的顶层窗口 |
如果每次点击增加两次，检查是否重复建立连接。如果点击没有变化，在 Lambda 第一行输出日志，确认回调是否执行。如果运行时报访问错误，先回看有没有引用捕获局部计数，或使用已经删除的对象。
## 17.4 为什么把刷新标题提成成员函数
加一与清零都需要同一种显示逻辑，提取 refreshTitle() 可以避免重复格式字符串。Lambda 保留“这次操作改变了什么”，成员函数承担“怎样更新显示”，以后增加 QLabel 时也只需集中修改刷新逻辑。
## 17.5 进一步改一小步
先把“加一”改成“加二”，确认自己知道应该修改哪里。再给清零按钮加入一条日志。最后尝试把加一逻辑提取成 Widget::increase()，使用成员函数指针直接连接，比较它与 Lambda 版本的阅读方式。

# 18 Lambda 自测
## 18.1 先预测输出 再运行
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
## 18.2 把语法放回 Qt 场景
题目四：构造函数里有 int count = 0，然后用 [&count] 连接按钮点击。为什么构造函数执行时不一定出错，点击后却有风险？写一个更合适的状态保存位置。
题目五：下面第三个参数 this 和 [this] 各自负责什么？删除 [this] 后，函数体还能直接访问窗口成员吗？
```cpp
connect(button, &QPushButton::clicked, this, [this]() {
    setWindowTitle("完成");
});
```
题目六：已知 clicked(bool) 的参数是 bool，Teacher::finishClass(const QString &) 需要菜名。请写出用 Lambda 传入“面条”的 connect。
做题时不要求背出所有术语。只要能说清“何时保存数据”“函数体何时执行”“访问的是哪个变量”“它还活着吗”，就掌握了最关键的部分。

# 18 自测参考答案
## 18.3 基础题答案
题目一：依次为 6、100。创建 f 时按值捕获 x = 5；f() 使用副本计算 5 + 1；外部 x 已被赋值为 100。
题目二：使用 [&x]，函数体写 ++x;。引用捕获操作的是外面的 x。若改成 [x]() mutable { ++x; }，只能改变副本，外部仍是 5。
```cpp
int x = 5;
auto f = [&x]() { ++x; };
f();
qDebug() << x;  // 6
```
题目三：依次为 6、7、5。同一个 f 对象保留着自己的 x 副本，两次调用连续修改副本，外部 x 没有改变。
## 18.4 Qt 场景答案
题目四：connect 只是保存回调，构造函数返回前不一定触发点击。等用户点击时，局部 count 已结束生命周期，引用失效。把计数改成 Widget 的成员 int count_ = 0;，然后以窗口为 context，使用 [this] 访问。
题目五：第三个 this 是 Qt 的 context，窗口销毁时相关连接会自动断开；[this] 是 C++ 捕获，让函数体能够访问窗口成员。删除捕获而保留 [] 时，这里的 setWindowTitle() 缺少可用的当前对象，不能这样调用。
题目六：在 teacher 和 button 已经创建、并保证 teacher 在回调执行时仍然存活的前提下：
```cpp
connect(button, &QPushButton::clicked, this, [teacher]() {
    teacher->finishClass(QStringLiteral("面条"));
});
```
该 Lambda 忽略信号的 bool 参数，自己提供 QString 业务参数。它捕获的是 teacher 指针的副本，不会复制 Teacher 对象。
## 18.5 遇到新 Lambda 时的阅读顺序
先看函数体，确定它要做什么；再看参数列表，确定每次输入来自哪里；然后看捕获列表，确认外部依赖的来源与生命期；最后找调用位置，区分立即调用、保存后调用、由 Qt 信号触发调用。
如果读不懂复杂写法，可以先把它保存为 auto task = ...，再单独写 task(...) 测试普通 C++ 行为。理解后再放回 connect，不必同时理解所有语法和事件机制。

# 19 易错点回顾与后续笔记结构
## 19.1 需要记牢的修正
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
## 19.2 以后每个新知识点沿用这个结构
知识点名称：写清要解决什么问题。
基本概念：用自己的话解释它与已有知识的关系。
最小例子：标明代码放在哪个文件、需要哪些头文件。
执行过程：解释从入口到结果发生了什么。
容易出错：记录错误现象、原因、修正办法。
练习与验证：先写预期行为，再记录实际观察和结论。
## 19.3 下一阶段可接着记录
布局管理器用于替代固定坐标；常用控件用于输入与显示；Qt Designer 用于编辑 .ui 界面；资源系统用于管理图片与图标；事件机制用于理解鼠标、键盘和重绘。学到哪一项，就沿用以上结构追加，不必现在一次展开。
复习前面部分时，优先重写第 17 章的计数窗口。如果能够不看答案解释状态存放位置、父对象作用和两个 this 的区别，再进入后续内容会更稳。

# 参考资料
以下为核对概念和继续查阅使用的官方资料。正文中的编号对应这里的链接；不需要一次读完，遇到相关问题时再查。
[1] Qt Creator 配置与项目创建
https://doc.qt.io/qtcreator/creator-configuring.html
https://doc.qt.io/qtcreator/creator-project-creating.html
用于理解 Qt Creator、Qt 版本、编译器和项目向导的关系。
[2] qmake 项目文件
https://doc.qt.io/qt-6/qmake-project-files.html
用于查 QT、CONFIG、SOURCES、HEADERS 等项目变量。
[3] Qt Widgets 教程
https://doc.qt.io/qt-6/widgets-tutorial.html
用于回看 QWidget、show()、QApplication 和事件循环的基础关系。
[4] QWidget 类
https://doc.qt.io/qt-6/qwidget.html
重点查 resize、setFixedSize、move、setGeometry、close 及窗口生命周期说明。
[5] 对象树与所有权
https://doc.qt.io/qt-6/objecttrees.html
重点阅读栈对象构造顺序与父子对象析构示例。
[6] QObject 类
https://doc.qt.io/qt-6/qobject.html
重点查 connect、disconnect、context、连接句柄和参数兼容规则。
[7] 信号与槽
https://doc.qt.io/qt-6/signalsandslots.html
重点查 Q_OBJECT、moc、自定义信号及槽的调用机制。
[8] Microsoft Learn C++ Lambda 表达式
https://learn.microsoft.com/en-us/cpp/cpp/lambda-expressions-in-cpp
重点查捕获列表、参数、mutable 与 this。阅读时注意例子所用 C++ 标准。
