#include "InputPlainTextEdit.h"

InputPlainTextEdit::InputPlainTextEdit(QWidget* parent):QPlainTextEdit(parent),isEnterSend(true)
{

}



InputPlainTextEdit::~InputPlainTextEdit()
{

}
//设置是否回车发送信息
void InputPlainTextEdit::setIsEnterSend(bool b)
{
    isEnterSend = b;
}

//按键响应函数
void InputPlainTextEdit::keyPressEvent(QKeyEvent *event)
{
    QPlainTextEdit::keyPressEvent(event);//必须增加这个，否则无法输入框正常编辑
    int key = event->key();

    if(key == Qt::Key_Up)
        emit arrowMessage(1);
    if(key == Qt::Key_Down)
        emit arrowMessage(0);

    if(!isEnterSend)return;

    if(key == Qt::Key_Return || key == Qt::Key_Enter)//分别是大小键盘的回车
    {
        emit sendMessage();
    }
}



