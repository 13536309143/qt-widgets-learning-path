[先熟悉统一模板与 Designer 操作](project-format.md)

# 学习目录

[从下载到运行](getting-started.md) · [基础与 Lambda](basics.md) · [四个完整实战](practices.md)

## 第 20—37 章详细教程

| 章节 | 内容 | 独立项目 |
|---|---|---|
| 20 | [主窗口与中心部件](chapters/20-mainwindow.md) | [31_mainwindow](../examples/31_mainwindow) |
| 21 | [菜单 动作与工具栏](chapters/21-menu_toolbar.md) | [32_menu_toolbar](../examples/32_menu_toolbar) |
| 22 | [状态栏和停靠面板](chapters/22-status_dock.md) | [33_status_dock](../examples/33_status_dock) |
| 23 | [资源路径 图片与背景](chapters/23-resources.md) | [34_resources](../examples/34_resources) |
| 24 | [Designer 与 setupUi](chapters/24-designer.md) | [35_designer](../examples/35_designer) |
| 25 | [模态 非模态与对象生命期](chapters/25-dialog.md) | [36_dialog](../examples/36_dialog) |
| 26 | [单文件 多文件与取消](chapters/26-file_dialog.md) | [37_file_dialog](../examples/37_file_dialog) |
| 27 | [选择颜色与字体](chapters/27-color_font.md) | [38_color_font](../examples/38_color_font) |
| 28 | [消息框与三种选择结果](chapters/28-messagebox.md) | [39_messagebox](../examples/39_messagebox) |
| 29 | [水平 垂直 网格与表单布局](chapters/29-layout.md) | [40_layout](../examples/40_layout) |
| 30 | [QLabel 文本 图片与动画](chapters/30-label_movie.md) | [41_label_movie](../examples/41_label_movie) |
| 31 | [QLineEdit 输入与显示模式](chapters/31-lineedit.md) | [42_lineedit](../examples/42_lineedit) |
| 32 | [组合控件与 Designer 提升](chapters/32-custom_widget.md) | [43_custom_widget](../examples/43_custom_widget) |
| 33 | [堆叠页面与窗口切换](chapters/33-stacked_pages.md) | [44_stacked_pages](../examples/44_stacked_pages) |
| 34 | [关闭事件与尺寸变化](chapters/34-close_resize.md) | [45_close_resize](../examples/45_close_resize) |
| 35 | [鼠标按键 坐标和跟踪](chapters/35-mouse_events.md) | [46_mouse_events](../examples/46_mouse_events) |
| 36 | [重写 event 拦截 Tab](chapters/36-event_tab.md) | [47_event_tab](../examples/47_event_tab) |
| 37 | [QPainter 与绘图设备](chapters/37-painter.md) | [48_painter](../examples/48_painter) |

## 全部 52 步

| 步骤 | 内容 |
|---|---|
| [01_first_window](../examples/01_first_window) | 第一个空白窗口 |
| [02_window_size](../examples/02_window_size) | 标题与窗口大小 |
| [03_buttons_coordinates](../examples/03_buttons_coordinates) | 按钮与父控件坐标 |
| [04_object_tree](../examples/04_object_tree) | 对象树与析构日志 |
| [05_stack_lifetime](../examples/05_stack_lifetime) | 栈对象安全的构造顺序 |
| [06_close_connection](../examples/06_close_connection) | 点击按钮关闭窗口 |
| [07_custom_signal](../examples/07_custom_signal) | 自定义信号与槽 |
| [08_signal_to_signal](../examples/08_signal_to_signal) | 信号连接另一个信号 |
| [09_overloaded_signal](../examples/09_overloaded_signal) | 带参数的重载信号与槽 |
| [10_disconnect](../examples/10_disconnect) | 保存句柄并断开连接 |
| [11_multiple_slots](../examples/11_multiple_slots) | 一个信号连接多个处理函数 |
| [12_function_to_lambda](../examples/12_function_to_lambda) | 普通函数改写为 Lambda |
| [13_define_and_call](../examples/13_define_and_call) | 定义与调用的区别 |
| [14_capture_and_parameter](../examples/14_capture_and_parameter) | 捕获列表与参数列表 |
| [15_value_capture](../examples/15_value_capture) | 值捕获保存旧值 |
| [16_reference_capture](../examples/16_reference_capture) | 引用捕获修改原变量 |
| [17_mixed_capture](../examples/17_mixed_capture) | 显式混合捕获 |
| [18_pointer_capture](../examples/18_pointer_capture) | 复制指针仍可改变目标 |
| [19_mutable_original](../examples/19_mutable_original) | 逐步复现原笔记的 mutable |
| [20_mutable_state](../examples/20_mutable_state) | 同一个 Lambda 会保留副本 |
| [21_this_member](../examples/21_this_member) | this 与窗口成员 |
| [22_empty_capture_button](../examples/22_empty_capture_button) | 空捕获的按钮回调 |
| [23_signal_parameter](../examples/23_signal_parameter) | Lambda 接收信号参数 |
| [24_lambda_adapter](../examples/24_lambda_adapter) | Lambda 转换业务参数 |
| [25_loop_capture](../examples/25_loop_capture) | 循环中保存按钮编号 |
| [26_safe_delayed_state](../examples/26_safe_delayed_state) | 避免引用已消失的局部变量 |
| [27_lambda_keeps_copy](../examples/27_lambda_keeps_copy) | 值捕获也能保存回调自身状态 |
| [28_complete_counter](../examples/28_complete_counter) | 完整练习 计数与清零 |
| [29_member_slot](../examples/29_member_slot) | 改为连接普通成员函数 |
| [30_self_test](../examples/30_self_test) | Lambda 自测结果对照 |
| [31_mainwindow](../examples/31_mainwindow) | 主窗口与中心部件 |
| [32_menu_toolbar](../examples/32_menu_toolbar) | 菜单 动作与工具栏 |
| [33_status_dock](../examples/33_status_dock) | 状态栏和停靠面板 |
| [34_resources](../examples/34_resources) | 资源路径 图片与背景 |
| [35_designer](../examples/35_designer) | Designer 与 setupUi |
| [36_dialog](../examples/36_dialog) | 模态 非模态与对象生命期 |
| [37_file_dialog](../examples/37_file_dialog) | 单文件 多文件与取消 |
| [38_color_font](../examples/38_color_font) | 选择颜色与字体 |
| [39_messagebox](../examples/39_messagebox) | 消息框与三种选择结果 |
| [40_layout](../examples/40_layout) | 水平 垂直 网格与表单布局 |
| [41_label_movie](../examples/41_label_movie) | QLabel 文本 图片与动画 |
| [42_lineedit](../examples/42_lineedit) | QLineEdit 输入与显示模式 |
| [43_custom_widget](../examples/43_custom_widget) | 组合控件与 Designer 提升 |
| [44_stacked_pages](../examples/44_stacked_pages) | 堆叠页面与窗口切换 |
| [45_close_resize](../examples/45_close_resize) | 关闭事件与尺寸变化 |
| [46_mouse_events](../examples/46_mouse_events) | 鼠标按键 坐标和跟踪 |
| [47_event_tab](../examples/47_event_tab) | 重写 event 拦截 Tab |
| [48_painter](../examples/48_painter) | QPainter 与绘图设备 |
| [49_text_editor](../examples/49_text_editor) | UTF-8 文件读取与安全保存 |
| [50_dialog_modes](../examples/50_dialog_modes) | show open exec 三种调用对比 |
| [51_event_filter](../examples/51_event_filter) | 事件过滤器处理输入 |
| [52_interactive_paint](../examples/52_interactive_paint) | 状态驱动的交互绘图 |
