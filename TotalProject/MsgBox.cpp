#include "MsgBox.h"

MsgBox::MsgBox(QWidget *parent,QString title,QString content)
    :QDialog(parent)
{

    setWindowOpacity(0.4);
    int width = this->width();
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);
    this->resize(378,400);
    QPalette pal;
    pal.setBrush(QPalette::Background,QColor(37,37,37));
    this->setPalette(pal);

    wndShadow = new QGraphicsDropShadowEffect;
    wndShadow->setBlurRadius(15);
    wndShadow->setColor(QColor(255,233,72,255));
    wndShadow->setOffset(0);
    this->setGraphicsEffect(wndShadow);

    mousePressed=false;
    Iconlabel = new QLabel(this);
    Iconlabel->setFixedSize(46,46);
    Iconlabel->setPixmap(QPixmap(":/project/netIcon"));
    titleLabel = new QLabel(this);
    QPalette pal1;
    pal1.setBrush(QPalette::WindowText,QColor(224,224,224));
    titleLabel->setPalette(pal1);
    titleLabel->setText(title);
    closeButton = new QPushButton(this);
    closeButton->setFixedSize(27,27);
    closeButton->setStyleSheet("QPushButton{border-image:url(:/new/dialog1);}"
                               "QPushButton:hover{border-image:url(:/new/dialog2);}"
                               "QPushButton:pressed{border-image:url(:/new/dialog3);}");

    //设置提示图片
     msgLabel = new QLabel(this);
     msgLabel->setGeometry(20, 50,36, 36);
     msgLabel->setScaledContents(true);
     msgLabel->setPixmap(QPixmap(":/project/netIcon"));
     msgLabel->setFixedSize(42,42);

     //设置提示信息，让QLabel能够自动判断并换行显示：
     askLabel = new QLabel(this);
     askLabel->setFont(QFont("SimHei",15,1));
     askLabel->setGeometry(65, 60,width-100, 25*2);
     askLabel->setWordWrap(true);
     askLabel->setAlignment(Qt::AlignVCenter);
     askLabel->setText("  "+content);
     QPalette pal2;
     pal2.setBrush(QPalette::WindowText,QColor(224,224,224));
     askLabel->setPalette(pal2);

     YesButton = new QPushButton(this);
     YesButton->setFixedSize(70,24);
     QPalette pal3;
     pal3.setBrush(QPalette::ButtonText,QColor(254,254,254));
     YesButton->setPalette(pal3);
     YesButton->setText("关闭");
     YesButton->setStyleSheet("QPushButton{border-image:url(:/new/diaYes1);}"
                              "QPushButton:hover{border-image:url(:/new/diaYes2);}"
                              "QPushButton:pressed{border-image:url(:/new/diaYes3);}");

    QHBoxLayout * layout1=new QHBoxLayout();
    layout1->setSpacing(0);
    layout1->addWidget(Iconlabel);
    layout1->addSpacing(5);
    layout1->addWidget(titleLabel);
    layout1->addStretch();
    layout1->addWidget(closeButton);

    QHBoxLayout * layout2 = new QHBoxLayout();
    layout2->addSpacing(50);
    layout2->addWidget(msgLabel);
    layout2->addWidget(askLabel);
    layout2->addSpacing(50);
    QHBoxLayout * layout4 = new QHBoxLayout();
    layout4->addStretch();
    layout4->addWidget(YesButton);

    QVBoxLayout * layout3 = new QVBoxLayout(this);
    layout3->addLayout(layout1);
    layout3->addLayout(layout2);
    layout3->addLayout(layout4);
    //this->setLayout(layout1);



    QObject::connect(closeButton,SIGNAL(clicked()),this,SLOT(close()));
    QObject::connect(YesButton,SIGNAL(clicked()),this,SLOT(close()));

    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
     animation->setDuration(2000);
     animation->setStartValue(0);
     animation->setEndValue(1);
     animation->start();
}


void MsgBox::mousePressEvent(QMouseEvent * event)
{
    if(event->button()==Qt::LeftButton)
        mousePressed=true;
    movepoint=event->globalPos()-pos();
}

void MsgBox::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed=false;
}

void MsgBox::mouseMoveEvent(QMouseEvent *event)
{
    if(mousePressed)
      {
        QPoint move_pos = event->globalPos();
        move(move_pos - movepoint);
      }
}

MsgBox::~MsgBox()
{


}
