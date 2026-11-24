# 第七章 对话框QDialog

学习对话框的模态、返回时机、对象生命周期和结果处理，再把文件选择、保存确认连接成完整操作。

本章对应的完整项目：

- [示例 36 dialog](../../examples/36_dialog/README.md)
- [示例 50 dialog_modes](../../examples/50_dialog_modes/README.md)
- [示例 37 file_dialog](../../examples/37_file_dialog/README.md)
- [示例 38 color_font](../../examples/38_color_font/README.md)
- [示例 39 messagebox](../../examples/39_messagebox/README.md)
- [示例 49 text_editor](../../examples/49_text_editor/README.md)

## 7.1 模态 非模态与对象生命期

对应示例：[完整源码](../../examples/36_dialog)。先运行项目观察行为，再按以下顺序阅读。

### 7.1.1 本章要解决的问题

模态决定其他窗口能否接受输入；阻塞决定调用处代码是否等待返回。exec() 显示模态对话框并等待返回，它会运行嵌套事件循环，因此对话框本身仍能响应。open() 默认按窗口模态显示并立即返回，通常连接 finished、accepted 或 rejected 处理结果。show() 通常用于非模态显示，但也会受事先设置的 modality 属性影响。

局部栈对象 dialog.show() 一闪而过的根本原因是函数返回后对象立即析构。异步显示应让对象持续存在，例如 new QDialog(&w)，并设置 WA_DeleteOnClose。仅有 parent 能在父窗口销毁时回收，不等于每次关掉对话框立即回收。

### 7.1.2 是否模态和是否等待是两条轴

一个对话框可能限制其他窗口的输入，却不阻塞调用处代码。open 就属于这种情况。exec 等待结果，但内部仍继续处理对话框事件，所以“阻塞代码”不等于整个 GUI 线程停止工作。嵌套事件循环会带来重入，调用之前保存的对象状态可能在对话框打开期间被其他事件改变。

### 7.1.3 按执行顺序拆解

mainwindow.ui 提供入口按钮；dialog.ui 提供自定义 Dialog 的标签与标准按钮。点击入口后 new Dialog(this)，其构造函数加载自己的 .ui；设置 WA_DeleteOnClose 后调用 open。示例 50进一步对照 show/open/exec，区分窗口模态和调用是否等待。

### 7.1.4 参数与行为边界

栈对象不是永远不能用：局部 QDialog dialog; dialog.exec(); 可以活到 exec 返回。问题出在局部对象配 show/open 后函数马上返回。异步版本应使用能存活的对象，合理设置 parent 和 WA_DeleteOnClose。持有可能被关闭删除的指针时，可用 QPointer 检测对象是否仍存在。

### 7.1.5 易错点与排查

open() 和 show() 后不要让对象立即离开作用域。exec() 的栈对象可活到返回，但新代码通常优先考虑 open() 加结果信号，减少嵌套事件循环。对话框有 parent 仍可作为独立顶层对话框显示，不是普通嵌入控件。

遇到问题时先区分三个层次：编译错误检查声明、头文件与构建配置；界面无反应检查连接、接收对象与生存期；运行结果不对检查输入、返回值和分支是否真正执行。可以先在连接入口记录一条日志，再在结果处理处记录一条，定位中间哪一步没有到达。

### 完整代码与界面操作

[本节完整源码与深入讲解](../topics/dialog-lifetime.md) · [独立项目与 Designer 步骤](../../examples/36_dialog/README.md)

![模态 非模态与对象生命期运行结果](../../examples/36_dialog/screenshots/03_result.png)

### 动手练习

打开示例 50，按 show、open、exec 分别测试确定和取消，观察日志顺序。再把 open 改成 show，比较父窗口是否可操作。不要对设置 DeleteOnClose 的对象关闭后继续调用成员。

先写下预期行为，再修改代码、重新构建，最后对照实际结果。程序没有报错不等于逻辑已经正确，尤其要测试取消、空输入、重复操作和失败分支。

### 自测与答案

**异步对话框必须写完 result=dialog->open() 吗？**

不可以。open 不用返回值传递选择结果，应提前 connect 到结果信号，并在 Lambda 中读取仍然存活的目标状态。

## 7.2 show open exec三种调用方式

[完整项目与 UI 步骤](../../examples/50_dialog_modes/README.md) · [窗口逻辑](../../examples/50_dialog_modes/widget.cpp) · [界面表单](../../examples/50_dialog_modes/widget.ui)

按钮和输出区在 widget.ui，自定义对话框在 dialog.ui，执行逻辑在 widget.cpp。分别点击 show、open、exec，然后观察调用前、调用后与结果通知的先后顺序。show 和 open 会立即返回，exec 等到关闭后返回结果；但 exec 内部仍在处理事件，因此按钮可以点击，自动检查中的 QTimer 也能工作。

异步对象通过 parent 与 WA_DeleteOnClose 管理，exec 示例使用栈对象，函数返回后自动析构。两者都不是“new 永远更好”或“栈永远不能用”，关键是对象必须活到最后一次使用结束。

练习：对每种入口分别点确定和取消，写出 Accepted/Rejected 出现的位置；在非模态 show 打开时尝试操作父窗口，在 open 打开时再尝试一次。

![对话框顺序验证](../../examples/50_dialog_modes/screenshots/result.png)

## 7.3 单文件 多文件与取消

对应示例：[完整源码](../../examples/37_file_dialog)。先运行项目观察行为，再按以下顺序阅读。

### 7.3.1 本章要解决的问题

getOpenFileName 返回 QString，getOpenFileNames 返回 QStringList；取消时分别得到空字符串或空列表。parent 用于归属，caption 是标题，dir 是起始目录，filter 是过滤规则。Images (*.png *.jpg);;Text (*.txt) 中 ;; 分隔多组过滤器。

文件选择只返回路径，不会把文件内容读到编辑框里。要读取还需 QFile 打开、检查错误、读取内容和处理编码。本项目明确只显示选中的路径。示例使用 QFileDialog 对象加 open() 与 filesSelected 信号，因此能以同样方式展示多个文件，取消时不执行选择后的逻辑。

### 7.3.2 选择路径与文件读写要分成两步

文件选择对话框只解决“用户要哪个路径”，QFile 才负责“这个路径能否打开、读到什么”。过滤器主要帮助用户选择，不是对文件内容的真实性验证。扩展名是 txt 也不保证内容就是 UTF-8 文本。读取后还要决定编码、大小限制和失败时是否保留当前文档。

### 7.3.3 按执行顺序拆解

先在 mainwindow.ui 确认 run 按钮和 resultEdit。点击后创建 QFileDialog，设置多选、过滤器、自动删除属性，再连接 filesSelected；选择文件后才将结果列表写入 resultEdit。取消不会进入选择结果逻辑。示例 49继续实现真正的文件读取和保存。

### 7.3.4 参数与行为边界

getOpenFileName 取消返回空字符串，getOpenFileNames 返回空列表；getSaveFileName 返回目标路径，不会自动完成写入。读取现有文件用 QFile，整份文档覆盖保存优先考虑 QSaveFile。文本内容从 QString 到字节数组需要明确编码，不要把本地编码和 UTF-8 混用。

### 7.3.5 易错点与排查

./ 依赖当前工作目录，未必是源码所在目录。示例用构建时提供的 QT_LESSON_SOURCE_DIR 定位源码中的 samples；发布应用时应改用用户目录或部署后的资源位置。为了截图可复现，使用 Qt 非原生对话框；实际程序可以根据平台选择原生外观。

遇到问题时先区分三个层次：编译错误检查声明、头文件与构建配置；界面无反应检查连接、接收对象与生存期；运行结果不对检查输入、返回值和分支是否真正执行。可以先在连接入口记录一条日志，再在结果处理处记录一条，定位中间哪一步没有到达。

### 完整代码与界面操作

[本节完整源码与深入讲解](../topics/file-dialog.md) · [独立项目与 Designer 步骤](../../examples/37_file_dialog/README.md)

![单文件 多文件与取消运行结果](../../examples/37_file_dialog/screenshots/03_result.png)

### 动手练习

用示例 49创建含中文和换行的文本，另存到自己的测试目录，关闭后重新打开。再选择不可写目录观察错误。对未保存内容发起打开或关闭，分别选择保存、不保存和取消，确认取消不会丢掉当前内容。

先写下预期行为，再修改代码、重新构建，最后对照实际结果。程序没有报错不等于逻辑已经正确，尤其要测试取消、空输入、重复操作和失败分支。

### 自测与答案

**保存后能立即把 modified 设成 false 吗？**

只有写入完整并且 commit 成功后才可以。用户取消路径选择或者磁盘写入失败时，保存未完成，当前文档仍应标记为修改过。

## 7.4 选择颜色与字体

对应示例：[完整源码](../../examples/38_color_font)。先运行项目观察行为，再按以下顺序阅读。

### 7.4.1 本章要解决的问题

QColorDialog::getColor 返回 QColor；取消返回无效颜色，先 isValid() 再应用。QFontDialog::getFont 使用 bool *ok 告诉调用者是否确认选择；只在 ok 为 true 时使用返回字体。把取消也当成有效结果，会意外改变用户当前设置。

颜色要应用到合适的调色板角色。QPlainTextEdit 的文字使用 QPalette::Text，普通 QLabel 的文字通常是 WindowText。先取原有 palette，仅改所需角色，再 setPalette，能够保留其他颜色。字体同样先从控件 font() 取得当前值。本项目用对象形式的颜色和字体对话框，以 open() 连接 colorSelected、fontSelected，确认后再更新编辑器。

### 7.4.2 区分用户正在选和已经确认

颜色对话框的 currentColorChanged 描述选择过程，colorSelected 通常用于最终确认；如果把预览直接永久应用，用户点取消时还需要恢复原值。字体也要区分预览、确认与取消。初学时先只连接最终选择信号，能更容易保证取消后原状态不变。

### 7.4.3 按执行顺序拆解

mainwindow.ui 配置颜色、字体按钮和 plainTextEdit。mainwindow.cpp 在按钮回调中读取编辑器当前的 palette 或 font，建立 Qt 标准对话框，确认选择后再通过 colorSelected/fontSelected 应用结果。取消时保持原状态。

### 7.4.4 参数与行为边界

QPalette::Text 用于输入类控件的文字；WindowText 常用于标签类文字；Base 用于文本输入区背景；Window 用于窗口背景。主题或样式表可能覆盖调色板效果，遇到颜色没变时也要检查这些设置。setFont 改整体字体，QTextEdit 选区格式则需要字符格式相关 API。

### 7.4.5 易错点与排查

QPlainTextEdit 整体字体与富文本局部字符格式是不同层级。设置整体 font 不等于给 QTextEdit 当前选区设置字符格式。对话框取消应该保持原显示。

遇到问题时先区分三个层次：编译错误检查声明、头文件与构建配置；界面无反应检查连接、接收对象与生存期；运行结果不对检查输入、返回值和分支是否真正执行。可以先在连接入口记录一条日志，再在结果处理处记录一条，定位中间哪一步没有到达。

### 完整代码与界面操作

[本节完整源码与深入讲解](../topics/color-font-dialog.md) · [独立项目与 Designer 步骤](../../examples/38_color_font/README.md)

![选择颜色与字体运行结果](../../examples/38_color_font/screenshots/03_result.png)

### 动手练习

增加一个重置按钮，恢复启动时缓存的 QPalette 和 QFont。再尝试同时修改背景与文字，避免低对比度。比较只改 Text 与只改 WindowText 对 QPlainTextEdit 的不同结果。

先写下预期行为，再修改代码、重新构建，最后对照实际结果。程序没有报错不等于逻辑已经正确，尤其要测试取消、空输入、重复操作和失败分支。

### 自测与答案

**为什么取消返回值仍看起来像某种字体？**

字体对象本身不能替代确认状态。使用静态 getFont 时看 ok；对象形式则把最终应用连接到 fontSelected，不把任意当前预览值当作确认。

## 7.5 消息框与三种选择结果

对应示例：[完整源码](../../examples/39_messagebox)。先运行项目观察行为，再按以下顺序阅读。

### 7.5.1 本章要解决的问题

information、warning、critical 分别用于信息、警告和错误提示；question 用于请求决策。它们返回 QMessageBox::StandardButton，应该与 Yes、No、Cancel 等标准按钮枚举比较，不能把任何非零返回值都当成确认。

文件已修改时，“保存”“不保存”“取消”代表三种不同的控制流程：保存成功后继续，不保存则继续但放弃改动，取消则中止当前关闭或切换。示例只演示分支，把选择结果写到编辑器，不会真正保存或删除文件。defaultButton 应明确指定，本例选择 Cancel，避免随手回车就放弃改动。

### 7.5.2 按钮结果必须落实到控制流程

“是否保存”不是只打印三种文本。保存意味着成功写入后继续；不保存意味着放弃当前修改后继续；取消意味着停止正在进行的关闭或切换。如果保存过程失败，后续关闭仍不能继续。这个分支可以抽成 confirmDiscard()，供打开新文档与关闭窗口共同使用。

### 7.5.3 按执行顺序拆解

mainwindow.ui 中的 run 按钮触发消息框，结果写入 resultEdit。查看 mainwindow.cpp 如何明确比较 Yes/No/Cancel。示例 49将这些选择接入 closeEvent 与 confirmDiscard，只有保存成功或用户明确放弃修改时才允许继续。

### 7.5.4 参数与行为边界

information、warning、critical 的图标表达不同严重程度，不会自动抛异常、记录日志或者退出程序。question 返回标准按钮枚举，默认按钮应有意识选择。键盘 Enter 触发默认按钮，Esc 的行为还与逃逸按钮和平台约定相关，不能把所有关闭方式当作 Yes。

### 7.5.5 易错点与排查

about 与 aboutQt 是说明信息，不是保存决策。示例问是否保存并不构成已经实现文件保存；真正保存要检查写入是否成功。

遇到问题时先区分三个层次：编译错误检查声明、头文件与构建配置；界面无反应检查连接、接收对象与生存期；运行结果不对检查输入、返回值和分支是否真正执行。可以先在连接入口记录一条日志，再在结果处理处记录一条，定位中间哪一步没有到达。

### 完整代码与界面操作

[本节完整源码与深入讲解](../topics/message-box.md) · [独立项目与 Designer 步骤](../../examples/39_messagebox/README.md)

![消息框与三种选择结果运行结果](../../examples/39_messagebox/screenshots/03_result.png)

### 动手练习

修改示例 39默认按钮，按 Enter 测试哪个分支被触发。再到示例 49编辑文本后关闭，选择 Yes 后取消保存路径，确认编辑器仍然打开且文本未丢失。

先写下预期行为，再修改代码、重新构建，最后对照实际结果。程序没有报错不等于逻辑已经正确，尤其要测试取消、空输入、重复操作和失败分支。

### 自测与答案

**为什么 if(result) 不够？**

Yes、No、Cancel 都是枚举值，并不把只有 Yes 定义为 true。必须显式与相应 StandardButton 比较，才能保持业务语义。

## 7.6 综合练习 文本读取与安全保存

[完整项目与 UI 步骤](../../examples/49_text_editor/README.md) · [窗口逻辑](../../examples/49_text_editor/mainwindow.cpp) · [界面表单](../../examples/49_text_editor/mainwindow.ui)

这个例子真正执行打开、编辑和保存，不仅显示一个文件路径。界面由 mainwindow.ui 创建，文件操作在 mainwindow.cpp。核心状态是当前路径与 QTextDocument 的 modified 标记。打开新文档前和关闭窗口时共用 confirmDiscard：没有修改则直接继续，有修改则分别处理保存、不保存、取消。

读取先检查 QFile::open，读取完成再检查错误，成功后才替换界面文本；保存先将 QString 转成 UTF-8，再写 QSaveFile，检查写入字节数并 commit。只有确认保存成功，才清除修改标记。路径对话框取消或磁盘写入失败，都应该保留原文档。

本例按 UTF-8 解释文本，并使用 readAll，适合小型文本学习。大文件、编码检测、异步读取与多文档管理可作为后续扩展。

练习顺序：输入两行中文 → 另存为自己的测试文件 → 关闭重新打开 → 修改后关闭并点取消 → 再关闭选择保存。自动检查在 QTemporaryDir 内验证 UTF-8 写入、读回和修改状态，不操作用户已有文件。

![文本编辑器运行结果](../../examples/49_text_editor/screenshots/result.png)

---

[总目录](../README.md) · [上一章](06-mainwindow.md) · [下一章](08-layouts.md)
