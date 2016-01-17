#include "QRenWidget.h"
#include "ui_widget.h"
#include <QFile>
#include <QBitmap>
#include <QDebug>
#include <string.h>
#include <QVBoxLayout>



QRenWidget::QRenWidget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);
    label = new QLabel(this);
    QVBoxLayout * layout = new QVBoxLayout(this);
    layout->addWidget(label);
}

QRenWidget::~QRenWidget()
{
    delete ui;
}

void QRenWidget::createQRCode()
{
    this->label->clear();
    this->label->setScaledContents(true);
    this->label->resize(QSize(500,500));
    const QString tem = url.trimmed();
    qDebug()<<tem<<endl;
    if(tem.isEmpty())
    {
        this->label->setText("不能为空!!!");
        return;
    }
    if(createQRCode(tem,"QRCode.bmp"))
    {
        QBitmap img(QCoreApplication::applicationDirPath()+"/"+"QRCode.bmp");
        this->label->setPixmap(img.scaled(QSize(500,500)));
    }
    else
        this->label->setText("生成二维码失败!!!");
}

bool QRenWidget::createQRCode(const QString & str,const QString & fileName){
    char*			szSourceSring = str.toLocal8Bit().data();
    unsigned int	unWidth, x, y, l, n, unWidthAdjusted, unDataBytes;
    unsigned char*	pRGBData, *pSourceData, *pDestData;
    QRcode*			pQRC;
    FILE*			f;

    QString filePath = QCoreApplication::applicationDirPath()+"//"+fileName;
            // Compute QRCode

        if (pQRC = QRcode_encodeString(szSourceSring, 0, QR_ECLEVEL_H, QR_MODE_8, 1))
            {
            unWidth = pQRC->width;
            unWidthAdjusted = unWidth * OUT_FILE_PIXEL_PRESCALER * 3;
            if (unWidthAdjusted % 4)
                unWidthAdjusted = (unWidthAdjusted / 4 + 1) * 4;
            unDataBytes = unWidthAdjusted * unWidth * OUT_FILE_PIXEL_PRESCALER;

                // Allocate pixels buffer

            if (!(pRGBData = (unsigned char*)malloc(unDataBytes)))
                {
                printf("Out of memory");
                exit(-1);
                }

                // Preset to white

            memset(pRGBData, 0xff, unDataBytes);


                // Prepare bmp headers

            BITMAPFILEHEADER kFileHeader;
            kFileHeader.bfType = 0x4d42;  // "BM"
            kFileHeader.bfSize =	sizeof(BITMAPFILEHEADER) +
                                    sizeof(BITMAPINFOHEADER) +
                                    unDataBytes;
            kFileHeader.bfReserved1 = 0;
            kFileHeader.bfReserved2 = 0;
            kFileHeader.bfOffBits =	sizeof(BITMAPFILEHEADER) +
                                    sizeof(BITMAPINFOHEADER);

            BITMAPINFOHEADER kInfoHeader;
            kInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
            kInfoHeader.biWidth = unWidth * OUT_FILE_PIXEL_PRESCALER;
            kInfoHeader.biHeight = -((int)unWidth * OUT_FILE_PIXEL_PRESCALER);
            kInfoHeader.biPlanes = 1;
            kInfoHeader.biBitCount = 24;
            kInfoHeader.biCompression = BI_RGB;
            kInfoHeader.biSizeImage = 0;
            kInfoHeader.biXPelsPerMeter = 0;
            kInfoHeader.biYPelsPerMeter = 0;
            kInfoHeader.biClrUsed = 0;
            kInfoHeader.biClrImportant = 0;


                // Convert QrCode bits to bmp pixels

            pSourceData = pQRC->data;
            for(y = 0; y < unWidth; y++)
                {
                pDestData = pRGBData + unWidthAdjusted * y * OUT_FILE_PIXEL_PRESCALER;
                for(x = 0; x < unWidth; x++)
                    {
                    if (*pSourceData & 1)
                        {
                        for(l = 0; l < OUT_FILE_PIXEL_PRESCALER; l++)
                            {
                            for(n = 0; n < OUT_FILE_PIXEL_PRESCALER; n++)
                                {
                                *(pDestData +		n * 3 + unWidthAdjusted * l) =	PIXEL_COLOR_B;
                                *(pDestData + 1 +	n * 3 + unWidthAdjusted * l) =	PIXEL_COLOR_G;
                                *(pDestData + 2 +	n * 3 + unWidthAdjusted * l) =	PIXEL_COLOR_R;
                                }
                            }
                        }
                    pDestData += 3 * OUT_FILE_PIXEL_PRESCALER;
                    pSourceData++;
                    }
                }


                // Output the bmp file

            if (!(fopen_s(&f, filePath.toLocal8Bit().data(), "wb")))
                {
                fwrite(&kFileHeader, sizeof(BITMAPFILEHEADER), 1, f);
                fwrite(&kInfoHeader, sizeof(BITMAPINFOHEADER), 1, f);
                fwrite(pRGBData, sizeof(unsigned char), unDataBytes, f);
                fflush(f);
                fclose(f);
                return true;
                }
            else
                {
                printf("Unable to open file");
                    return false;
                }

                // Free data

            free(pRGBData);
            QRcode_free(pQRC);
            }
        else
            {
            printf("NULL returned");
                return false;
            }

        return false;

}

/*bool QRenWidget::createQRCode(const QString &str, const QString &fileName)
{
    int m_nImgWidth   = 0;
    int m_nImgHeight  = 0;
    int m_nImgRowDist = 0;
    void *m_pImgData  = 0;
    char *tem = qstrdup(str.toLocal8Bit().constData());
    int m_nRes = BSS_GenerateQRCode( tem,        //str.toStdString().c_str(),
                                     strlen(tem),//strlen(str.toStdString().c_str()),
                                     3,          //二维码图像大小相当于是(29*size)*(29*size)
                                     4,
                                     &m_pImgData,//m_pImgData就是每个像素点的8位数据
                                     &m_nImgWidth,//m_nImgWidth是图像宽度，这里的结果是29的倍数，导致都不是4的倍数
                                     &m_nImgHeight,//图像高度
                                     &m_nImgRowDist );//所以m_nImgRowDist会比m_nImgWidth大点，但是是4的倍数，这是为了位图存储(windows下位图每行的字节数必须是4的倍数)

    //接下来就把m_pImgData的像素位数据转成CBitmap位图就可以了

    delete [] tem;
    switch( m_nRes )
    {
    case BSSG_QRCODE_OK:
        qDebug() <<  "\n--Create QRCode is OK!!!--";
        break;
    case BSSG_QRCODE_ERR_GENERIC:
        qDebug() << "\nGeneric error code.\nThe library doesn抰 clarify or cannot determine what is happening.";
        break;
    case BSSG_QRCODE_ERR_OUTOFMEMORY:
        qDebug() << "\nRan out of memory.\nSome allocation failed.";
        break;
    case BSSG_QRCODE_ERR_NOTSUPPORTED:
        qDebug() << "\nThe feature requested is not supported.";
        break;
    case BSSG_QRCODE_ERR_INVALIDARGS:
        qDebug() << "\nArgument validation failed.";
        break;
    case BSSG_QRCODE_ERR_OUTOFSYMBOLOGY:
        qDebug() << "\nCannot create the barcode symbol that would be contain this specified input data string" \
                    "by the reason of  some bar code symbology limitation (one or more characters from input " \
                    "data string do not belong  to symbology character set, input data string is too long, etc.)";
        break;
    }
    if(m_nRes != BSSG_QRCODE_OK)
        return false;
    uchar* pImage = static_cast<uchar*>( m_pImgData );
    CBitmapImage Image;
    Image.Attach( pImage, m_nImgWidth, m_nImgHeight, m_nImgRowDist);
    if( Image.Save8bppBitmap(fileName.toStdString()))
    {
        qDebug() << "I/O Error: Unable to open the output file." << endl;
    }
    Image.Detach();
    BSS_ReleaseQRCodeImage( m_pImgData, m_nImgHeight, m_nImgWidth, m_nImgRowDist );
    return true;
}*/


void QRenWidget::setURL(QString url){
    this->url = url;
}

QString QRenWidget::getURL(){
    return url;
}

void QRenWidget::setRect(QRect rect){
    this->screenRect = rect;
}

QRect QRenWidget::getRect(){
    return screenRect;
}

void QRenWidget::mousePressEvent(QMouseEvent * event)
{
    if(event->button()==Qt::LeftButton)
        mousePressed=true;
    movepoint=event->globalPos()-pos();
}

void QRenWidget::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed=false;
}

void QRenWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(mousePressed)
      {
        QPoint move_pos = event->globalPos();
        move(move_pos - movepoint);
      }
}

