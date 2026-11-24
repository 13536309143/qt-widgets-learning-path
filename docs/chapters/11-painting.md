# 项目 5.6 章 绘图和绘图设备

本章按项目逐个学习，每个项目对应一个知识点。先运行再读关键代码，最后做一次修改验证。[理论补充与复习](../foundations/11-painting.md)保留更完整的背景说明。

## 11.1 paintEvent与绘图设备

[单独阅读本知识点](../knowledge/11_01_painter.md) · [打开完整项目](../../examples/11_01_painter/README.md)

### 本项目要掌握什么

paintEvent与绘图设备。这一节只围绕这个核心问题阅读和修改程序。

### 关键代码在哪里

关键片段 1：[ canvas.cpp 第 9 行起](../../examples/11_01_painter/canvas.cpp#L9)。以下代码从本项目源文件提取，完整上下文见源文件。

```cpp
void Canvas::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), Qt::white);
    painter.setPen(QPen(Qt::darkGray, 2));
    painter.drawLine(35, 35, 500, 270);
    painter.setPen(QPen(Qt::red, 3));
    painter.drawRect(60, 70, 360, 180);
    painter.setPen(QPen(Qt::blue, 5));
    painter.drawEllipse(60, 70, 360, 180);
    painter.setPen(Qt::black);
    painter.drawText(50, 320, "QPainter 在 paintEvent 中绘制，缩放和遮挡后仍可重绘。");
    QImage image(180, 80, QImage::Format_ARGB32);
    image.fill(QColor("#dbeafe"));
    {
        QPainter imagePainter(&image);
        imagePainter.setPen(Qt::darkBlue);
        imagePainter.drawText(image.rect(), Qt::AlignCenter, "QImage 离屏绘图");
    }
    painter.drawImage(50, 360, image);
}
```

### 逐段讲解

QPainter 在 paintEvent 内绑定 Canvas，设置画笔后绘制线条、矩形、椭圆和文字；它是局部对象，函数退出时结束本次绘制。

QImage 是另一个绘图目标，内层作用域结束 imagePainter 后，再把图像画到窗口；设备、画笔状态和屏幕显示不是同一概念。

### 按执行顺序阅读

窗口初始化先准备对象和连接；用户输入、尺寸变化或重绘请求到达后，才进入上述处理函数。依次追踪事件或信号的目标、类型判断、状态变化和结果更新，再观察是否把剩余处理交回基类。

### 容易出错的地方与动手验证

缩放、遮挡后恢复观察重绘；不要保存长期活动的 QPainter，或只在按钮槽临时画一次就期望永久存在。

先写下预测，再修改一个条件重新构建。记录实际结果，并用本节代码中的变量、对象和调用顺序解释原因；仅仅没有报错不代表已经验证了所有分支。

### 运行结果

![项目 11.1 运行结果](../../examples/11_01_painter/screenshots/03_result.png)

### 完整项目与界面文件

- [11_01_painter.pro](../../examples/11_01_painter/11_01_painter.pro)
- [canvas.cpp](../../examples/11_01_painter/canvas.cpp)
- [canvas.h](../../examples/11_01_painter/canvas.h)
- [main.cpp](../../examples/11_01_painter/main.cpp)
- [mainwindow.cpp](../../examples/11_01_painter/mainwindow.cpp)
- [mainwindow.h](../../examples/11_01_painter/mainwindow.h)
- [mainwindow.ui](../../examples/11_01_painter/mainwindow.ui)
- [verify.h](../../examples/11_01_painter/verify.h)

打开本项目 .pro，选择匹配的 Kit，构建运行；修改界面请打开 .ui，保存后重新构建。verify.h 只在传入 --verify 时执行检查。


## 11.2 状态更新与重绘分离

[单独阅读本知识点](../knowledge/11_02_interactive_paint.md) · [打开完整项目](../../examples/11_02_interactive_paint/README.md)

### 本项目要掌握什么

状态更新与重绘分离。这一节只围绕这个核心问题阅读和修改程序。

### 关键代码在哪里

关键片段 1：[ widget.cpp 第 6 行起](../../examples/11_02_interactive_paint/widget.cpp#L6)。以下代码从本项目源文件提取，完整上下文见源文件。

```cpp
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(ui->toggleButton, &QPushButton::clicked, this,
            [this]()
            {
                auto *canvas = ui->drawingArea;
                canvas->setColor(canvas->color() == QColor(Qt::blue) ? QColor(Qt::red)
                                                                     : QColor(Qt::blue));
            });
}
```

关键片段 2：[ drawingarea.cpp 第 10 行起](../../examples/11_02_interactive_paint/drawingarea.cpp#L10)。以下代码从本项目源文件提取，完整上下文见源文件。

```cpp
void DrawingArea::setColor(const QColor &color)
{
    color_ = color;
    update();
}
```

关键片段 3：[ drawingarea.cpp 第 21 行起](../../examples/11_02_interactive_paint/drawingarea.cpp#L21)。以下代码从本项目源文件提取，完整上下文见源文件。

```cpp
void DrawingArea::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), Qt::white);
    painter.setPen(QPen(color_, 5));
    painter.setBrush(QColor(color_.red(), color_.green(), color_.blue(), 45));
    const QRectF area = QRectF(rect()).adjusted(40, 40, -40, -40);
    painter.drawEllipse(area);
}
```

### 逐段讲解

按钮修改 DrawingArea 的 color_，setColor 调用 update 请求稍后重绘；paintEvent 每次根据当前状态和当前 rect 重新构建椭圆。

颜色保存在成员中，QPainter 只在绘制时短暂存在。窗口缩放或遮挡后再次绘制，使用的仍是最新颜色。

### 关键分支再拆一步

setColor 的赋值负责保存新状态，update 只安排以后绘制，不能假定它立即调用一次 paintEvent。重绘时先清背景，再按当前 color_ 建立 QPen 和带透明度的画刷，椭圆区域从当前 rect 缩进得到，因此颜色与尺寸可以在任意后续重绘中恢复。

### 按执行顺序阅读

窗口初始化先准备对象和连接；用户输入、尺寸变化或重绘请求到达后，才进入上述处理函数。依次追踪事件或信号的目标、类型判断、状态变化和结果更新，再观察是否把剩余处理交回基类。

### 容易出错的地方与动手验证

增加线宽成员并使用同样流程更新；不要从 paintEvent 无条件调用 update，避免持续无效重绘。

先写下预测，再修改一个条件重新构建。记录实际结果，并用本节代码中的变量、对象和调用顺序解释原因；仅仅没有报错不代表已经验证了所有分支。

### 运行结果

![项目 11.2 运行结果](../../examples/11_02_interactive_paint/screenshots/result.png)

### 完整项目与界面文件

- [11_02_interactive_paint.pro](../../examples/11_02_interactive_paint/11_02_interactive_paint.pro)
- [drawingarea.cpp](../../examples/11_02_interactive_paint/drawingarea.cpp)
- [drawingarea.h](../../examples/11_02_interactive_paint/drawingarea.h)
- [main.cpp](../../examples/11_02_interactive_paint/main.cpp)
- [verify.h](../../examples/11_02_interactive_paint/verify.h)
- [widget.cpp](../../examples/11_02_interactive_paint/widget.cpp)
- [widget.h](../../examples/11_02_interactive_paint/widget.h)
- [widget.ui](../../examples/11_02_interactive_paint/widget.ui)

打开本项目 .pro，选择匹配的 Kit，构建运行；修改界面请打开 .ui，保存后重新构建。verify.h 只在传入 --verify 时执行检查。



[返回总目录](../README.md)
