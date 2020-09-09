#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTextCodec>
#include <QTextStream>
#include <QDebug>

#include <synchapi.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initComponents();
    initSignalAndSlot();
    serialPortScan();

    flag_serial = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initComponents(void)
{
    serial = new QSerialPort;
    ui->inNumbLab->setText("0");
    ui->outNumbLab->setText("0");
    ui->inTrafficLab->setText("0");
    ui->outTrafficLab->setText("0");
    ui->inThresholdTex->setText("0");
    ui->outThresholdTex->setText("0");

    timer = new QTimer(this);
    timer->start(1000);
}

void MainWindow::initSignalAndSlot(void)
{
    connect(timer,SIGNAL(timeout()),this,SLOT(dataUpdate()));
    connect(ui->openBtn, SIGNAL(clicked()), this, SLOT(openSerial()));
    connect(ui->closeBtn, SIGNAL(clicked()), this, SLOT(closeSerial()));
}

void MainWindow::serialPortScan(void)
{
    if(serial != NULL)
    {
        foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        {
            QSerialPort ser;
            ser.setPort(info);
            //判断端口是否能打开
            if(ser.open(QIODevice::ReadWrite))
            {
                ui->serialName->addItem(ser.portName());
                ser.close();
            }
        }
    }
}

void MainWindow::openSerial(void)
{
    //设置串口名
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
            QMessageBox::critical(this, "警告", "数据位设置出错");
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
            QMessageBox::critical(this, "警告", "奇偶校验位设置出错");
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
            QMessageBox::critical(this, "警告", "停止位设置出错");
        }
    }
    //设置流控制
    serial->setFlowControl(QSerialPort::NoFlowControl);
    //打开串口
    if(!serial->open(QIODevice::ReadWrite)){
        serial->close();
        serial->open(QIODevice::ReadWrite);
    }
    QMessageBox::information(this, "提示", "串口打开成功");

    flag_serial = 1;
}

void MainWindow::closeSerial(void)
{
    serial->close();
    QMessageBox::information(this, "提示", "串口关闭成功");
    flag_serial = 0;
}

void MainWindow::dataUpdate(void)
{
    datatime = QDateTime::currentDateTime();
    if(flag_serial == 1)
    {
        QTime time = QTime::currentTime();
        if(time.operator>=(ui->startTime->time()) && time.operator<=(ui->endTime->time()))
        {
            saveData();
        }
        QString str = datatime.toString("yyyy-MM-dd hh:mm:ss dddd");
        ui->timeLab->setText(str);
        readData();
        writeData();
    }
}

void MainWindow::readData(void)
{
    QByteArray buf;
    buf = serial->readAll();
    qDebug() << buf;
    if(buf.size() == 8)
    {
        QString str1;
        str1.append(buf.at(0));
        str1.append(buf.at(1));
        ui->inNumbLab->setText(str1);

        QString str2;
        str2.append(buf.at(2));
        str2.append(buf.at(3));
        ui->outNumbLab->setText(str2);

        QString str3;
        str3.append(buf.at(4));
        str3.append(buf.at(5));
        ui->inTrafficLab->setText(str3);

        QString str4;
        str4.append(buf.at(6));
        str4.append(buf.at(7));
        ui->outTrafficLab->setText(str4);
    }
}

void MainWindow::writeData(void)
{
    QByteArray data;
    data.append(ui->inThresholdTex->text());//进入人流量阈值
    data.append(ui->outThresholdTex->text());//出去人流量阈值

    QString str = datatime.toString("hhmmss");
    data.append(str);//时间
    data.append("\n");
    serial->write(data);
}

void MainWindow::saveData(void)
{
    QString sFilePath = "InfraredCounterData.txt";

    QFile file(sFilePath);
    //方式：Append为追加，WriteOnly，ReadOnly
    if (!file.open(QIODevice::Append|QIODevice::Text)) {
        QMessageBox::information(this, "提示", "无法创建文件");
    }
    //获得文件大小
    qint64 pos;
    pos = file.size();
    //重新定位文件输入位置，这里是定位到文件尾端
    file.seek(pos);

    QTextCodec *codec = QTextCodec::codecForName("UTF8");

    QString str;
    str.append(datatime.toString(codec->toUnicode("yyyy-MM-dd hh:mm:ss ")));
    str.append("in:");
    str.append(ui->inNumbLab->text());
    str.append(" ");
    str.append("out:");
    str.append(ui->outNumbLab->text());
    str.append("\n");

    //写入文件
    file.write(str.toLatin1(),str.length());
}
