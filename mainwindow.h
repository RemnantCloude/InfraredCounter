#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>
#include <QMessageBox>

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

    void initComponents(void);
    void initSignalAndSlot(void);
    void serialPortScan(void);

private slots:
    void dataUpdate(void);
    void setSerial(void);
    void readData(void);
    void writeData(void);
};

#endif // MAINWINDOW_H
