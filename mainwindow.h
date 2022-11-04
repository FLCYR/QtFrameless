#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QNetworkAccessManager>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void initWindow();  //初始化窗口控件
    void loadQssFile(); //加载qss
protected:
    void mousePressEvent(QMouseEvent*event);
    void mouseMoveEvent(QMouseEvent*event);
    void mouseReleaseEvent(QMouseEvent*event);
    void mouseDoubleClickEvent(QMouseEvent*event);
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);
private:
    void windowMin();
    void windowMax();
    void windowClose();

private:
    Ui::MainWindow *ui;
    bool isPressed;
    bool isMaxWin;
    QPoint curPos;
};
#endif // MAINWINDOW_H
