#ifndef DANMU_H
#define DANMU_H

#include <QLabel>
#include <QRect>
#include <QColor>
#include <QDebug>
#include <QTextCharFormat>
#include <QPainter>
#include <iostream>
#include <ctime>
#include <QTime>
#include <QThread>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include "limits"
class Danmu : public QLabel{

    Q_OBJECT

  public:

      Danmu(QWidget *parent);       //构造函数，默认一般不用

      Danmu(QWidget * parent, QString text, QRect rect);        //构造函数，一般也不用

      Danmu(QWidget * parent,QString text,QString color,int type,QRect rect,QFont danmuFont = QFont("SimHei",20,100),double Transparency = 1.00,int runTime=15000);       //构造函数，常用

      ~Danmu();     //析构函数

      void Fly();       //弹幕飞函数

      //一些成员变量的Get方法与Set方法
      int getPosX();

      int getPosY();

      void setPosX(int posx);

      void setPosY(int posy);

      bool DanmuisOut();

      bool DanmuisNotIn();

      void setColor(QString color);

      QString getColor();

      void setType(int type);

      int getType();

      void setQColor(QColor qcolor);

      int getLastTime();

      void setLastTime(int lastTime);

      QColor getQColor();

      void setQFont(QFont danmuFont);

      QFont getQFont();

      void setTransparency(double Transparency);

      double getTransparency();

      void setScreenRect(QRect screenRect);

      QRect getScreenRect();

      int getRunTime();

      void setRunTime(int runTime);

      QPropertyAnimation * getanimation();

  public slots:
      void changeDanmuShow();
      void changeTopShow();
      void changeBottomShow();

  protected:
      void paintEvent(QPaintEvent *);       //重点，弹幕的绘制函数

  private:
      int PosX;
      int PosY;
      bool isShow;
      QString DText;
      QString color;
      QColor qcolor;
      int lastTime;
      int type;
      QFont danmuFont;
      int DHeight;
      double Transparency;
      QRect screenrect;
      QPropertyAnimation *anim2;
      int runTime;
      bool showDanmu,topShow,bottomShow;
};

#endif // DANMU_H
