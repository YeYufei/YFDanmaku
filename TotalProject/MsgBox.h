#ifndef MSGBOX_H
#define MSGBOX_H
#include <QtGui>
#include <QDialog>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QGraphicsDropShadowEffect>


class MsgBox:public QDialog
{

public:
    MsgBox(QWidget *parent =0);
    MsgBox(QWidget *parent,QString title,QString content);
    ~MsgBox();
protected:
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent *event);
private:
    bool mousePressed;
    QGraphicsDropShadowEffect * wndShadow;
    QPoint movepoint;
    QLabel * Iconlabel;
    QLabel * titleLabel;
    QLabel * askLabel;
    QLabel * msgLabel;
    QPushButton * closeButton;
    QPushButton * YesButton;
};

#endif // MSGBOX_H
