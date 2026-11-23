#include <QtWidgets>
#include <QMouseEvent>
#include <QKeyEvent>
class EventArea : public QLabel
{
  public:
    explicit EventArea(QWidget *p = nullptr) : QLabel(p)
    {
        setMouseTracking(true);
        setFocusPolicy(Qt::StrongFocus);
        setAlignment(Qt::AlignCenter);
        setText("移动鼠标，点击后按键盘；拖动主窗口观察大小变化。");
    }

  protected:
    void resizeEvent(QResizeEvent *event) override
    {
        setText(QString("区域大小 %1 × %2").arg(event->size().width()).arg(event->size().height()));
        QLabel::resizeEvent(event);
    }
    void mouseMoveEvent(QMouseEvent *event) override
    {
        setText(QString("鼠标位置 %1，%2").arg(event->position().x()).arg(event->position().y()));
        QLabel::mouseMoveEvent(event);
    }
    void mousePressEvent(QMouseEvent *event) override
    {
        setFocus();
        setText(event->button() == Qt::LeftButton ? "左键按下" : "其他鼠标键按下");
        QLabel::mousePressEvent(event);
    }
    bool event(QEvent *event) override
    {
        if (event->type() == QEvent::KeyPress)
        {
            auto *key = static_cast<QKeyEvent *>(event);
            if (key->key() == Qt::Key_Tab)
            {
                setText("event 已处理 Tab，阻止默认焦点切换");
                return true;
            }
        }
        return QLabel::event(event);
    }
};
class CloseWindow : public QMainWindow
{
  protected:
    void closeEvent(QCloseEvent *event) override
    {
        auto choice = QMessageBox::question(this, "关闭确认", "确定关闭窗口吗？",
                                            QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        if (choice == QMessageBox::Yes)
            event->accept();
        else
            event->ignore();
    }
};

#include "verify.h"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    CloseWindow w;
    w.resize(800, 580);
    w.setWindowTitle("45 关闭事件与尺寸变化");
    w.setCentralWidget(new EventArea);
    w.statusBar()->showMessage("拖动边缘改变尺寸；关闭窗口会询问确认。");
    w.show();
    scheduleVerification(&w);
    return app.exec();
}
