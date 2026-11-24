# 3.2 资源路径与图像加载

[教程中的本知识点](../../docs/knowledge/03_02_resources.md) · [第 3 章](../../docs/chapters/03-window-properties.md)

### 本项目要掌握什么

资源路径与图像加载。这一节只围绕这个核心问题阅读和修改程序。

### 关键代码在哪里

关键片段 1：[ mainwindow.cpp 第 12 行起](mainwindow.cpp#L12)。以下代码从本项目源文件提取，完整上下文见源文件。

```cpp
QPixmap picture(":/images/demo.png");
if (picture.isNull())
    ui->imageLabel->setText("图片加载失败，请检查 qrc 资源路径。");
else
    ui->imageLabel->setPixmap(
        picture.scaled(600, 360, Qt::KeepAspectRatio, Qt::SmoothTransformation));
setWindowIcon(QIcon(":/images/demo.png"));
```

### 逐段讲解

:/images/demo.png 是 qrc 的资源路径，不是磁盘路径；构建系统把资源编入程序。isNull 检查图片是否成功加载。

scaled 返回处理后的图像，KeepAspectRatio 保留比例，SmoothTransformation 控制缩放质量；随后设置标签图像和窗口图标。

### 按执行顺序阅读

构造函数先准备界面与连接；交互发生后才进入回调。阅读时按“发送对象 → 信号或调用 → 回调里的数据 → 被修改对象 → 显示结果”追踪，不要把建立连接当成已经执行操作。

### 容易出错的地方与动手验证

改错别名观察失败提示，再恢复。构造时缩放一次不会自动适应以后所有窗口尺寸。

先写下预测，再修改一个条件重新构建。记录实际结果，并用本节代码中的变量、对象和调用顺序解释原因；仅仅没有报错不代表已经验证了所有分支。

### 运行结果

![项目 3.2 运行结果](screenshots/03_result.png)

### 完整项目与界面文件

- [03_02_resources.pro](03_02_resources.pro)
- [assets.qrc](assets.qrc)
- [main.cpp](main.cpp)
- [mainwindow.cpp](mainwindow.cpp)
- [mainwindow.h](mainwindow.h)
- [mainwindow.ui](mainwindow.ui)
- [verify.h](verify.h)

打开本项目 .pro，选择匹配的 Kit，构建运行；修改界面请打开 .ui，保存后重新构建。verify.h 只在传入 --verify 时执行检查。

## 在 Designer 中重做界面

从空白 Widgets 模板开始时，按下表拖入控件并设置相同的 objectName。已有完整 .ui 可直接使用；geometry 只适用于没有布局管理的控件。

### mainwindow.ui

| objectName | 类型 | 主要设置 |
|---|---|---|
| `MainWindow` | QMainWindow | geometry=(0,0,800,580) |
| `centralwidget` | QWidget | 按 .ui 中的默认属性 |
| `imageLabel` | QLabel | 按 .ui 中的默认属性 |
| `verticalLayout` | QVBoxLayout | 布局内的控件交给布局分配空间 |
