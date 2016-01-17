#ifndef QRENWIDGET_H
#define QRENWIDGET_H
#include <QMouseEvent>
#include <QWidget>
#include <QLabel>
#include <QBitmap>
#include <QPixmap>
#include <QImage>
#include <QRgb>
#include <string.h>
#include <errno.h>
#include <conio.h>
#include <ctype.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <wchar.h>
#include <windows.h>
#include "LibQREncode/qrencode.h"
//	-------------------------------------------------------


//	-------------------------------------------------------
//	DEFines
//	-------------------------------------------------------


#define OUT_FILE_PIXEL_PRESCALER	8											// Prescaler (number of pixels in bmp file for each QRCode pixel, on each dimension)

#define PIXEL_COLOR_R				0											// Color of bmp pixels
#define PIXEL_COLOR_G				0
#define PIXEL_COLOR_B				0

    // BMP defines

typedef unsigned short	WORD;
typedef unsigned long	DWORD;
typedef signed long		LONG;



//	-------------------------------------------------------


//	-------------------------------------------------------
//	Main
//	-------------------------------------------------------

namespace Ui {
class Widget;
}

class QRenWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QRenWidget(QWidget *parent = 0);
    ~QRenWidget();
    void setURL(QString url);
    QString getURL();
    void createQRCode();
    QRect getRect();
    void setRect(QRect rect);

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent *event);
private:
    bool createQRCode(const QString &str, const QString &fileName = "QRCode.bmp");

private slots:
    /*!
     * \brief getQRCode
     * \param str        要转为二维码的串
     * \param fileName   转为二维码生成的图片
     */


private:
    Ui::Widget *ui;
    QLabel * label;
    QString url;
    bool mousePressed;
    bool m_bDataEncoded;
    int m_nSymbleSize;
    QImage *m_pSymbleBitmap;
    QPoint movepoint;
    QRect screenRect;
};

#endif // QRENWIDGET_H
