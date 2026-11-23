#include <QtWidgets>
// --verify 只在自动检查时运行，正常打开窗口不会执行。
QString verificationFolder()
{
    const auto args = QCoreApplication::arguments();
    const int index = args.indexOf("--verify");
    return index >= 0 && index + 1 < args.size() ? args[index + 1] : QString();
}
void finishVerification(QWidget *window, const QString &folder, bool ok)
{
    QDir().mkpath(folder);
    ok = window->grab().save(folder + "/result.png") && ok;
    QFile status(folder + "/verification.txt");
    if (!status.open(QIODevice::WriteOnly))
        ok = false;
    else
        status.write(ok ? "PASS" : "FAIL");
    QCoreApplication::exit(ok ? 0 : 2);
}

class ReturnFilter : public QObject
{
  public:
    ReturnFilter(QLabel *result, QObject *parent) : QObject(parent), result_(result) {}

  protected:
    bool eventFilter(QObject *watched, QEvent *event) override
    {
        if (event->type() == QEvent::KeyPress)
        {
            auto *key = static_cast<QKeyEvent *>(event);
            if (key->key() == Qt::Key_Return || key->key() == Qt::Key_Enter)
            {
                auto *line = qobject_cast<QLineEdit *>(watched);
                if (line)
                {
                    result_->setText("过滤器收到：" + line->text());
                    return true; // 消费回车，不再交给输入框默认处理。
                }
            }
        }
        return QObject::eventFilter(watched, event);
    }

  private:
    QLabel *result_;
};
int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QWidget window;
    window.resize(720, 360);
    window.setWindowTitle("事件过滤器");
    auto *layout = new QVBoxLayout(&window);
    auto *line = new QLineEdit;
    line->setPlaceholderText("输入内容后按回车");
    auto *result = new QLabel("等待回车");
    auto *filter = new ReturnFilter(result, &window);
    line->installEventFilter(filter);
    layout->addWidget(line);
    layout->addWidget(result);
    window.show();
    const auto folder = verificationFolder();
    if (!folder.isEmpty())
        QTimer::singleShot(300, &window,
                           [&window, line, result, folder]()
                           {
                               line->setText("Qt 事件过滤器");
                               QKeyEvent key(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                               QCoreApplication::sendEvent(line, &key);
                               finishVerification(&window, folder,
                                                  result->text() ==
                                                      QStringLiteral("过滤器收到：Qt 事件过滤器"));
                           });
    return app.exec();
}
