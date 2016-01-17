#ifndef DOGE_H
#define DOGE_H

#include <QLabel>
#include <QRect>
#include <QDebug>
#include <QPainter>
#include <ctime>
#include <QTime>
#include <QPixmap>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QMovie>
class Doge : public QLabel
{
    Q_OBJECT
public:
    explicit Doge(QWidget *parent = 0);

    Doge(QWidget *parent, QPixmap * pic2,QRect rect,int type = 4,double Transparency = 1.00,int runTime=15000,int qType = 0 );

    Doge(QWidget *parent, QMovie * movie2,QRect rect,int type = 4,double Transparency = 1.00,int runTime=15000);

    ~Doge();

    void Fly();       //弹幕飞函数

    //一些成员变量的Get方法与Set方法
    int getPosX();

    int getPosY();

    void setPosX(int posx);

    void setPosY(int posy);

    bool DogeisOut();

    void setType(int type);

    int getType();

    void setScreenRect(QRect screenRect);

    QRect getScreenRect();

    bool DanmuisNotIn();

    int getRunTime();

    void setRunTime(int runTime);

    void setTransparency(double Transparency);

    double getTransparency();

    QPropertyAnimation * getanimation();

    QSequentialAnimationGroup * getgroup();

signals:

public slots:
    void changeDogeShow();

private:
    int PosX;
    int PosY;
    bool isShow;
    int type;
    QMovie * movie;
    QRect screenrect;
    QPropertyAnimation *anim2,*anim3,*anim4,*anim5;
    double Transparency;
    int runTime;
    bool showDoge;
    bool isMov;
    QSequentialAnimationGroup * group;
    QPixmap pic;

};

#endif // DOGE_H
