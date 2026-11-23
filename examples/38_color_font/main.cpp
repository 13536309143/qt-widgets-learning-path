#include <QtWidgets>
#include <QMouseEvent>
#include <QKeyEvent>

#include "verify.h"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QMainWindow w;
    w.resize(800, 580);
    w.setWindowTitle("38 选择颜色与字体");
    auto *root = new QWidget;
    auto *layout = new QVBoxLayout(root);
    auto *color = new QPushButton("选择文字颜色");
    color->setObjectName("run");
    auto *font = new QPushButton("选择字体");
    font->setObjectName("fontButton");
    auto *edit = new QPlainTextEdit("选择后观察这行文字的颜色和字体。");
    layout->addWidget(color);
    layout->addWidget(font);
    layout->addWidget(edit);
    w.setCentralWidget(root);
    QObject::connect(color, &QPushButton::clicked, &w,
                     [&w, edit]()
                     {
                         auto *dialog = new QColorDialog(edit->palette().color(QPalette::Text), &w);
                         dialog->setOption(QColorDialog::DontUseNativeDialog);
                         dialog->setAttribute(Qt::WA_DeleteOnClose);
                         QObject::connect(dialog, &QColorDialog::colorSelected, edit,
                                          [edit](const QColor &value)
                                          {
                                              auto pal = edit->palette();
                                              pal.setColor(QPalette::Text, value);
                                              edit->setPalette(pal);
                                          });
                         dialog->open();
                     });
    QObject::connect(font, &QPushButton::clicked, &w,
                     [&w, edit]()
                     {
                         auto *dialog = new QFontDialog(edit->font(), &w);
                         dialog->setOption(QFontDialog::DontUseNativeDialog);
                         dialog->setAttribute(Qt::WA_DeleteOnClose);
                         QObject::connect(dialog, &QFontDialog::fontSelected, edit,
                                          &QWidget::setFont);
                         dialog->open();
                     });
    w.show();
    scheduleVerification(&w);
    return app.exec();
}
