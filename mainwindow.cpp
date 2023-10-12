#include "mainwindow.h"
#include "ui_mainwindow.h"
#ifdef Q_OS_WIN
    #include <qt_windows.h>
    #include <Windowsx.h>
#endif
#include <QDebug>
#include <QDesktopWidget>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent,Qt::FramelessWindowHint | Qt::Window | Qt::WindowMinimizeButtonHint)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Window | Qt::WindowMinimizeButtonHint);
    initWindow();
    loadQssFile();
    connect(ui->btnMin,&QPushButton::clicked,this,&MainWindow::windowMin);
    connect(ui->btnMax,&QPushButton::clicked,this,&MainWindow::windowMax);
    connect(ui->btnClose,&QPushButton::clicked,this,&MainWindow::windowClose);

    isPressed=false;
    ui->titleWidget->setFixedHeight(46);    //固定标题栏高度
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadQssFile()
{

    QFile file(":/style.qss");
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream text(&file);
        QString styleSheet=text.readAll();
        this->setStyleSheet(styleSheet);
        file.close();
    }

}

void MainWindow::initWindow()
{

    isMaxWin=false;
    ui->btnClose->setObjectName("winBtn");
    ui->btnMin->setObjectName("winBtn");
    ui->btnMax->setObjectName("winBtn");
    ui->winTitle->setFont(QFont("宋体",11));
    ui->titleWidget->setObjectName("titleLabel");

    ui->btnMax->setIcon(QIcon(":/icons/maxsize.png"));
    ui->btnMin->setIcon(QIcon(":/icons/minium.png"));
    ui->btnClose->setIcon(QIcon(":/icons/close.png"));
}

void MainWindow:: mouseDoubleClickEvent(QMouseEvent*event)
{
    Q_UNUSED(event);

    if(ui->titleWidget->underMouse())
    {
        this->showNormal();
        ui->btnMax->setIcon(QIcon(":/icons/maxsize.png"));
        isMaxWin=false;
    }
}

void MainWindow::mousePressEvent(QMouseEvent*event)
{
    if(event->button()==Qt::LeftButton
            && ui->titleWidget->underMouse()) //如果鼠标左键处于标题栏并且按下
    {
        isPressed=true;
        curPos=event->pos();    //记录当前的点击坐标
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent*event)
{
    if(isPressed) //如果鼠标左键按下
    {
        this->move(event->pos()-curPos+this->pos());    //窗口移动

        if(isMaxWin)
        {
            windowMax();
           // this->showMaximized();
        }
    }
}

//鼠标释放
void MainWindow::mouseReleaseEvent(QMouseEvent*event)
{
    isPressed=false;
}

//关闭窗口
void MainWindow::windowClose()
{
    qApp->exit();
}

//窗口最小化
void MainWindow::windowMin()
{
    this->showMinimized();
}
//窗口最大化
void MainWindow::windowMax()
{
    isMaxWin=!isMaxWin;
    if(isMaxWin)    //根据是否最大化窗口,改变对应的图标
    {
        ui->btnMax->setIcon(QIcon(":/icons/normal.png"));
        this->showMaximized();
        this->showFullScreen(); //fixed

    }
    else
    {
        ui->btnMax->setIcon(QIcon(":/icons/maxsize.png"));
        this->showNormal();
    }


}

//消息处理
bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    int m_nBorder = 5;  //边界宽度
    Q_UNUSED(eventType)
    MSG *param = static_cast<MSG *>(message);

    switch (param->message)
    {
        case WM_NCHITTEST:
        {
            int nX = GET_X_LPARAM(param->lParam) - this->geometry().x();
            int nY = GET_Y_LPARAM(param->lParam) - this->geometry().y();


            *result = HTCAPTION;

            //判断鼠标位置是否位于窗口边界
            if ((nX > 0) && (nX < m_nBorder))
                *result = HTLEFT;

            if ((nX > this->width() - m_nBorder) && (nX < this->width()))
                *result = HTRIGHT;

            if ((nY > 0) && (nY < m_nBorder))
                *result = HTTOP;

            if ((nY > this->height() - m_nBorder) && (nY < this->height()))
                *result = HTBOTTOM;

            if ((nX > 0) && (nX < m_nBorder) && (nY > 0)
                    && (nY < m_nBorder))
                *result = HTTOPLEFT;

            if ((nX > this->width() - m_nBorder) && (nX < this->width())
                    && (nY > 0) && (nY < m_nBorder))
                *result = HTTOPRIGHT;

            if ((nX > 0) && (nX < m_nBorder)
                    && (nY > this->height() - m_nBorder) && (nY < this->height()))
                *result = HTBOTTOMLEFT;

            if ((nX > this->width() - m_nBorder) && (nX < this->width())
                    && (nY > this->height() - m_nBorder) && (nY < this->height()))
                *result = HTBOTTOMRIGHT;

            if (*result == HTCAPTION)
            {
                return false;
            }
            return true;
        }
    }
    return QMainWindow::nativeEvent(eventType, message, result);
}






