#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setWindowTitle(tr("LMC飞控调试软件 V1.0 - by Tzs"));

    comLabel = new QLabel(tr("串口"));
    comBox = new QComboBox;
    openMyComBtn = new QPushButton(tr("打开串口"));
    openMyComBtn->setCheckable(true);
    openMyComBtn->setChecked(false);

    QGridLayout *comLayout = new QGridLayout;
    comLayout->addWidget(comLabel,0,0,1,1);
    comLayout->addWidget(comBox,0,1,1,1);
    comLayout->addWidget(openMyComBtn,0,2,1,1);
    widget = new QWidget;
    widget->setLayout(comLayout);

    comWidget = new QDockWidget;
    comWidget->setWindowTitle(tr("端口参数设置"));
    comWidget->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
    comWidget->setWidget(widget);
    comWidget->setFloating(true);

    connect(openMyComBtn,SIGNAL(clicked()),this,SLOT(onMycomBtnClick()));


    addDockWidget(Qt::LeftDockWidgetArea,comWidget);

    textEdit = new QTextEdit;
    setCentralWidget(textEdit);
    setMinimumSize(600,400);


    //    QToolBar *toolbar = new QToolBar;
    //    toolbar->addWidget(comLabel);
    //    toolbar->addWidget(toolBtn1);
    //    addToolBar(toolbar);
}

//读串口函数
void MainWindow::readMycom()
{
    //读取串口缓冲区的所有数据给临时变量temp
    QByteArray temp = myCom->readAll();

    //将串口的数据显示在窗口的文本浏览器中
    textEdit->insertPlainText(temp);
}

void MainWindow::onMycomBtnClick()
{
    if(openMyComBtn->isChecked())
        openMyCom();
    else
        closeMyCom();
}

void MainWindow::openMyCom()
{
    //定义一个结构体，用来存放串口各个参数
    struct PortSettings myComSetting = {BAUD9600,DATA_8,PAR_NONE,STOP_1,FLOW_OFF};

    //定义串口对象，并传递参数，在构造函数里对其进行初始化
    myCom = new Win_QextSerialPort("com3",myComSetting,QextSerialBase::EventDriven);

    //一可读写方式打开串口
    myCom->open(QIODevice::ReadWrite);

    //信号和槽函数关联，当初a口缓冲区有数据时，进行读串口操作
    connect(myCom,SIGNAL(readyRead()),this,SLOT(readMycom()));

    openMyComBtn->setText(tr("关闭串口"));
}

void MainWindow::closeMyCom()
{
    myCom->close();
    openMyComBtn->setText(tr("打开串口"));
}
