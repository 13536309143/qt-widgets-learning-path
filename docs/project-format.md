[返回十一章目录](README.md)

# 统一项目模板：从 Designer 到 C++

全部 52 个示例采用 Qt Creator 标准 Widgets 项目结构。普通窗口使用 Widget，带菜单、工具栏和状态栏的窗口使用 MainWindow。每个项目都有可编辑的 .ui；自定义对话框和组合控件也有自己的 .ui。

## 先认清每个文件

| 文件 | 应该修改什么 |
|---|---|
| main.cpp | QApplication、创建窗口、show、进入事件循环 |
| widget.h / mainwindow.h | 窗口类、成员、槽和事件函数声明 |
| widget.cpp / mainwindow.cpp | setupUi、信号槽、Lambda 与业务逻辑 |
| widget.ui / mainwindow.ui | Designer 中的控件、objectName、属性、布局、菜单与动作 |
| 项目名.pro | SOURCES、HEADERS、FORMS、RESOURCES 与构建设置 |
| verify.h | 可选运行检查；普通启动时不执行，不必照抄到自己的新项目 |

完整文件都在每个项目目录中，每个目录是一份独立工程。不要将不同项目的 main.cpp 或同名 Widget 类合并到一个可执行程序。

## 在 Qt Creator 中操作

1. 选择“文件 → 打开文件或项目”，打开某个示例的 `.pro`。
2. 选择与本机 Qt 相匹配的 Kit，等待项目加载。
3. 在项目树中展开 Forms，双击 `.ui`，进入“设计”模式。
4. 在对象查看器中选中控件，查看右侧属性编辑器中的 objectName、text、geometry 等属性。每个项目 README 都有对象与布局清单。
5. 修改文字或布局后保存，切回“编辑”，打开对应 `.cpp` 阅读信号槽。
6. 构建并运行。界面由新的 .ui 重新生成；不需要也不应该编辑 ui_widget.h / ui_mainwindow.h。

要从空白模板重做某一步，先拖入该步 README 中列出的控件，设置相同的 objectName，再设置属性和布局，最后补充对应 .h/.cpp 的逻辑。直接使用项目时，这些步骤已替你完成。

## 构造与析构为什么这样写

```cpp
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}
```

初始化列表先构造 QWidget 基类，再创建生成的 Ui::Widget 辅助对象。Ui::Widget 本身不是窗口：它保存 Designer 中各控件的指针，并提供 setupUi。调用 setupUi(this) 后才会建立控件、属性和布局，因此访问 ui->pushButton 等成员必须写在它之后。

ui 指向的辅助对象由窗口类负责 delete。按钮、输入框等 QWidget 实例主要通过 QObject 父子关系管理，不需要把每一个 ui->控件指针再手动 delete 一遍。不要删完 ui 后继续在析构函数中用 ui 访问控件。

## .ui 放外观，.cpp 放行为

固定按钮、标签、输入框、页面、菜单、QAction 和布局放入 .ui。Lambda 的定义和调用、运行时状态、文件读写、事件处理与 QPainter 绘制留在 .cpp。

Qt 标准文件、颜色和字体对话框由 Qt 提供，不需要复制它们内部的 .ui。示例 36 和 50的自定义对话框使用 dialog.ui。QStatusBar 的普通/永久状态控件需要调用 addWidget / addPermanentWidget，演示代码保留了这部分动态添加操作。

示例 01—30使用 geometry 放置演示控件，便于观察坐标和 Lambda 输出；示例 40集中学习布局，后续项目采用可随窗口变化的布局。没有为早期固定坐标练习偷偷添加布局，因为布局会覆盖手工位置。

## 两种连接方式分别在哪里

大多数示例显式调用 connect，便于追踪信号、接收对象和 Lambda。访问成员 ui 时使用 [this]；它捕获当前窗口指针，不会复制整个界面。

示例 35单独示范 Designer 常见的自动连接：objectName 为 runButton 的按钮，对应声明在 private slots 下的 on_runButton_clicked()。setupUi 最后调用 connectSlotsByName。不要再为同一个槽重复写 connect，否则会执行两次。改变 objectName 后也必须同步改槽名。

第 36、50 步 dialog.ui 的“信号/槽编辑器”连接 buttonBox 的 accepted() → Dialog::accept()、rejected() → Dialog::reject()，可以直接在 Designer 中查看。

## 自定义控件如何在 Designer 中显示

示例 43在 mainwindow.ui 中放置两个 QWidget 占位，分别提升为 SmallWidget，提升头文件为 smallwidget.h。SmallWidget 内部的 QSpinBox、QSlider 则在 smallwidget.ui 中设计。两个实例各自拥有自己的 Ui 和数值。

示例 45—47将 QLabel 提升为 EventArea，头文件 eventarea.h。示例 48将 QWidget 提升为 Canvas，头文件 canvas.h；示例 52将 QWidget 提升为 DrawingArea，头文件 drawingarea.h。提升信息已经保存在对应 .ui 的 customwidgets 中。

从空模板重做时，在占位控件上右键选择“提升为”，输入准确的类名和头文件名，添加后点击提升。自定义类必须提供接受 QWidget *parent 的构造函数，并在 .pro 的 SOURCES/HEADERS 中加入实现。Designer 中显示占位区域是正常的，自己的 paintEvent 会在运行后绘制。

## 编译时发生什么

qmake 从 FORMS 读取 .ui，再让 uic 生成 ui_*.h；从带 Q_OBJECT 的头文件生成 moc 代码。CMake 入口同时开启 AUTOUIC、AUTOMOC 和 AUTORCC。两种入口使用相同的源文件和表单。

如果提示 ui_xxx.h 找不到，检查 .pro 的 FORMS 和 #include 名称，重新运行 qmake 再构建。如果提示某个 ui->成员不存在，检查 Designer 的 objectName，保存 .ui 后重新构建。不要通过手改生成文件暂时消除错误。
