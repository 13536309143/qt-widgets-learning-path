# 第四章 按钮的创建和属性设置

学会在 Designer 和 C++ 中创建按钮、设置属性，并理解按钮的位置和父子对象关系。按钮如何触发操作在第五章展开。

本章对应的完整项目：

- [示例 03 buttons_coordinates](../../examples/03_buttons_coordinates/README.md)
- [示例 04 object_tree](../../examples/04_object_tree/README.md)
- [示例 05 stack_lifetime](../../examples/05_stack_lifetime/README.md)

## 4.1 在Designer中创建按钮

打开 widget.ui，从控件箱拖入 QPushButton，设置 objectName 和 text。示例 03 使用 firstButton、secondButton 两个对象名，按钮的 geometry 分别表示它相对父控件的位置和大小。完整配置见示例 README 的控件清单。

生成的 ui->firstButton 是按钮指针，可在 setupUi 后使用。不要因为按钮的显示文字改变，就把它误认为另一个对象。

## 4.2 用代码创建按钮

```cpp
auto *button = new QPushButton("第一个按钮", this);
button->move(100, 80);
button->resize(140, 40);
```

这是独立的代码创建练习。若现有 .ui 已经有同样作用的按钮，直接使用 ui->对应对象名，避免再创建一份重叠的控件。this 指向当前窗口，并作为按钮的父控件。

## 4.3 常用属性怎样设置

| 属性或函数 | 作用 |
|---|---|
| text / setText | 按钮显示文字 |
| objectName / setObjectName | 代码识别与自动连接使用的对象名 |
| geometry / setGeometry | 没有布局管理时的位置与大小 |
| resize / setFixedSize | 当前尺寸或固定尺寸 |
| checkable / setCheckable | 是否允许保持勾选状态 |

按钮启用 checkable 后，clicked 的 checked 参数与状态有关。这个参数怎样传入 Lambda，结合第五章的示例 23 学习。

## 4.4 父控件坐标与布局的关系

子控件坐标以父控件内容区域左上角为原点，x 向右、y 向下。move(100, 80) 表示按钮左上角距离父控件左侧 100、上侧 80 个逻辑像素，不是桌面坐标。

固定坐标便于入门观察；安装布局后，应让布局分配控件空间。不要一边让布局管理按钮，一边期待 move/setGeometry 持续控制它的位置。布局管理器在第八章集中学习。

## 4.5 父子对象与生命周期

### 4.5.1 谁负责释放 new 出来的对象
QObject 的父子关系构成对象树。父对象析构时，会删除仍属于自己的子对象；子对象先析构时，会把自己从父对象的子对象列表移除。QWidget 在此基础上还提供可视控件的父子关系。[参考5](https://doc.qt.io/qt-6/objecttrees.html)
```cpp
auto *button = new QPushButton("按钮", this);
// this 析构时负责删除 button 指向的对象
// 局部指针 button 离开作用域 不会删除按钮对象
```
局部指针变量和它指向的对象是两回事。构造函数结束后，button 这个局部变量消失了，但堆上的按钮继续存在。后面还要访问它，可以把指针保存为成员变量。
没有 parent 的 new QObject 或 new Teacher 不会因为“使用了 Qt”就自动回收。初学时，让属于窗口的 QObject 在创建时接收 this 作为父对象。这个机制不管理所有 C++ 类型，例如 QString 是值类型，按自己的 C++ 生命周期管理。
### 4.5.2 栈对象必须注意构造顺序
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
这里 window 后构造、先析构，会试图删除作为子对象的 quit；但 quit 的存储属于栈，不能由父对象用 delete 释放，随后还存在自动析构问题，属于未定义行为，可能崩溃。Qt 的父子管理不能替代 C++ 的存储期规则。[参考5](https://doc.qt.io/qt-6/objecttrees.html)
### 4.5.3 用日志观察析构
可以定义继承 QPushButton 的 MyButton，在构造函数打印 qDebug() << "MyButton"，在析构函数打印 qDebug() << "~MyButton"。把它 new 出来并传 this，正常结束程序时观察构造与析构输出。
关闭窗口不一定立刻删除窗口对象。main 中的栈窗口通常在 exec() 返回后析构；堆窗口是否随关闭删除还受 WA_DeleteOnClose 等设置影响。判断释放时机时，应追踪对象析构，不能只看窗口是否消失。[参考4](https://doc.qt.io/qt-6/qwidget.html)

## 4.6 本章练习

在示例 03 中改变两个按钮的 geometry，观察位置变化；运行示例 04、05，写出父子对象的析构过程。区分局部指针变量消失、窗口隐藏、控件关闭和对象真正析构这几件事。


---

[总目录](../README.md) · [上一章](03-window-properties.md) · [下一章](05-signals-slots-lambda.md)
