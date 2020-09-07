#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initComponents();
    initSignalAndSlot();
    serialPortScan();
    setSerial();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initComponents(void)
{
    serial = new QSerialPort;

    timer = new QTimer(this);
    timer->start(1000);
}

void MainWindow::initSignalAndSlot(void)
{
    connect(timer,SIGNAL(timeout()),this,SLOT(dataUpdate()));
    connect(ui->refreshSettingsBtn, SIGNAL(clicked()), this, SLOT(setSerial()));
    connect(ui->sendSettingsBtn, SIGNAL(clicked()), this, SLOT(writeData()));
    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
}

void MainWindow::serialPortScan(void)
{
    if(serial!=NULL)
    {
        foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        {
            QSerialPort  ser;
            ser.setPort(info);
            /* 判断端口是否能打开 */
            if(ser.open(QIODevice::ReadWrite))
            {
                ui->serialName->addItem(ser.portName());
                ser.close();
            }
        }
    }
}

void MainWindow::setSerial(void)
{
    //设置串口名//TODO
    serial->setPortName(ui->serialName->currentText());
    //设置波特率
    serial->setBaudRate(ui->baudRate->currentText().toInt());
    //设置数据位数
    switch(ui->dataBits->currentText().toInt())
    {
        case 5:serial->setDataBits(QSerialPort::Data5);break;
        case 6:serial->setDataBits(QSerialPort::Data6);break;
        case 7:serial->setDataBits(QSerialPort::Data7);break;
        case 8:serial->setDataBits(QSerialPort::Data8);break;
        default:
        {
            QMessageBox::information(this, "警告", "数据位设置出错");
        }
    }
    //设置奇偶校验
    switch(ui->parity->currentIndex())
    {
        case 0:serial->setParity(QSerialPort::OddParity);break;
        case 1:serial->setParity(QSerialPort::EvenParity);break;
        case 2:serial->setParity(QSerialPort::NoParity);break;
        default:
        {
            QMessageBox::information(this, "警告", "奇偶校验位设置出错");
        }
    }
    //设置停止位
    serial->setStopBits(QSerialPort::OneStop);
    switch(ui->stopBits->currentIndex())
    {
        case 0:serial->setStopBits(QSerialPort::OneStop);break;
        case 1:serial->setStopBits(QSerialPort::OneAndHalfStop);break;
        case 2:serial->setStopBits(QSerialPort::TwoStop);break;
        default:
        {
            QMessageBox::information(this, "警告", "停止位设置出错");
        }
    }
    //设置流控制
    serial->setFlowControl(QSerialPort::NoFlowControl);
    //打开串口
    serial->open(QIODevice::ReadWrite);

//    QMessageBox::information(this, "提示", "串口设置成功");
}

void MainWindow::dataUpdate(void)
{
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("yyyy-MM-dd hh:mm:ss dddd");
    ui->timeLab->setText(str);
}

void MainWindow::readData(void)
{
    QByteArray buf;
    buf = serial->readAll();
    //TODO 数据协议
}

void MainWindow::writeData(void)
{
    //TODO
    //serial->write(data);
}
