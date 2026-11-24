# 按章节与知识点学习

项目编号就是章节中的知识点编号：例如 05_03 表示第五章的第三个项目。文件夹按名称排序即可依次学习。

## [第 1 章 Qt概述](chapters/01-overview.md)

概念导读，认识框架与工具。


## [第 2 章 创建Qt项目](chapters/02-create-project.md)

- [2.1 应用入口与事件循环](knowledge/02_01_first_window.md) · [完整项目](../examples/02_01_first_window/README.md)
- [2.2 Designer自动连接](knowledge/02_02_designer.md) · [完整项目](../examples/02_02_designer/README.md)

## [第 3 章 设置窗口属性](chapters/03-window-properties.md)

- [3.1 初始大小与固定大小](knowledge/03_01_window_size.md) · [完整项目](../examples/03_01_window_size/README.md)
- [3.2 资源路径与图像加载](knowledge/03_02_resources.md) · [完整项目](../examples/03_02_resources/README.md)

## [第 4 章 按钮的创建和属性设置](chapters/04-buttons.md)

- [4.1 按钮坐标与父控件](knowledge/04_01_buttons_coordinates.md) · [完整项目](../examples/04_01_buttons_coordinates/README.md)
- [4.2 对象树的级联销毁](knowledge/04_02_object_tree.md) · [完整项目](../examples/04_02_object_tree/README.md)
- [4.3 栈对象的构造与析构顺序](knowledge/04_03_stack_lifetime.md) · [完整项目](../examples/04_03_stack_lifetime/README.md)

## [第 5 章 信号和槽机制（Lambda）](chapters/05-signals-slots-lambda.md)

- [5.1 连接按钮与成员函数](knowledge/05_01_close_connection.md) · [完整项目](../examples/05_01_close_connection/README.md)
- [5.2 自定义信号槽的调用链](knowledge/05_02_custom_signal.md) · [完整项目](../examples/05_02_custom_signal/README.md)
- [5.3 信号转发](knowledge/05_03_signal_to_signal.md) · [完整项目](../examples/05_03_signal_to_signal/README.md)
- [5.4 选择带参数的重载](knowledge/05_04_overloaded_signal.md) · [完整项目](../examples/05_04_overloaded_signal/README.md)
- [5.5 保存连接句柄并断开](knowledge/05_05_disconnect.md) · [完整项目](../examples/05_05_disconnect/README.md)
- [5.6 一个信号连接多个处理函数](knowledge/05_06_multiple_slots.md) · [完整项目](../examples/05_06_multiple_slots/README.md)
- [5.7 Lambda的定义与调用](knowledge/05_07_function_to_lambda.md) · [完整项目](../examples/05_07_function_to_lambda/README.md)
- [5.8 定义阶段与执行阶段](knowledge/05_08_define_and_call.md) · [完整项目](../examples/05_08_define_and_call/README.md)
- [5.9 捕获值与函数参数的来源](knowledge/05_09_capture_and_parameter.md) · [完整项目](../examples/05_09_capture_and_parameter/README.md)
- [5.10 值捕获保存创建时的副本](knowledge/05_10_value_capture.md) · [完整项目](../examples/05_10_value_capture/README.md)
- [5.11 引用捕获访问原变量](knowledge/05_11_reference_capture.md) · [完整项目](../examples/05_11_reference_capture/README.md)
- [5.12 显式混合捕获](knowledge/05_12_mixed_capture.md) · [完整项目](../examples/05_12_mixed_capture/README.md)
- [5.13 指针捕获复制地址](knowledge/05_13_pointer_capture.md) · [完整项目](../examples/05_13_pointer_capture/README.md)
- [5.14 mutable修改捕获副本](knowledge/05_14_mutable_original.md) · [完整项目](../examples/05_14_mutable_original/README.md)
- [5.15 闭包副本的状态保留](knowledge/05_15_mutable_state.md) · [完整项目](../examples/05_15_mutable_state/README.md)
- [5.16 通过this访问窗口成员](knowledge/05_16_this_member.md) · [完整项目](../examples/05_16_this_member/README.md)
- [5.17 空捕获回调](knowledge/05_17_empty_capture_button.md) · [完整项目](../examples/05_17_empty_capture_button/README.md)
- [5.18 信号实参与捕获数据同时使用](knowledge/05_18_signal_parameter.md) · [完整项目](../examples/05_18_signal_parameter/README.md)
- [5.19 Lambda适配业务参数](knowledge/05_19_lambda_adapter.md) · [完整项目](../examples/05_19_lambda_adapter/README.md)
- [5.20 循环回调保存各自编号](knowledge/05_20_loop_capture.md) · [完整项目](../examples/05_20_loop_capture/README.md)
- [5.21 长期回调使用窗口状态](knowledge/05_21_safe_delayed_state.md) · [完整项目](../examples/05_21_safe_delayed_state/README.md)
- [5.22 回调内部保存独立计数](knowledge/05_22_lambda_keeps_copy.md) · [完整项目](../examples/05_22_lambda_keeps_copy/README.md)
- [5.23 成员状态与界面统一刷新](knowledge/05_23_complete_counter.md) · [完整项目](../examples/05_23_complete_counter/README.md)
- [5.24 连接普通成员函数](knowledge/05_24_member_slot.md) · [完整项目](../examples/05_24_member_slot/README.md)
- [5.25 用输出验证捕获规则](knowledge/05_25_self_test.md) · [完整项目](../examples/05_25_self_test/README.md)

## [第 6 章 QMainWindow](chapters/06-mainwindow.md)

- [6.1 QMainWindow中心工作区](knowledge/06_01_mainwindow.md) · [完整项目](../examples/06_01_mainwindow/README.md)
- [6.2 QAction共享命令入口](knowledge/06_02_menu_toolbar.md) · [完整项目](../examples/06_02_menu_toolbar/README.md)
- [6.3 临时状态与停靠区域](knowledge/06_03_status_dock.md) · [完整项目](../examples/06_03_status_dock/README.md)

## [第 7 章 对话框QDialog](chapters/07-dialogs.md)

- [7.1 异步对话框的生命周期](knowledge/07_01_dialog.md) · [完整项目](../examples/07_01_dialog/README.md)
- [7.2 show open exec的返回时机](knowledge/07_02_dialog_modes.md) · [完整项目](../examples/07_02_dialog_modes/README.md)
- [7.3 文件选择与取消处理](knowledge/07_03_file_dialog.md) · [完整项目](../examples/07_03_file_dialog/README.md)
- [7.4 确认颜色或字体后再应用](knowledge/07_04_color_font.md) · [完整项目](../examples/07_04_color_font/README.md)
- [7.5 消息框返回值与控制分支](knowledge/07_05_messagebox.md) · [完整项目](../examples/07_05_messagebox/README.md)
- [7.6 打开编辑保存的完整流程](knowledge/07_06_text_editor.md) · [完整项目](../examples/07_06_text_editor/README.md)

## [第 8 章 布局管理器](chapters/08-layouts.md)

- [8.1 四种布局的嵌套](knowledge/08_01_layout.md) · [完整项目](../examples/08_01_layout/README.md)

## [第 9 章 常用控件](chapters/09-widgets.md)

- [9.1 标签的文本图像与动画接口](knowledge/09_01_label_movie.md) · [完整项目](../examples/09_01_label_movie/README.md)
- [9.2 输入内容与显示模式](knowledge/09_02_lineedit.md) · [完整项目](../examples/09_02_lineedit/README.md)
- [9.3 可复用组合控件与提升](knowledge/09_03_custom_widget.md) · [完整项目](../examples/09_03_custom_widget/README.md)
- [9.4 堆叠页面与状态保留](knowledge/09_04_stacked_pages.md) · [完整项目](../examples/09_04_stacked_pages/README.md)

## [项目 5.5 章 Qt消息机制和事件](chapters/10-events.md)

- [10.1 关闭请求与尺寸通知](knowledge/10_01_close_resize.md) · [完整项目](../examples/10_01_close_resize/README.md)
- [10.2 鼠标事件的坐标与按键](knowledge/10_02_mouse_events.md) · [完整项目](../examples/10_02_mouse_events/README.md)
- [10.3 event中拦截Tab](knowledge/10_03_event_tab.md) · [完整项目](../examples/10_03_event_tab/README.md)
- [10.4 为现有控件安装事件过滤器](knowledge/10_04_event_filter.md) · [完整项目](../examples/10_04_event_filter/README.md)

## [项目 5.6 章 绘图和绘图设备](chapters/11-painting.md)

- [11.1 paintEvent与绘图设备](knowledge/11_01_painter.md) · [完整项目](../examples/11_01_painter/README.md)
- [11.2 状态更新与重绘分离](knowledge/11_02_interactive_paint.md) · [完整项目](../examples/11_02_interactive_paint/README.md)

[新旧编号对照](project-renumbering.md) · [项目模板与 UI 操作](project-format.md) · [安装运行](getting-started.md)
