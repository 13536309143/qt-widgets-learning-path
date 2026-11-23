#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QPlainTextEdit;

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    bool loadPath(const QString &path);
    bool savePath(const QString &path);
    QPlainTextEdit *editor() const;

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::MainWindow *ui;
    QString path_;
    bool saveCurrent(bool saveAs = false);
    bool confirmDiscard();
    void updateTitle();
};
#endif // MAINWINDOW_H
