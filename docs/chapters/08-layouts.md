# 第八章 布局管理器

理解布局如何管理空间，集中比较水平、垂直、网格和表单布局，再学习边距、间距和伸缩项。

本章对应的完整项目：

- [示例 40 layout](../../examples/40_layout/README.md)

## 8.1 水平 垂直 网格与表单布局

对应示例：[完整源码](../../examples/40_layout)。先运行项目观察行为，再按以下顺序阅读。

### 8.1.1 本章要解决的问题

绝对定位要给控件逐个设置坐标和尺寸；布局则描述控件关系，由布局管理器在可用区域改变时重新安排。QHBoxLayout 从左到右，QVBoxLayout 从上到下，QGridLayout 按行列定位，QFormLayout 适合标签与输入框成对排列。

父布局用 addLayout 嵌入子布局，控件用 addWidget 加入。addStretch 放可伸缩空白；setContentsMargins 控制布局与外边界间距；setSpacing 控制相邻项目间距。stretch 是分配额外空间的权重，不是承诺最终宽高严格按比例分配，还会受 sizeHint、最小尺寸和 sizePolicy 约束。

### 8.1.2 布局管理的是空间约束

布局会综合 available space、sizeHint、minimumSize、maximumSize、sizePolicy 和 stretch 来分配空间。它不是简单把窗口按比例切成几个盒子。一个按钮即使 stretch 为 1，也可能由于样式或尺寸策略不愿竖向拉高；多行编辑器通常适合承担更多可伸缩空间。

### 8.1.3 按执行顺序拆解

打开 mainwindow.ui，在 centralwidget 下找到 verticalLayout，内部依次是 formLayout、gridLayout 和 buttonLayout。表单项设置 row/column，网格放置六个按钮，底部水平布局包含伸缩占位和确定/取消按钮。外层边距为 20、spacing 为 14。C++ 不重复创建布局。

### 8.1.4 参数与行为边界

contentsMargins 是容器内缘留白，spacing 是相邻项目间隔，addSpacing 是固定间隔，addStretch 是可伸缩空白。QGridLayout 的 rowSpan 和 columnSpan 允许跨行列。QFormLayout 不等于 QTableWidget：前者安排控件，后者显示可编辑表格数据。

### 8.1.5 易错点与排查

布局不是可见控件。控件加入布局后不要再依赖 move()/resize() 固定摆放。不要给同一 QWidget 安装两个顶层布局；QMainWindow 先设置 centralWidget，再给这个容器安装布局。

遇到问题时先区分三个层次：编译错误检查声明、头文件与构建配置；界面无反应检查连接、接收对象与生存期；运行结果不对检查输入、返回值和分支是否真正执行。可以先在连接入口记录一条日志，再在结果处理处记录一条，定位中间哪一步没有到达。

### 完整代码与界面操作

[本节完整源码与深入讲解](../topics/layout-management.md) · [独立项目与 Designer 步骤](../../examples/40_layout/README.md)

![水平 垂直 网格与表单布局运行结果](../../examples/40_layout/screenshots/03_result.png)

### 动手练习

把编辑器放到垂直布局中，设置 addWidget(editor,1)，底部按钮行不设置伸展。把窗口拉宽拉高，观察谁吸收额外空间。再把某个按钮 setFixedSize，检查它如何限制布局，理解固定大小不应滥用。

先写下预期行为，再修改代码、重新构建，最后对照实际结果。程序没有报错不等于逻辑已经正确，尤其要测试取消、空输入、重复操作和失败分支。

### 自测与答案

**一个控件可以同时加入两个布局吗？**

同一个控件只有一个实际父层级与几何位置，不能被两个布局当作两个副本使用。需要重复显示时应创建多个控件实例并同步数据。

## 8.2 四种布局放在一起比较

| 布局 | 排列方式 | 对照示例40 |
|---|---|---|
| QHBoxLayout | 一行中横向排列 | 底部确定与取消按钮 |
| QVBoxLayout | 一列中纵向排列 | centralwidget 的外层布局 |
| QGridLayout | 通过行列组织控件 | 六个数字按钮 |
| QFormLayout | 标签与输入控件形成行 | 用户名和备注输入 |

## 8.3 在Designer里重做布局

先放置控件并设置 objectName，再选择相应控件设置子布局，最后给 centralwidget 设置外层布局。示例 40 中，外层 verticalLayout 包含 formLayout、gridLayout、buttonLayout。检查对象查看器中的嵌套关系，而不只看屏幕上是否排列整齐。

## 8.4 空间分配的观察方法

改变窗口尺寸、按钮文字和字体，观察 available space、sizeHint、minimumSize、maximumSize、sizePolicy 与 stretch 如何共同影响结果。stretch 不是保证控件最终像素宽度严格成比例的开关；setFixedSize 会限制控件的伸缩。

把边距和间距分开修改，再调整伸缩项，避免一次修改多个变量后分不清是哪项起作用。组合控件、堆叠页面和绘图区域也使用布局，可在第九、十一章继续观察。


---

[总目录](../README.md) · [上一章](07-dialogs.md) · [下一章](09-widgets.md)
