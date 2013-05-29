#include "mainwindow.h"
#include<QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setWindowTitle(tr("LMC飞控调试软件 V0.1 - by Tzs"));
    DataPlot = new QCustomPlot(this);
    DataPlot->setFocus();
    setupRealtimeDataGraph();

    myAttitudeWidget = new AttitudeWidget;

    QVBoxLayout *mainlayout = new QVBoxLayout;
    mainlayout->addWidget(DataPlot);
    mainlayout->addWidget(myAttitudeWidget);

    QWidget *mainWidget = new QWidget;
    mainWidget->setLayout(mainlayout);
    setCentralWidget(mainWidget);




    lockLabel = new QLabel(tr("已锁定"));
    statusGroup = new QGroupBox;
    statusGroup->setTitle(tr("四轴状态"));
    QVBoxLayout *statusLayout = new QVBoxLayout;
    statusLayout->addWidget(lockLabel);
    statusGroup->setLayout(statusLayout);

    comLabel = new QLabel(tr("串口"));
    comLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    comBox = new QComboBox;
    openMyComBtn = new QPushButton(tr("打开串口"));
    openMyComBtn->setCheckable(true);
    openMyComBtn->setChecked(false);

    baudLabel = new QLabel(tr("波特率"));
    baudLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    baudBox = new QComboBox;
    baudBox->addItem("9600");
    baudBox->addItem("19200");
    baudBox->addItem("38400");

    parityLabel = new QLabel(tr("校验位"));
    parityLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    parityBox = new QComboBox;
    parityBox->addItem(tr("无"));
    parityBox->addItem(tr("奇"));
    parityBox->addItem(tr("偶"));

    dataBitsLabel = new QLabel(tr("数据位"));
    dataBitsLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    dataBitsBox = new QComboBox;
    dataBitsBox->addItem("8");
    dataBitsBox->addItem("7");

    stopBitsLabel = new QLabel(tr("停止位"));
    stopBitsLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    stopBitsBox = new QComboBox;
    stopBitsBox->addItem("1");
    stopBitsBox->addItem("2");

    QGridLayout *comLayout = new QGridLayout;
    comLayout->addWidget(openMyComBtn,0,1,1,1);
    comLayout->addWidget(comLabel,1,0,1,1);
    comLayout->addWidget(comBox,1,1,1,1);
    comLayout->addWidget(baudLabel,2,0,1,1);
    comLayout->addWidget(baudBox,2,1,1,1);
    comLayout->addWidget(parityLabel,3,0,1,1);
    comLayout->addWidget(parityBox,3,1,1,1);
    comLayout->addWidget(dataBitsLabel,4,0,1,1);
    comLayout->addWidget(dataBitsBox,4,1,1,1);
    comLayout->addWidget(stopBitsLabel,5,0,1,1);
    comLayout->addWidget(stopBitsBox,5,1,1,1);
    comGroup = new QGroupBox;
    comGroup->setTitle(tr("端口参数设置"));
    comGroup->setLayout(comLayout);

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addWidget(statusGroup);
    leftLayout->addWidget(comGroup);

    comWidget = new QDockWidget;
    QWidget *leftDockWidget = new QWidget;
    leftDockWidget->setLayout(leftLayout);

    //    comWidget->setWindowTitle(tr("端口参数设置"));
    comWidget->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
    comWidget->setWidget(leftDockWidget);
    comWidget->setMaximumWidth(200);

    connect(openMyComBtn,SIGNAL(clicked()),this,SLOT(onMycomBtnClick()));
    //关联DataPlot绘图widget和mainwindow的鼠标事件
    connect(DataPlot,SIGNAL(mousePress(QMouseEvent*)),this,SLOT(mousePressEvent(QMouseEvent*)));
    connect(DataPlot,SIGNAL(mouseRelease(QMouseEvent*)),this,SLOT(mouseReleaseEvent(QMouseEvent*)));

    addDockWidget(Qt::LeftDockWidgetArea,comWidget);

    setMinimumSize(800,500);

    OKtoReadH = false;
}

//点击鼠标左键进行拖拽时指针形状变成手型
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        QCursor  cursor;
        cursor.setShape(Qt::ClosedHandCursor);
        QApplication::setOverrideCursor(cursor);
    }
}

//恢复鼠标形状
void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    QApplication::restoreOverrideCursor();
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
    //定义串口对象，并传递参数，在构造函数里对其进行初始化
    myCom = new QextSerialPort("com3", QextSerialPort::EventDriven);

    //一可读写方式打开串口
    myCom->open(QIODevice::ReadWrite | QIODevice::Unbuffered);

    //在定时器再一次开启之前清理曲线数据
    DataPlot->graph(0)->clearData();
    DataPlot->graph(1)->clearData();
    DataPlot->graph(2)->clearData();
    DataPlot->graph(3)->clearData();
    DataPlot->graph(4)->clearData();
    DataPlot->graph(5)->clearData();
    dataTimer.start(0);

    //we set the port properties
    myCom->setBaudRate(BAUD38400);//modify the port settings on your own
    myCom->setFlowControl(FLOW_OFF);
    myCom->setParity(PAR_NONE);
    myCom->setDataBits(DATA_8);
    myCom->setStopBits(STOP_1);

    //信号和槽函数关联，当初a口缓冲区有数据时，进行读串口操作
    connect(myCom,SIGNAL(readyRead()),this,SLOT(readMycom()));

    openMyComBtn->setText(tr("关闭串口"));

}

void MainWindow::closeMyCom()
{
    myCom->close();
    dataTimer.stop();
    openMyComBtn->setText(tr("打开串口"));
}

void MainWindow::setupRealtimeDataGraph()
{
    QPen myPen;
    myPen.setWidth(2);

    myPen.setColor(Qt::blue);
    DataPlot->addGraph(); // 蓝线-角度
    DataPlot->graph(0)->setPen(myPen);
    DataPlot->graph(0)->setName(tr("俯仰角"));

    myPen.setColor(Qt::red);
    DataPlot->addGraph(); // 红线-角度
    DataPlot->graph(1)->setPen(myPen);
    DataPlot->graph(1)->setName(tr("滚转角"));

    myPen.setColor(Qt::black);
    DataPlot->addGraph(); // 黑色-遥控信号角度
    DataPlot->graph(2)->setPen(myPen);
    DataPlot->graph(2)->setName(tr("滚转输入角度"));

    myPen.setColor(Qt::yellow);
    DataPlot->addGraph(); // 黄色-电机信号滚转Offset
    DataPlot->graph(3)->setPen(myPen);
    DataPlot->graph(3)->setName(tr("电机滚转输出"));

    myPen.setStyle(Qt::DotLine);

    myPen.setColor(Qt::gray);
    DataPlot->addGraph(); // 黄色-电机信号滚转Offset
    DataPlot->graph(4)->setPen(myPen);
    DataPlot->graph(4)->setName(tr("滚转PID:P项"));

    myPen.setColor(Qt::green);
    DataPlot->addGraph(); // 黄色-电机信号滚转Offset
    DataPlot->graph(5)->setPen(myPen);
    DataPlot->graph(5)->setName(tr("滚转PID:D项"));

    //DataPlot->graph(0)->setChannelFillGraph(DataPlot->graph(1));

    DataPlot->yAxis->setRange(-100,100);
    DataPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    DataPlot->xAxis->setDateTimeFormat("hh:mm:ss");
    DataPlot->xAxis->setAutoTickStep(false);
    DataPlot->xAxis->setTickStep(4);
    DataPlot->legend->setVisible(true);
    DataPlot->legend->setPositionStyle(QCPLegend::psTopLeft);

    DataPlot->setupFullAxesBox();
    DataPlot->setRangeDrag(Qt::Horizontal|Qt::Vertical);
    DataPlot->setRangeZoom(Qt::Horizontal|Qt::Vertical);

    // 使上-下、左-右坐标轴的范围一致
    connect(DataPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), DataPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(DataPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), DataPlot->yAxis2, SLOT(setRange(QCPRange)));

    // 设置定时器来重复调用实时绘图函数
    connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
}

//读串口函数
void MainWindow::readMycom()
{
    char onebyte;
    myCom->read(&onebyte,1);    //读取一字节数据
    if(onebyte=='H')                       //读到开始的标记
        OKtoReadH = true;                //表示后面可以读取数据
//    else if(onebyte=='M')//如果不是数据结尾，将其插入到一个QByteArray字节数组
//        OKtoReadM = true;
    else if((onebyte!='\n')&&OKtoReadH)//如果不是数据结尾，将其插入到一个QByteArray字节数组
        temp.append(onebyte);
//    else if((onebyte!='\n')&&OKtoReadM)//如果不是数据结尾，将其插入到一个QByteArray字节数组
//        temp.append(onebyte);
    else if(onebyte=='\n'&&(!temp.isNull()))//到达了结尾，而字节数组不为空时
    {
        comData = temp.split('*');//将数组按*号分割
        if(OKtoReadH)
        {
            //yawAngle = -QString(comData.at(0)).toFloat();
            //pitchAngle = -QString(comData.at(0)).toFloat();
            rollAngle = QString(comData.at(0)).toFloat();
            rollInput = QString(comData.at(1)).toFloat();
            rollOffset = QString(comData.at(2)).toFloat();
//            rollPID_P = QString(comData.at(3)).toFloat();
            rollPID_D = QString(comData.at(3)).toFloat();

            OKtoReadH = false;//标记为数据不可读取

            //inLock = QString(comData.at(3)).toInt();
            if(inLock)
                lockLabel->setText(tr("已锁定"));
            else
                lockLabel->setText(tr("已解锁"));
            qDebug()<<"yawAngle:"<<yawAngle<<"  pitchAngle:"<<pitchAngle<<"   rollAngle:"<<rollAngle;//输出
        }
//        else if(OKtoReadM)
//        {
//            rollInput = QString(comData.at(0)).toFloat();
//            rollOffset = QString(comData.at(1)).toFloat();
//            rollPID_P = QString(comData.at(2)).toFloat();
//            rollPID_D = QString(comData.at(3)).toFloat();
//            OKtoReadM = false;//标记为数据不可读取
//        }
        myCom->readAll();//读取此时串口剩余全部数据，否则输出的显示会延迟很多
        temp.clear();//清空字节数组
    }
}

void MainWindow::realtimeDataSlot()
{
    key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    static double lastPointKey = 0;
    if (key-lastPointKey > 0.01) // at most add point every 10 ms
    {
        // 加入数据点到曲线中:
        DataPlot->graph(0)->addData(key, -pitchAngle);
        DataPlot->graph(1)->addData(key, rollAngle);
        DataPlot->graph(2)->addData(key,rollInput);
        DataPlot->graph(3)->addData(key,rollOffset);
        DataPlot->graph(4)->addData(key,rollPID_P);
        DataPlot->graph(5)->addData(key,rollPID_D);

        lastPointKey = key;
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    DataPlot->xAxis->setRange(key+0.25, 40, Qt::AlignRight);
    DataPlot->replot();
}
