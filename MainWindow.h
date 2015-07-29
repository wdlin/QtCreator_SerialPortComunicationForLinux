#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
class SerialPortThread;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void init();//自定义的初始化函数

private slots:

    void serialPortOpenStatus(int status);//接收串口是否打开的槽
    void serialPortReceivedData(QByteArray receivedData);//接收串口线程传来的数据的槽
    void sendMessage();//接收输入框回车发送信息的槽
    void arrowMessage(int arrow);//接收输入框上下箭头的槽
    void on_onOffSerialPortButton_clicked();//开关串口按钮的槽
    void on_sendMessageButton_clicked();//发送信息按钮的槽
    void on_enterSendCheckBox_clicked();//设置回车发送的检查框的槽
    void on_autoEnterCheckBox_clicked();//设置发送的数据自动换行的检查框的槽
    void on_clearShowButton_clicked();//清除显示信息按钮的槽
    void on_clearInputButton_clicked();//清除输入信息按钮的槽

private:
    Ui::MainWindow *ui;//ui文件
    SerialPortThread* serialPortThread;//串口线程
    bool isEnterSend;//是否回车发送的标志
    bool isAutoEnter;//是否发送数据时自动换行的标志
    QVector<QString> inputHistory;//输入历史的向量
    QVector<QString>::iterator historyIterator;//指向某个历史输入字符串
    QString showString;//显示的字符串

};

#endif // MAINWINDOW_H
