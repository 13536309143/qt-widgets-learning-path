# 从下载到第一次运行

## 先准备什么

需要 Qt 6 Widgets 开发库、支持 C++17 的编译器，以及 Qt Creator。使用 CMake 时还需 CMake 3.16 或更新版本和构建工具。Windows 上 Qt 的 MinGW 版本应配套 MinGW 编译器；MSVC 版本应配套相应 MSVC 工具链，不能把两套二进制混用。

本机验证环境为 Windows、Qt 6.12.0、MinGW 13.1、C++17、Release。其他 Qt 6 版本可尝试构建，并以自己的编译与运行结果为准。

## 最直接的方式 用 Qt Creator 打开一个例子

1. 下载仓库完整 ZIP 并解压，或克隆仓库。
2. 在 Qt Creator 中选择“打开项目”，定位到 `examples/49_text_editor/49_text_editor.pro`。
3. 选择与已安装 Qt 匹配的 Kit。
4. 构建并运行。先尝试输入文本、另存为自己的测试文件，再重新打开。
5. 回到 `main.cpp`，按章节说明逐步阅读。

每个目录都是独立项目。打开第 49 步不需要先构建第 1 步；资源、头文件和 `.ui` 必须与本步源文件一起保留。

## 用 CMake 构建全部项目

在仓库根目录运行，`CMAKE_PREFIX_PATH` 应替换成自己的 Qt 安装路径：

```sh
cmake -S . -B build -DCMAKE_PREFIX_PATH="你的 Qt 安装目录"
cmake --build build --parallel 4
ctest --test-dir build --output-on-failure
```

例如 Windows MinGW 可以先把配套编译器和 Ninja 加入当前终端的 PATH，再选择 Ninja 生成器。不要把本机示例路径原样当成所有电脑通用的安装路径。

只构建一个项目：

```sh
cmake -S . -B build-one -DQT_LESSON=49_text_editor -DCMAKE_PREFIX_PATH="你的 Qt 安装目录"
cmake --build build-one
```

Windows 单配置生成器的程序通常位于 `build/49_text_editor.exe`；多配置生成器可能位于 `build/Release/49_text_editor.exe`。运行时需要 Qt 动态库可被找到，例如在当前终端设置 `QT_BIN` 并加入 PATH，或通过 Qt Creator 运行。Qt 安装目录、编译器及其运行库应保持配套。

`scripts/run.cmd 49_text_editor` 可以启动已在根目录 build 中构建的程序；它读取可选的 `QT_BIN`，不会改写系统环境变量。

## qmake 命令行方式

在单个例子的目录外建立构建目录，执行 `qmake 路径/项目.pro`，然后用与 Kit 匹配的 make 工具构建。MinGW 通常使用 `mingw32-make`。Qt Creator 会替你管理这些步骤，入门阶段可以先用 IDE。

## 找不到 Qt 或程序无法启动

- 找不到 `Qt6Config.cmake`：检查 Qt 开发包是否安装，以及 `CMAKE_PREFIX_PATH` 是否指向正确版本。
- 链接器报符号或库格式错误：检查 Qt、编译器和 32/64 位是否匹配。
- 提示平台插件无法加载：检查 PATH 中是否混入另一套 Qt，使用当前 Kit 提供的环境。
- 图片空白：检查本步 qrc、资源别名和资源是否加入构建。
- 修改 `.ui` 后没有变化：改源 `.ui`，重新构建；不要修改生成的 `ui_*.h`。

构建目录可以重新生成，自己的代码和文档应保留在源目录。不要把源文件存进容易清理的 build 目录。
