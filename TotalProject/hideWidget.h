#ifndef HIDEWIDGET_H
#define HIDEWIDGET_H


#include <QtGui>
#include <QPoint>
#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QLinearGradient>


class HideWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HideWidget(QWidget *parent = 0);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *);

};

#endif // HIDEWIDGET_H
