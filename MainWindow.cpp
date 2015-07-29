#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "SerialPortThread.h"
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),isEnterSend(true),isAutoEnter(true),historyIterator(inputHistory.end()),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    serialPortThread = new SerialPortThread();
}

MainWindow::~MainWindow()
{
    delete ui;
    if(serialPortThread->isRunning())
    {
        serialPortThread->setIsEnd(true);
        serialPortThread->wait();//qt的wait类似于join()
    }
    delete serialPortThread;
}
//自定义的初始化函数
void MainWindow::init()
{
    connect(serialPortThread,SIGNAL(serialPortOpenStatus(int)),this,SLOT(serialPortOpenStatus(int)));
    connect(serialPortThread,SIGNAL(serialPortReceivedData(QByteArray)),this,SLOT(serialPortReceivedData(QByteArray)));
    connect(ui->inputPlainTextEdit,SIGNAL(sendMessage()),this,SLOT(sendMessage()));
    connect(ui->inputPlainTextEdit,SIGNAL(arrowMessage(int)),this,SLOT(arrowMessage(int)));
}

//接收串口是否打开的槽
void MainWindow::serialPortOpenStatus(int status)
{
    switch(status)
    {
    case -1://这个是点击打开按钮，但是失败的响应
    {
        ui->onOffSerialPortButton->setText(QStringLiteral("打开串口"));
        ui->serialPortLineEdit->setEnabled(true);
        ui->baudRateComboBox->setEnabled(true);
        ui->parityComboBox->setEnabled(true);
        ui->dataBitsComboBox->setEnabled(true);
        ui->stopBitsComboBox->setEnabled(true);
        QMessageBox::warning(NULL, QStringLiteral("错误"), QStringLiteral("串口号不存在或者被其他程序占用!"), QMessageBox::Ok, QMessageBox::Ok);
        break;
    }
    case 0://正常关闭串口
    {
        ui->onOffSerialPortButton->setText(QStringLiteral("打开串口"));
        ui->serialPortLineEdit->setEnabled(true);
        ui->baudRateComboBox->setEnabled(true);
        ui->parityComboBox->setEnabled(true);
        ui->dataBitsComboBox->setEnabled(true);
        ui->stopBitsComboBox->setEnabled(true);
        break;
    }
    case 1://成功打开串口
    {
        ui->onOffSerialPortButton->setText(QStringLiteral("关闭串口"));
        ui->serialPortLineEdit->setEnabled(false);
        ui->baudRateComboBox->setEnabled(false);
        ui->parityComboBox->setEnabled(false);
        ui->dataBitsComboBox->setEnabled(false);
        ui->stopBitsComboBox->setEnabled(false);
        break;
    }
    default:
        break;

    }
}
//接收串口线程传来的数据的槽
void MainWindow::serialPortReceivedData(QByteArray receivedData)
{
    showString.append(receivedData);
    ui->textBrowser->clear();
    ui->textBrowser->append(showString);
}
//接收输入框回车发送信息的槽
void MainWindow::sendMessage()
{
    if(!serialPortThread->isRunning())return;//如果串口线程没有运行，不发送数据
    QString sendStr = ui->inputPlainTextEdit->toPlainText();
    inputHistory.push_back(sendStr.trimmed());
    historyIterator = inputHistory.end();//每一次发送数据，都要将其置end()
    ui->inputPlainTextEdit->clear();
    serialPortThread->setSendData(sendStr.toUtf8());//QByteArray()
    serialPortThread->setSendFlag(true);
}
//接收输入框上下箭头的槽
void MainWindow::arrowMessage(int arrow)
{
    if(inputHistory.isEmpty())
        return;
    if(arrow == 1)//up
    {
        historyIterator--;
        if(historyIterator == inputHistory.begin() - 1)
            historyIterator = inputHistory.end() - 1;
    }
    if(arrow == 0)//down
    {
        if(historyIterator == inputHistory.end())
            historyIterator = inputHistory.begin();
        else if(historyIterator == inputHistory.end() - 1)
            historyIterator = inputHistory.begin();
        else
            historyIterator++;
    }

    ui->inputPlainTextEdit->clear();
    ui->inputPlainTextEdit->appendPlainText(*historyIterator);//显示历史输入
}
//开关串口按钮的槽
void MainWindow::on_onOffSerialPortButton_clicked()
{
    //如果正在运行，则关闭串口线程
    if(serialPortThread->isRunning())
    {
        serialPortThread->setIsEnd(true);
    }
    else//打开串口线程
    {//首先设置串口参数
        serialPortThread->setSerialPortNumber(ui->serialPortLineEdit->text());

        serialPortThread->setSerialPortBaudRate(ui->baudRateComboBox->currentText().toInt());

        QSerialPort::Parity parity;
        switch(ui->parityComboBox->currentIndex())
        {
        case 0:
            parity = QSerialPort::NoParity;
            break;
        case 1:
            parity = QSerialPort::EvenParity;
            break;
        case 2:
            parity = QSerialPort::OddParity;
            break;
        case 3:
            parity = QSerialPort::SpaceParity;
            break;
        case 4:
            parity = QSerialPort::MarkParity;
            break;
        default:
            parity = QSerialPort::UnknownParity;
            break;
        }
        serialPortThread->setSerialPortParity(parity);

        QSerialPort::DataBits dataBits;
        switch(ui->dataBitsComboBox->currentIndex())
        {
        case 0:
            dataBits = QSerialPort::Data5;
            break;
        case 1:
            dataBits = QSerialPort::Data6;
            break;
        case 2:
            dataBits = QSerialPort::Data7;
            break;
        case 3:
            dataBits = QSerialPort::Data8;
            break;

        default:
            dataBits = QSerialPort::UnknownDataBits;
            break;
        }
        serialPortThread->setSerialPortDataBits(dataBits);

        QSerialPort::StopBits stopBits;
        switch(ui->dataBitsComboBox->currentIndex())
        {
        case 0:
            stopBits = QSerialPort::OneStop;
            break;
        case 1:
            stopBits = QSerialPort::OneAndHalfStop;
            break;
        case 2:
            stopBits = QSerialPort::TwoStop;
            break;

        default:
            stopBits = QSerialPort::UnknownStopBits;
            break;
        }
        serialPortThread->setSerialPortStopBits(stopBits);
        serialPortThread->setIsEnd(false);//设置停止标志为false
        serialPortThread->start();//开始线程
    }

}
//发送信息按钮的槽
void MainWindow::on_sendMessageButton_clicked()
{
    if(!serialPortThread->isRunning())return;//如果串口线程没有运行，不发送数据
    QString sendStr = ui->inputPlainTextEdit->toPlainText();
    inputHistory.push_back(sendStr.trimmed());//添加历史指令
    historyIterator = inputHistory.end();
    ui->inputPlainTextEdit->clear();//清空输入框
    if(isAutoEnter)sendStr += "\n";//如果自动换行
    serialPortThread->setSendData(sendStr.toUtf8());//QByteArray()
    serialPortThread->setSendFlag(true);
}

//设置回车发送的检查框的槽
void MainWindow::on_enterSendCheckBox_clicked()
{
    isEnterSend = ui->enterSendCheckBox->isChecked();
    ui->inputPlainTextEdit->setIsEnterSend(isEnterSend);
}
//设置发送的数据自动换行的检查框的槽
void MainWindow::on_autoEnterCheckBox_clicked()
{
    isAutoEnter = ui->autoEnterCheckBox->isChecked();
}
//清除显示信息按钮的槽
void MainWindow::on_clearShowButton_clicked()
{
    ui->textBrowser->clear();
    showString.clear();
}

//清除输入信息按钮的槽
void MainWindow::on_clearInputButton_clicked()
{
    ui->inputPlainTextEdit->clear();
}
