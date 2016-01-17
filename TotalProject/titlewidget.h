#ifndef TITLEWIDGET_H
#define TITLEWIDGET_H

#include <QtGui>
#include <QPoint>
#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QLinearGradient>


class TitleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TitleWidget(QWidget *parent = 0);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *);

};

#endif // TITLEWIDGET_H
