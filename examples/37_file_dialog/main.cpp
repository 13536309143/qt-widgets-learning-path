#include <QtWidgets>
#include <QMouseEvent>
#include <QKeyEvent>

#include "verify.h"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QMainWindow w;
    w.resize(800, 580);
    w.setWindowTitle("37 单文件 多文件与取消");
    auto *run = new QPushButton("选择一个或多个示例文件");
    run->setObjectName("run");
    auto *root = new QWidget;
    auto *layout = new QVBoxLayout(root);
    auto *result = new QTextEdit;
    result->setReadOnly(true);
    layout->addWidget(run);
    layout->addWidget(result);
    w.setCentralWidget(root);
    QObject::connect(run, &QPushButton::clicked, &w,
                     [&w, result]()
                     {
                         auto *dialog = new QFileDialog(
                             &w, "选择文件", QStringLiteral(QT_LESSON_SOURCE_DIR "/samples"));
                         dialog->setOption(QFileDialog::DontUseNativeDialog);
                         dialog->setFileMode(QFileDialog::ExistingFiles);
                         dialog->setNameFilter("Text (*.txt);;Images (*.png *.jpg)");
                         dialog->setAttribute(Qt::WA_DeleteOnClose);
                         QObject::connect(dialog, &QFileDialog::filesSelected, result,
                                          [result](const QStringList &files)
                                          { result->setPlainText(files.join('\n')); });
                         dialog->open();
                     });
    w.show();
    scheduleVerification(&w);
    return app.exec();
}
