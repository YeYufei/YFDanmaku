#include "hideWidget.h"

HideWidget::HideWidget(QWidget *parent) :
    QWidget(parent)
{
}

void HideWidget::paintEvent(QPaintEvent *){
    QPixmap pixmap = QPixmap(":/project/hidebg").scaled(this->size());
    QPainter painter(this);
    painter.drawPixmap(this->rect(), pixmap);
}

