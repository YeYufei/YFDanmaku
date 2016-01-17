#include "messageWidget.h"

MessageWidget::MessageWidget(QWidget * parent)
    :QWidget(parent){
    paintStatus = 0;
}

void MessageWidget::setPaintStatus(int status){
    this->paintStatus = status;
}

int MessageWidget::getPaintStatus(){
    return this->paintStatus;
}

void MessageWidget::paintEvent(QPaintEvent *){
    if(this->getPaintStatus()==0){
        //设置画笔
        QPen objPen(Qt::NoBrush,1);
        QPainter Painter(this);
        Painter.setPen(objPen);
        //设置渐变画刷
        QLinearGradient objLinear(rect().topLeft(),rect().bottomLeft());
        //顶部颜色和透明度
        objLinear.setColorAt(0,QColor(0,160,234,240));
        //中间颜色和透明度
        objLinear.setColorAt(0.9,QColor(51,122,183,255));
        //底部颜色和透明度
        objLinear.setColorAt(1,QColor(51,122,183,255));
        QBrush objBrush(objLinear);
        Painter.setBrush(objBrush);
        //画圆角矩形
        //Painter.drawRoundedRect(rect(),5,5);
        Painter.drawRect(rect());
    }else if(this->getPaintStatus()==1){
        QPixmap pixmap = QPixmap(":/new/mbg1").scaled(this->size());
        QPainter painter(this);
        painter.drawPixmap(this->rect(),pixmap);
    }else if(this->getPaintStatus()==2){
        QPixmap pixmap = QPixmap(":/new/mbg2").scaled(this->size());
        QPainter painter(this);
        painter.drawPixmap(this->rect(),pixmap);
    }else if(this->getPaintStatus()==3){
        QPixmap pixmap = QPixmap(":/new/mbg3").scaled(this->size());
        QPainter painter(this);
        painter.drawPixmap(this->rect(),pixmap);
    }else if(this->getPaintStatus()==4){
        QPixmap pixmap = QPixmap(":/new/mbg4").scaled(this->size());
        QPainter painter(this);
        painter.drawPixmap(this->rect(),pixmap);
    }else if(this->getPaintStatus()==5){
        QPixmap pixmap = QPixmap(":/new/mbg5").scaled(this->size());
        QPainter painter(this);
        painter.drawPixmap(this->rect(),pixmap);
    }else if(this->getPaintStatus()==6){
        QPixmap pixmap = QPixmap(":/new/mbg6").scaled(this->size());
        QPainter painter(this);
        painter.drawPixmap(this->rect(),pixmap);
    }else if(this->getPaintStatus()==7){
        QPixmap pixmap = QPixmap(":/new/mbg7").scaled(this->size());
        QPainter painter(this);
        painter.drawPixmap(this->rect(),pixmap);
    }else if(this->getPaintStatus()==8){
        QPixmap pixmap = QPixmap(":/new/mbg8").scaled(this->size());
        QPainter painter(this);
        painter.drawPixmap(this->rect(),pixmap);
    }else if(this->getPaintStatus()==9){
        QPixmap pixmap = QPixmap(":/new/mbg9").scaled(this->size());
        QPainter painter(this);
        painter.drawPixmap(this->rect(),pixmap);
    }else if(this->getPaintStatus()==10){
        QPixmap pixmap = QPixmap(":/new/mbg10").scaled(this->size());
        QPainter painter(this);
        painter.drawPixmap(this->rect(),pixmap);
    }else {
        this->setPaintStatus(0);
        //设置画笔
        QPen objPen(Qt::NoBrush,1);
        QPainter Painter(this);
        Painter.setPen(objPen);
        //设置渐变画刷
        QLinearGradient objLinear(rect().topLeft(),rect().bottomLeft());
        //顶部颜色和透明度
        objLinear.setColorAt(0,QColor(0,160,234,240));
        //中间颜色和透明度
        objLinear.setColorAt(0.9,QColor(51,122,183,255));
        //底部颜色和透明度
        objLinear.setColorAt(1,QColor(51,122,183,255));
        QBrush objBrush(objLinear);
        Painter.setBrush(objBrush);
        //画圆角矩形
        //Painter.drawRoundedRect(rect(),5,5);
        Painter.drawRect(rect());
    }

}
