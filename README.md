<div align="center">

# Qt Widgets Learning Path

**从第一扇窗口，到能解释、修改和验证的桌面交互。**

一份中文 Qt Widgets 学习教程：统一的 Qt Creator 项目模板、可编辑的 Designer 界面、完整源码、逐步讲解与运行截图。

[![Build and verify](https://github.com/13536309143/qt-widgets-learning-path/actions/workflows/build.yml/badge.svg)](https://github.com/13536309143/qt-widgets-learning-path/actions/workflows/build.yml)
![Qt 6](https://img.shields.io/badge/Qt-6-41CD52?logo=qt&logoColor=white)
![C++17](https://img.shields.io/badge/C%2B%2B-17-00599C?logo=cplusplus)
![Examples](https://img.shields.io/badge/examples-52-2563EB)

[开始学习](docs/README.md) · [项目模板与 UI 操作](docs/project-format.md) · [离线入口](开始学习.html) · [Word 教程](docs/print/Qt学习教程_统一模板版.docx) · [安装与运行](docs/getting-started.md) · [完整实战](docs/practices.md) · [验证记录](docs/validation.md)

</div>

## 这份教程怎样学

所有项目按 Qt Creator 标准模板拆分：`main.cpp`、`widget/mainwindow.h`、对应 `.cpp` 与 `.ui`。每个项目都保留一个明确的学习目标：先观察效果，再理解对象与数据怎样流动，最后修改一个条件验证自己的判断。前半部分建立窗口、对象生命周期、信号槽与 Lambda 的基础；后半部分继续进入主窗口、对话框、布局、控件、事件和绘图。

- **52 个统一模板项目**：main.cpp 只启动窗口，逻辑拆入 Widget/MainWindow 类，每步有自己的 `.pro`。
- **55 份可编辑表单**：52 个主界面，以及组合控件和自定义对话框；固定控件、菜单和布局在 `.ui` 中，源码目录不提交生成的 `ui_*.h`。
- **18 章深入讲解**：对后半部分补齐执行过程、参数边界、错误原因、动手练习与自测答案。
- **4 个操作闭环**：文件读写与保存、对话框调用对比、事件过滤、状态驱动绘图。
- **两种构建入口**：保留 qmake 单步学习，也提供 CMake 批量构建与 CTest 检查。
- **真实运行截图**：截图与对应代码放在一起，可对照操作前后的状态。

教程适合具备基本 C++ 类、指针、引用与函数概念，正在学习 Qt Widgets 的读者。主要内容围绕桌面界面编程；后续可以继续补充线程、网络、数据库、模型视图和工程部署。

## 看看运行结果

<table>
<tr>
<td width="50%"><img src="examples/49_text_editor/screenshots/result.png" alt="UTF-8 文本编辑器"><br>文件读取、编辑与安全保存</td>
<td width="50%"><img src="examples/52_interactive_paint/screenshots/result.png" alt="交互绘图"><br>修改状态后请求重绘</td>
</tr>
</table>

## 快速开始

### 在 Qt Creator 中运行一个项目

```sh
git clone https://github.com/13536309143/qt-widgets-learning-path.git
```

打开 `examples/01_first_window/01_first_window.pro`，选择与已安装 Qt 配套的 Kit，构建并运行。双击项目树中的 `widget.ui` 进入 Designer；按每个项目 README 的控件清单修改界面。想直接看完整应用交互，可以打开 `examples/49_text_editor/49_text_editor.pro`。

### 用 CMake 构建全部项目

需要 Qt 6 Widgets 开发库、支持 C++17 的编译器和 CMake 3.16+。将路径替换成自己的 Qt 安装目录：

```sh
cmake -S . -B build -DCMAKE_PREFIX_PATH="你的 Qt 安装目录"
cmake --build build --parallel 4
ctest --test-dir build --output-on-failure
```

只构建一个示例：

```sh
cmake -S . -B build-one -DQT_LESSON=49_text_editor -DCMAKE_PREFIX_PATH="你的 Qt 安装目录"
cmake --build build-one
```

Windows 上注意 Qt 与编译器的 MinGW/MSVC、32/64 位及运行库匹配。详细配置与常见错误见 [安装与运行](docs/getting-started.md)。

## 学习路线

| 阶段 | 项目 | 重点 |
|---|---|---|
| 创建与通信 | 01—11 | 项目结构、坐标、对象树、信号与槽 |
| Lambda 基础 | 12—20 | 调用时机、值捕获、引用捕获、mutable |
| 回调与状态 | 21—30 | this、参数适配、生存期、计数与自测 |
| 主窗口与界面 | 31—35 | 菜单、QAction、工具栏、资源、Designer |
| 对话框与布局 | 36—40 | 模态、取消处理、标准对话框、布局约束 |
| 控件与事件 | 41—47 | 输入、组合控件、页面、事件分发与焦点 |
| 绘图与实战 | 48—52 | 绘图设备、文件保存、过滤器、交互重绘 |

建议从 [基础与 Lambda](docs/basics.md) 开始；后半部分从 [第 20 章 主窗口](docs/chapters/20-mainwindow.md) 继续。全部章节与项目映射见 [学习目录](docs/README.md)。

## 每章不止一段代码

后半部分统一包含：要解决的问题 → 核心概念 → 执行顺序 → 参数和边界 → 完整源码 → 运行结果 → 常见错误 → 动手练习与答案。

例如，文件对话框一章会继续追到文件读取、UTF-8 编解码、写入失败和未保存关闭；绘图一章会解释为什么需要保存状态、调用 `update()`，以及为什么遮挡后仍能正确重绘。

## 仓库结构

```text
.
├── examples/                 # 52 个可独立构建的项目
│   ├── 01_first_window/
│   ├── 37_file_dialog/       # .pro、完整源码及 samples
│   ├── 49_text_editor/       # 文件读写与保存闭环
│   └── 52_interactive_paint/
├── docs/
│   ├── basics.md            # 基础与 Lambda
│   ├── chapters/            # 第 20—37 章详细教程
│   ├── practices.md         # 4 个完整实战
│   ├── getting-started.md
│   └── validation.md
├── tests/local-results.json # 本机验证摘要
├── scripts/run.cmd          # Windows 已构建项目启动入口
└── CMakeLists.txt           # 全部项目的构建与检查入口
```

构建产物、个人 Qt 路径、Qt SDK 和原始课堂文档不属于仓库内容。每步的 `.ui`、qrc、图片和 GIF 则与代码一起提供。

## 验证情况

本机 Windows / Qt 6.12.0 / MinGW 13.1 / Release 环境下，52 个项目编译成功，52 项运行检查通过。部分早期检查是启动与截图的冒烟检查，后面的实战增加了文件读回、对话框顺序、事件处理和绘制像素等断言。

GitHub Actions 使用 Ubuntu 的 Qt 开发包重新构建和运行。跨平台结果以 Actions 实际执行为准；验证范围、手动练习项和环境差异见 [验证记录](docs/validation.md)。

## 资料与维护

本文以自己的学习顺序组织，并用 [Qt 官方文档](https://doc.qt.io/qt-6/qtwidgets-index.html) 校对 API 行为。具体章节提供对应原始文档链接。Qt 及相关商标属于其权利人。

发现错误欢迎提交 Issue；改进说明见 [CONTRIBUTING](CONTRIBUTING.md)。项目目前公开供阅读，尚未指定统一的代码与文档复用许可，公开可见不等同于已授予开源许可证。
