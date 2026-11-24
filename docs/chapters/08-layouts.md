# 第 8 章 布局管理器

本章按项目逐个学习，每个项目对应一个知识点。先运行再读关键代码，最后做一次修改验证。[理论补充与复习](../foundations/08-layouts.md)保留更完整的背景说明。

## 8.1 四种布局的嵌套

[单独阅读本知识点](../knowledge/08_01_layout.md) · [打开完整项目](../../examples/08_01_layout/README.md)

### 本项目要掌握什么

四种布局的嵌套。这一节只围绕这个核心问题阅读和修改程序。

### 关键代码在哪里

关键片段 1：[ mainwindow.ui 第 16 行起](../../examples/08_01_layout/mainwindow.ui#L16)。以下是相关界面 XML 节选，完整表单保留在项目中。

```xml
  <widget class="QWidget" name="centralwidget">
   <layout class="QVBoxLayout" name="verticalLayout">
    <item>
     <layout class="QFormLayout" name="formLayout">
      <item row="0" column="0">
       <widget class="QLabel" name="nameEditLabel">
        <property name="text">
         <string>用户名</string>
        </property>
       </widget>
      </item>
      <item row="0" column="1">
       <widget class="QLineEdit" name="nameEdit" />
      </item>
      <item row="1" column="0">
       <widget class="QLabel" name="noteEditLabel">
        <property name="text">
         <string>备注</string>
        </property>
       </widget>
      </item>
      <item row="1" column="1">
       <widget class="QLineEdit" name="noteEdit" />
      </item>
     </layout>
    </item>
    <item>
     <layout class="QGridLayout" name="gridLayout">
      <item row="0" column="0">
       <widget class="QPushButton" name="number1">
        <property name="text">
         <string>1</string>
        </property>
       </widget>
      </item>
```

### 逐段讲解

mainwindow.ui 中 verticalLayout 依次管理 formLayout、gridLayout 和 buttonLayout；form 的行列配对标签与输入，grid 排列六个按钮。

底部水平布局的 spacer 吸收空余空间，外层 margins 与 spacing 控制留白。窗口尺寸变化后，布局根据约束重新分配位置。

### 按执行顺序阅读

先从 main.cpp 创建应用和窗口进入构造函数，再由 setupUi 根据 .ui 建立对象、属性与布局。对照上面的片段定位本节设置；show 后进入事件循环，界面才会持续响应操作。

### 容易出错的地方与动手验证

逐项修改 spacing、边距与 stretch，再试固定按钮大小；不要同时用布局和 move 强行控制同一位置。

先写下预测，再修改一个条件重新构建。记录实际结果，并用本节代码中的变量、对象和调用顺序解释原因；仅仅没有报错不代表已经验证了所有分支。

### 运行结果

![项目 8.1 运行结果](../../examples/08_01_layout/screenshots/03_result.png)

### 完整项目与界面文件

- [08_01_layout.pro](../../examples/08_01_layout/08_01_layout.pro)
- [main.cpp](../../examples/08_01_layout/main.cpp)
- [mainwindow.cpp](../../examples/08_01_layout/mainwindow.cpp)
- [mainwindow.h](../../examples/08_01_layout/mainwindow.h)
- [mainwindow.ui](../../examples/08_01_layout/mainwindow.ui)
- [verify.h](../../examples/08_01_layout/verify.h)

打开本项目 .pro，选择匹配的 Kit，构建运行；修改界面请打开 .ui，保存后重新构建。verify.h 只在传入 --verify 时执行检查。



[返回总目录](../README.md)
