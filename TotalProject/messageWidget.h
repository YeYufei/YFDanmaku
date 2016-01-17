#ifndef MESSAGEWIDGET_H
#define MESSAGEWIDGET_H

#include <QtGui>
#include <QPoint>
#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QPixmap>
#include <QLinearGradient>

class MessageWidget : public QWidget //ShaoeWidget继承了QWidget
{
    Q_OBJECT
    friend class MainWindow;
public:
    void setPaintStatus(int status);
    int getPaintStatus();
    MessageWidget(QWidget *parent=0);        //创建一个空内容的构造函数
protected:
    void paintEvent(QPaintEvent *);             //自定义一个刷屏事件函数
private:
    int paintStatus;

};

#endif // MESSAGEWIDGET_H
