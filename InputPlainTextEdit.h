/*
 * 这个文件是命令输入框的类，重载了按键事件响应等
*/
#ifndef INPUTPLAINTEXTEDIT_H
#define INPUTPLAINTEXTEDIT_H
#include "QPlainTextEdit"

class InputPlainTextEdit:public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit InputPlainTextEdit(QWidget* parent = 0);
    ~InputPlainTextEdit();
    void keyPressEvent(QKeyEvent * event);//按键响应函数

    void setIsEnterSend(bool b);//设置是否回车发送信息

signals:
    void sendMessage();//发送发送信息的信号给主界面
    void arrowMessage(int array);//发送点击上下箭头的信号给主界面

private:
    bool isEnterSend;//回车发送信息标志，true表示回车发送

};

#endif // INPUTPLAINTEXTEDIT_H
