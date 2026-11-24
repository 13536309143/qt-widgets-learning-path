# 6.1 QMainWindow中心工作区

[所属章节](../chapters/06-mainwindow.md) · [完整项目](../../examples/06_01_mainwindow/README.md)

### 本项目要掌握什么

QMainWindow中心工作区。这一节只围绕这个核心问题阅读和修改程序。

### 关键代码在哪里

关键片段 1：[ mainwindow.ui 第 16 行起](../../examples/06_01_mainwindow/mainwindow.ui#L16)。以下是相关界面 XML 节选，完整表单保留在项目中。

```xml
  <widget class="QWidget" name="centralwidget">
   <layout class="QVBoxLayout" name="verticalLayout">
    <item>
     <widget class="QTextEdit" name="textEdit">
      <property name="plainText">
       <string>中心部件就是主窗口的工作区。
试着拖动窗口边缘。</string>
      </property>
     </widget>
    </item>
   </layout>
  </widget>
  <widget class="QMenuBar" name="menubar">
   <widget class="QMenu" name="menuFile">
    <property name="title">
     <string>文件</string>
    </property>
   </widget>
   <addaction name="menuFile" />
  </widget>
  <widget class="QStatusBar" name="statusbar" />
 </widget>
 <resources />
 <connections />
</ui>
```

### 逐段讲解

mainwindow.ui 建立 centralwidget、布局和 textEdit，setupUi 内部把工作区交给主窗口管理。构造函数只需设置状态提示。

主窗口自带菜单、工具栏、停靠和状态栏区域，不能把普通 QWidget 的任意布局直接装到 QMainWindow 顶层。

### 按执行顺序阅读

先从 main.cpp 创建应用和窗口进入构造函数，再由 setupUi 根据 .ui 建立对象、属性与布局。对照上面的片段定位本节设置；show 后进入事件循环，界面才会持续响应操作。

### 容易出错的地方与动手验证

在 centralwidget 布局里增加标签，缩放观察工作区；到 .ui 查看对象层级，区分被创建与被正确放置。

先写下预测，再修改一个条件重新构建。记录实际结果，并用本节代码中的变量、对象和调用顺序解释原因；仅仅没有报错不代表已经验证了所有分支。

### 运行结果

![项目 6.1 运行结果](../../examples/06_01_mainwindow/screenshots/03_result.png)

### 完整项目与界面文件

- [06_01_mainwindow.pro](../../examples/06_01_mainwindow/06_01_mainwindow.pro)
- [main.cpp](../../examples/06_01_mainwindow/main.cpp)
- [mainwindow.cpp](../../examples/06_01_mainwindow/mainwindow.cpp)
- [mainwindow.h](../../examples/06_01_mainwindow/mainwindow.h)
- [mainwindow.ui](../../examples/06_01_mainwindow/mainwindow.ui)
- [verify.h](../../examples/06_01_mainwindow/verify.h)

打开本项目 .pro，选择匹配的 Kit，构建运行；修改界面请打开 .ui，保存后重新构建。verify.h 只在传入 --verify 时执行检查。
