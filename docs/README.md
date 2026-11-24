# Qt学习笔记 十一章目录

正文按十一章学习；示例 01—52 保留独立项目编号。章节与示例是两套索引，后续内容请继续归入对应章节的小节。

| 正文章节 | 主要内容 | 对应示例 |
|---|---|---|
| [第一章 Qt概述](chapters/01-overview.md) | Qt与开发工具的区别、基础类、学习方法 | 概念导读 |
| [第二章 创建Qt项目](chapters/02-create-project.md) | 新建项目、Kit、默认模板、setupUi、构建与调试 | 01、35 |
| [第三章 设置窗口属性](chapters/03-window-properties.md) | 标题、尺寸、固定窗口、图标与资源 | 02、34 |
| [第四章 按钮的创建和属性设置](chapters/04-buttons.md) | Designer按钮、代码创建、属性、坐标、对象树 | 03、04、05 |
| [第五章 信号和槽机制（Lambda）](chapters/05-signals-slots-lambda.md) | connect、自定义信号、重载、捕获、mutable、this、生命周期 | 06—30 |
| [第六章 QMainWindow](chapters/06-mainwindow.md) | 中心部件、菜单、QAction、工具栏、状态栏与停靠 | 31、32、33 |
| [第七章 对话框QDialog](chapters/07-dialogs.md) | 模态与调用时机、标准对话框、保存确认与文本编辑器 | 36、50、37、38、39、49 |
| [第八章 布局管理器](chapters/08-layouts.md) | 水平、垂直、网格、表单、间距与伸缩 | 40 |
| [第九章 常用控件](chapters/09-widgets.md) | QLabel、QMovie、QLineEdit、组合控件与页面 | 41、42、43、44 |
| [第十章 Qt消息机制和事件](chapters/10-events.md) | 事件循环、关闭、尺寸、鼠标、event与过滤器 | 45、46、47、51 |
| [第十一章 绘图和绘图设备](chapters/11-painting.md) | QPainter、绘图设备、paintEvent、update与状态 | 48、52 |

## 怎样开始

先读第一章建立整体认识，再按第二章运行第一个项目。每一步都有完整源码、.pro、.ui、运行截图和项目 README 中的 Designer 操作清单。遇到源文件分工问题，查[统一模板](project-format.md)；遇到构建问题，查[安装与运行](getting-started.md)。

## 全部示例归属

| 示例 | 内容 | 所属正文章节 |
|---|---|---|
| [示例 01](../examples/01_first_window/README.md) | 第一个空白窗口 | [第二章](chapters/02-create-project.md) |
| [示例 02](../examples/02_window_size/README.md) | 标题与窗口大小 | [第三章](chapters/03-window-properties.md) |
| [示例 03](../examples/03_buttons_coordinates/README.md) | 按钮与父控件坐标 | [第四章](chapters/04-buttons.md) |
| [示例 04](../examples/04_object_tree/README.md) | 对象树与析构日志 | [第四章](chapters/04-buttons.md) |
| [示例 05](../examples/05_stack_lifetime/README.md) | 栈对象安全的构造顺序 | [第四章](chapters/04-buttons.md) |
| [示例 06](../examples/06_close_connection/README.md) | 点击按钮关闭窗口 | [第五章](chapters/05-signals-slots-lambda.md) |
| [示例 07](../examples/07_custom_signal/README.md) | 自定义信号与槽 | [第五章](chapters/05-signals-slots-lambda.md) |
| [示例 08](../examples/08_signal_to_signal/README.md) | 信号连接另一个信号 | [第五章](chapters/05-signals-slots-lambda.md) |
| [示例 09](../examples/09_overloaded_signal/README.md) | 带参数的重载信号与槽 | [第五章](chapters/05-signals-slots-lambda.md) |
| [示例 10](../examples/10_disconnect/README.md) | 保存句柄并断开连接 | [第五章](chapters/05-signals-slots-lambda.md) |
| [示例 11](../examples/11_multiple_slots/README.md) | 一个信号连接多个处理函数 | [第五章](chapters/05-signals-slots-lambda.md) |
| [示例 12](../examples/12_function_to_lambda/README.md) | 普通函数改写为 Lambda | [第五章](chapters/05-signals-slots-lambda.md) |
| [示例 13](../examples/13_define_and_call/README.md) | 定义与调用的区别 | [第五章](chapters/05-signals-slots-lambda.md) |
| [示例 14](../examples/14_capture_and_parameter/README.md) | 捕获列表与参数列表 | [第五章](chapters/05-signals-slots-lambda.md) |
| [示例 15](../examples/15_value_capture/README.md) | 值捕获保存旧值 | [第五章](chapters/05-signals-slots-lambda.md) |
| [示例 16](../examples/16_reference_capture/README.md) | 引用捕获修改原变量 | [第五章](chapters/05-signals-slots-lambda.md) |
| [示例 17](../examples/17_mixed_capture/README.md) | 显式混合捕获 | [第五章](chapters/05-signals-slots-lambda.md) |
| [示例 18](../examples/18_pointer_capture/README.md) | 复制指针仍可改变目标 | [第五章](chapters/05-signals-slots-lambda.md) |
| [示例 19](../examples/19_mutable_original/README.md) | 逐步复现原笔记的 mutable | [第五章](chapters/05-signals-slots-lambda.md) |
| [示例 20](../examples/20_mutable_state/README.md) | 同一个 Lambda 会保留副本 | [第五章](chapters/05-signals-slots-lambda.md) |
| [示例 21](../examples/21_this_member/README.md) | this 与窗口成员 | [第五章](chapters/05-signals-slots-lambda.md) |
| [示例 22](../examples/22_empty_capture_button/README.md) | 空捕获的按钮回调 | [第五章](chapters/05-signals-slots-lambda.md) |
| [示例 23](../examples/23_signal_parameter/README.md) | Lambda 接收信号参数 | [第五章](chapters/05-signals-slots-lambda.md) |
| [示例 24](../examples/24_lambda_adapter/README.md) | Lambda 转换业务参数 | [第五章](chapters/05-signals-slots-lambda.md) |
| [示例 25](../examples/25_loop_capture/README.md) | 循环中保存按钮编号 | [第五章](chapters/05-signals-slots-lambda.md) |
| [示例 26](../examples/26_safe_delayed_state/README.md) | 避免引用已消失的局部变量 | [第五章](chapters/05-signals-slots-lambda.md) |
| [示例 27](../examples/27_lambda_keeps_copy/README.md) | 值捕获也能保存回调自身状态 | [第五章](chapters/05-signals-slots-lambda.md) |
| [示例 28](../examples/28_complete_counter/README.md) | 完整练习 计数与清零 | [第五章](chapters/05-signals-slots-lambda.md) |
| [示例 29](../examples/29_member_slot/README.md) | 改为连接普通成员函数 | [第五章](chapters/05-signals-slots-lambda.md) |
| [示例 30](../examples/30_self_test/README.md) | Lambda 自测结果对照 | [第五章](chapters/05-signals-slots-lambda.md) |
| [示例 31](../examples/31_mainwindow/README.md) | 主窗口与中心部件 | [第六章](chapters/06-mainwindow.md) |
| [示例 32](../examples/32_menu_toolbar/README.md) | 菜单 动作与工具栏 | [第六章](chapters/06-mainwindow.md) |
| [示例 33](../examples/33_status_dock/README.md) | 状态栏和停靠面板 | [第六章](chapters/06-mainwindow.md) |
| [示例 34](../examples/34_resources/README.md) | 资源路径 图片与背景 | [第三章](chapters/03-window-properties.md) |
| [示例 35](../examples/35_designer/README.md) | Designer 与 setupUi | [第二章](chapters/02-create-project.md) |
| [示例 36](../examples/36_dialog/README.md) | 模态 非模态与对象生命期 | [第七章](chapters/07-dialogs.md) |
| [示例 37](../examples/37_file_dialog/README.md) | 单文件 多文件与取消 | [第七章](chapters/07-dialogs.md) |
| [示例 38](../examples/38_color_font/README.md) | 选择颜色与字体 | [第七章](chapters/07-dialogs.md) |
| [示例 39](../examples/39_messagebox/README.md) | 消息框与三种选择结果 | [第七章](chapters/07-dialogs.md) |
| [示例 40](../examples/40_layout/README.md) | 水平 垂直 网格与表单布局 | [第八章](chapters/08-layouts.md) |
| [示例 41](../examples/41_label_movie/README.md) | QLabel 文本 图片与动画 | [第九章](chapters/09-widgets.md) |
| [示例 42](../examples/42_lineedit/README.md) | QLineEdit 输入与显示模式 | [第九章](chapters/09-widgets.md) |
| [示例 43](../examples/43_custom_widget/README.md) | 组合控件与 Designer 提升 | [第九章](chapters/09-widgets.md) |
| [示例 44](../examples/44_stacked_pages/README.md) | 堆叠页面与窗口切换 | [第九章](chapters/09-widgets.md) |
| [示例 45](../examples/45_close_resize/README.md) | 关闭事件与尺寸变化 | [第十章](chapters/10-events.md) |
| [示例 46](../examples/46_mouse_events/README.md) | 鼠标按键 坐标和跟踪 | [第十章](chapters/10-events.md) |
| [示例 47](../examples/47_event_tab/README.md) | 重写 event 拦截 Tab | [第十章](chapters/10-events.md) |
| [示例 48](../examples/48_painter/README.md) | QPainter 与绘图设备 | [第十一章](chapters/11-painting.md) |
| [示例 49](../examples/49_text_editor/README.md) | UTF-8 文件读取与安全保存 | [第七章](chapters/07-dialogs.md) |
| [示例 50](../examples/50_dialog_modes/README.md) | show open exec 三种调用对比 | [第七章](chapters/07-dialogs.md) |
| [示例 51](../examples/51_event_filter/README.md) | 事件过滤器处理输入 | [第十章](chapters/10-events.md) |
| [示例 52](../examples/52_interactive_paint/README.md) | 状态驱动的交互绘图 | [第十一章](chapters/11-painting.md) |
