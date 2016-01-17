#include "doge.h"

Doge::Doge(QWidget *parent) :
    QLabel(parent)
{
}

Doge::~Doge(){
    if(this->movie !=NULL)
        delete movie;
    //if(this->pic != NULL)
    //    delete pic;
    //pic = NULL;
    movie = NULL;
    qDebug()<<"Doge被杀死了。。。555"<<endl;
}

Doge::Doge(QWidget *parent, QPixmap * pic2,QRect rect,int type,double Transparency,int runTime,int qType):QLabel(parent){
    //pic = NULL;
    movie = NULL;
    isMov = false;
    pic = pic2->scaled((pic2->width())*((rect.height())/(pic2->height())),rect.height(),Qt::KeepAspectRatio);
    delete pic2;
    this->setPixmap(pic);
    this->setFixedSize(pic.size());
    showDoge = true;
    this->setType(type);
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::Tool|Qt::WindowStaysOnTopHint);
    this->setTransparency(Transparency);        //弹幕透明度
    this->setRunTime(runTime);
    this->adjustSize();
    this->setWordWrap(false);
    this->setAlignment(Qt::AlignTop);
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setFocusPolicy(Qt::NoFocus);
    this->setStyleSheet(" border: none;padding: 0;"
                        "background: none;");

    //this->setMovie(this->movie);
    //this->movie->start();
    //qDebug()<<this->movie->isValid();

    if(qType == 0){
        int y = rect.height();
        int xx = -(pic.size().rwidth());
        this->move(xx,y);
        this->setPosX(xx);
        this->setPosY(y);
        group = new QSequentialAnimationGroup(this);
        anim5=new QPropertyAnimation(this, "pos");
        anim5->setDuration(100);
        anim5->setStartValue(QPoint(this->getPosX(),this->getPosY()));
        anim5->setEndValue(QPoint(this->getPosX(),this->getPosY()));
        anim5->setEasingCurve(QEasingCurve::Linear);
        anim2=new QPropertyAnimation(this, "pos");
        anim2->setDuration(200);
        anim2->setStartValue(QPoint(this->getPosX(),this->getPosY()));
        anim2->setEndValue(QPoint(0, rect.height()-pic.size().rheight()));
        anim2->setEasingCurve(QEasingCurve::Linear);
        anim3=new QPropertyAnimation(this, "pos");
        anim3->setDuration(500);
        anim3->setStartValue(QPoint(0, rect.height()-pic.size().rheight()));
        anim3->setEndValue(QPoint(0, rect.height()-pic.size().rheight()));
        anim3->setEasingCurve(QEasingCurve::Linear);
        anim4=new QPropertyAnimation(this, "pos");
        anim4->setDuration(200);
        anim4->setEndValue(QPoint(this->getPosX(),this->getPosY()));
        anim4->setStartValue(QPoint(0, rect.height()-pic.size().rheight()));
        anim4->setEasingCurve(QEasingCurve::Linear);
        group->addAnimation(anim5);
        group->addAnimation(anim2);
        group->addAnimation(anim3);
        group->addAnimation(anim4);
        this->setWindowOpacity(this->getTransparency());
        this->show();
        this->repaint();
    }else if(qType == 1){
        int y = rect.height();
        int xx = rect.width();
        this->move(xx,y);
        this->setPosX(xx);
        this->setPosY(y);
        group = new QSequentialAnimationGroup(this);
        anim5=new QPropertyAnimation(this, "pos");
        anim5->setDuration(300);
        anim5->setStartValue(QPoint(this->getPosX(),this->getPosY()));
        anim5->setEndValue(QPoint(this->getPosX(),this->getPosY()));
        anim5->setEasingCurve(QEasingCurve::Linear);
        anim2=new QPropertyAnimation(this, "pos");
        anim2->setDuration(200);
        anim2->setStartValue(QPoint(this->getPosX(),this->getPosY()));
        anim2->setEndValue(QPoint(rect.width()-pic.size().rwidth(), rect.height()-pic.size().rheight()));
        anim2->setEasingCurve(QEasingCurve::Linear);
        anim3=new QPropertyAnimation(this, "pos");
        anim3->setDuration(500);
        anim3->setStartValue(QPoint(rect.width()-pic.size().rwidth(), rect.height()-pic.size().rheight()));
        anim3->setEndValue(QPoint(rect.width()-pic.size().rwidth(), rect.height()-pic.size().rheight()));
        anim3->setEasingCurve(QEasingCurve::Linear);
        anim4=new QPropertyAnimation(this, "pos");
        anim4->setDuration(200);
        anim4->setEndValue(QPoint(this->getPosX(),this->getPosY()));
        anim4->setStartValue(QPoint(rect.width()-pic.size().rwidth(), rect.height()-pic.size().rheight()));
        anim4->setEasingCurve(QEasingCurve::Linear);
        group->addAnimation(anim5);
        group->addAnimation(anim2);
        group->addAnimation(anim3);
        group->addAnimation(anim4);
        this->setWindowOpacity(this->getTransparency());
        this->show();
        this->repaint();
    }else if(qType == 2){
        int y = -(pic.size().rheight());
        int xx = rect.width();
        this->move(xx,y);
        this->setPosX(xx);
        this->setPosY(y);
        group = new QSequentialAnimationGroup(this);
        anim5=new QPropertyAnimation(this, "pos");
        anim5->setDuration(500);
        anim5->setStartValue(QPoint(this->getPosX(),this->getPosY()));
        anim5->setEndValue(QPoint(this->getPosX(),this->getPosY()));
        anim5->setEasingCurve(QEasingCurve::Linear);
        anim2=new QPropertyAnimation(this, "pos");
        anim2->setDuration(200);
        anim2->setStartValue(QPoint(this->getPosX(),this->getPosY()));
        anim2->setEndValue(QPoint(rect.width()-pic.width(), 0));
        anim2->setEasingCurve(QEasingCurve::Linear);
        anim3=new QPropertyAnimation(this, "pos");
        anim3->setDuration(500);
        anim3->setStartValue(QPoint(rect.width()-pic.width(), 0));
        anim3->setEndValue(QPoint(rect.width()-pic.width(), 0));
        anim3->setEasingCurve(QEasingCurve::Linear);
        anim4=new QPropertyAnimation(this, "pos");
        anim4->setDuration(200);
        anim4->setEndValue(QPoint(this->getPosX(),this->getPosY()));
        anim4->setStartValue(QPoint(rect.width()-pic.width(), 0));
        anim4->setEasingCurve(QEasingCurve::Linear);
        group->addAnimation(anim5);
        group->addAnimation(anim2);
        group->addAnimation(anim3);
        group->addAnimation(anim4);
        this->setWindowOpacity(this->getTransparency());
        this->show();
        this->repaint();
    }else if(qType == 3){
        int y = -(pic.size().rheight());
        int xx = -(pic.size().rwidth());
        this->move(xx,y);
        this->setPosX(xx);
        this->setPosY(y);
        group = new QSequentialAnimationGroup(this);
        anim5=new QPropertyAnimation(this, "pos");
        anim5->setDuration(700);
        anim5->setStartValue(QPoint(this->getPosX(),this->getPosY()));
        anim5->setEndValue(QPoint(this->getPosX(),this->getPosY()));
        anim5->setEasingCurve(QEasingCurve::Linear);
        anim2=new QPropertyAnimation(this, "pos");
        anim2->setDuration(200);
        anim2->setStartValue(QPoint(this->getPosX(),this->getPosY()));
        anim2->setEndValue(QPoint(0,0));
        anim2->setEasingCurve(QEasingCurve::Linear);
        anim3=new QPropertyAnimation(this, "pos");
        anim3->setDuration(500);
        anim3->setStartValue(QPoint(0,0));
        anim3->setEndValue(QPoint(0,0));
        anim3->setEasingCurve(QEasingCurve::Linear);
        anim4=new QPropertyAnimation(this, "pos");
        anim4->setDuration(200);
        anim4->setEndValue(QPoint(this->getPosX(),this->getPosY()));
        anim4->setStartValue(QPoint(0,0));
        anim4->setEasingCurve(QEasingCurve::Linear);
        group->addAnimation(anim5);
        group->addAnimation(anim2);
        group->addAnimation(anim3);
        group->addAnimation(anim4);
        this->setWindowOpacity(this->getTransparency());
        this->show();
        this->repaint();
    }
    //QSequentialAnimationGroup group;
    //QParallelAnimationGroup group;
    //group.addAnimation(anim1);
    //group.addAnimation(anim2);
    //group.start();
    group->start();
    connect(group,SIGNAL(finished()),this,SLOT(deleteLater()));
    this->show();
}

Doge::Doge(QWidget *parent, QMovie * movie2,QRect rect,int type,double Transparency,int runTime):QLabel(parent){
    //pic = NULL;
    movie = NULL;
    isMov = true;
    this->movie = movie2;
    showDoge = true;
    this->setType(type);
    this->setFixedHeight(100);
    this->setFixedWidth(100);
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::Tool|Qt::WindowStaysOnTopHint);
    this->setTransparency(Transparency);        //弹幕透明度
    this->setRunTime(runTime);
    this->adjustSize();
    this->setWordWrap(false);
    this->setAlignment(Qt::AlignTop);
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setFocusPolicy(Qt::NoFocus);
    this->setStyleSheet(" border: none;padding: 0;"
                        "background: none;");

    this->setMovie(this->movie);
    this->movie->start();
    //qDebug()<<this->movie->isValid();
    int yy = qrand()%rect.height();
    int y = yy<(rect.height()-100)?(yy):(rect.height()-100);
    int xx = rect.width()+qrand()%500;
    this->move(xx,y);
    this->setPosX(xx);
    this->setPosY(y);
    anim2=new QPropertyAnimation(this, "pos");
    anim2->setDuration(this->getRunTime());
    anim2->setStartValue(QPoint(this->getPosX(),this->getPosY()));
    anim2->setEndValue(QPoint(-(this->width()), this->getPosY()));
    anim2->setEasingCurve(QEasingCurve::Linear);
    this->setWindowOpacity(this->getTransparency());
    this->show();
    this->repaint();
    anim2->start();
    connect(anim2,SIGNAL(finished()),this,SLOT(deleteLater()));
    this->show();
}

void Doge::Fly(){
    if(!DogeisOut()){
        this->PosX-=4;      //位移
        this->move(PosX,PosY);
    }else{

    }
}

void Doge::setScreenRect(QRect screenRect){
    this->screenrect = screenRect;
}

QRect Doge::getScreenRect(){
    return this->screenrect;
}

bool Doge::DanmuisNotIn(){
    if(this->getPosX()>screenrect.width())
        return true;
    return false;

}

int Doge::getPosX(){
    return PosX;
}

int Doge::getPosY(){
    return PosY;
}

void Doge::setPosX(int posx){
    this->PosX = posx;
}

void Doge::setPosY(int posy){
    this->PosY = posy;
}

bool Doge::DogeisOut(){       //判断弹幕是否飞出了窗口画面
    if(this->PosX<(-this->width())){
        return true;
    }
    return false;
}

int Doge::getType(){
    return type;
}

void Doge::setType(int type){
    this->type = type;
}

void Doge::setRunTime(int runTime){
    this->runTime = runTime;
}

int Doge::getRunTime(){
    return this->runTime;
}

void Doge::setTransparency(double Transparency){
    this->Transparency = Transparency;
}

double Doge::getTransparency(){
    return Transparency;
}

QPropertyAnimation * Doge::getanimation(){
    return this->anim2;
}

QSequentialAnimationGroup * Doge::getgroup(){
    return this->group;
}

void Doge::changeDogeShow(){
    if(this->showDoge){
        this->setWindowOpacity(0);
        if(isMov)
        this->anim2->pause();
        else
        this->group->pause();
        this->showDoge = false;
    }else{
        this->setWindowOpacity(this->getTransparency());
        if(isMov)
        this->anim2->resume();
        else
        this->group->resume();
        this->showDoge = true;
    }
}


