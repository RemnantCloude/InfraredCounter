#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>
#include <QDateTime>
#include <QMessageBox>
#include <QFile>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;
    QTimer *timer;
    QDateTime datatime;
    QTime time;
    bool flag_serial;

    void initComponents(void);
    void initSignalAndSlot(void);
    void serialPortScan(void);
    void saveData(void);
    void readData(void);

private slots:
    void dataUpdate(void);
    void openSerial(void);
    void closeSerial(void);
    void writeData(void);
};

#endif // MAINWINDOW_H
