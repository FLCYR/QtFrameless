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
    ui->titleWidget->setFixedHeight(46);    //�̶��������߶�
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
    ui->winTitle->setFont(QFont("����",11));
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
            && ui->titleWidget->underMouse()) //������������ڱ��������Ұ���
    {
        isPressed=true;
        curPos=event->pos();    //��¼��ǰ�ĵ������
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent*event)
{
    if(isPressed) //�������������
    {
        this->move(event->pos()-curPos+this->pos());    //�����ƶ�

        if(isMaxWin)
        {
            windowMax();
           // this->showMaximized();
        }
    }
}

//����ͷ�
void MainWindow::mouseReleaseEvent(QMouseEvent*event)
{
    isPressed=false;
}

//�رմ���
void MainWindow::windowClose()
{
    qApp->exit();
}

//������С��
void MainWindow::windowMin()
{
    this->showMinimized();
}
//�������
void MainWindow::windowMax()
{
    isMaxWin=!isMaxWin;
    if(isMaxWin)    //�����Ƿ���󻯴���,�ı��Ӧ��ͼ��
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

//��Ϣ����
bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    int m_nBorder = 5;  //�߽���
    Q_UNUSED(eventType)
    MSG *param = static_cast<MSG *>(message);

    switch (param->message)
    {
        case WM_NCHITTEST:
        {
            int nX = GET_X_LPARAM(param->lParam) - this->geometry().x();
            int nY = GET_Y_LPARAM(param->lParam) - this->geometry().y();


            *result = HTCAPTION;

            //�ж����λ���Ƿ�λ�ڴ��ڱ߽�
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






