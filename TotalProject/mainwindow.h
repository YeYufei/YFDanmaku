#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QtGui>
#include <QMainWindow>
#include <QPushButton>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QTextCodec>
//#include <QDebug>
#include <QSpacerItem>
#include <QMouseEvent>
#include <QListWidget>
#include <QLabel>
#include <QListWidgetItem>
#include <QWidget>
#include <QStyle>
#include <QPainterPath>
#include <QDesktopWidget>
#include <QKeyEvent>
#include <iostream>
#include <ctime>
#include <QGraphicsDropShadowEffect>
#include <QCompleter>
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QWaitCondition>
#include <QCheckBox>
#include <QtNetwork>
#include <QMessageBox>
#include <QMovie>
#include <QLineEdit>
#include <QRegExp>
#include <QRegExpValidator>
#include <QScrollBar>
#include <QWheelEvent>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include "Danmu.h"
#include "messageWidget.h"
#include "titlewidget.h"
#include "doge.h"
#include "BanListWidget.h"
#include "hideWidget.h"
#include "MsgBox.h"
#define DEFAULT_HOST "121.42.205.237"
#define DEFAULT_PORT 2000
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);       //构造函数
    ~MainWindow();      //析构函数
    void addDanmu(QString content, QString color = "white", int type = 1,QFont danmuFont = QFont("SimHei",20,100));     //添加弹幕的方法
    void addDoge(QMovie * movie2);  //添加Doge;
    void addDoge(QPixmap * pic,int qType = 0);
    void writeConfigFile();
    void readConfigFile();

private slots:      //槽函数  Qt的重点就是 信号  与 槽函数的关联
    void connectToTCPServer();      //连接到服务器
    void changeDanmuShowStatus();       //改编弹幕状态
    void mousePressEvent(QMouseEvent *event);//三个鼠标时间 按下 移动 释放
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    bool closeWidget();     //关闭软件
    void contextMenuEvent(QContextMenuEvent * event);       //右键菜单
    void paintEvent(QPaintEvent *event);        //重绘事件
    void disconnectTCPServer();     //断开服务器连接
    void connectOrNot();        //是否连接上服务器
    void changeTopDanmuShowState();     //改变顶端弹幕状态
    void changeBottomDanmuShowState();  //改变底端弹幕状态
    void changeFontSize(int size);      //改变字体大小
    void changeTransparency(int transparency);      //改变透明度
    void changeSpeed(int speed);
    void addBanWordList();
    void updateConfig();
    void updateMessageWidget();
    void setCurrentNum();
    void setCurrent2();
    void setCurrent3();
    void setCurrentDoge();
    void wheelEvent(QWheelEvent *);                                   //处理鼠标
    void drawQrenCodePic();
    //连接成功
    void OnSuccConnect();       //连接成功
    //接收数据
    void ReadStructDataAndShow();       //读取socket数据
    void changeInterface();
    void aboutApp();                //关于本软件
    void aboutAuthor();             //关于作者
    void aboutAim();                //关于初衷(笑)

private:
    void initUI();
    Ui::MainWindow *ui;
    QWidget * danmuWidget;                                            //弹幕窗口
    bool danmushow;                                                   //是否显示弹幕
    bool isLeftPressDown;                                             //判断是否左键按下
    bool isconnectToServer;                                           //判断是否连接到了服务器
    bool topdanmushow;
    bool bottomdanmushow;

    QDesktopWidget* desktopWidget;                                    //获取桌面设备
    QRect screenRect;                                                 //一个Qt矩形

    QTimer * mtimer;
    int current2,current3,currentNum,currentDoge,MaxTotal,MaxTop,MaxBottom,fontSize,mstatus,DSpeed;  //一些整形
    float Transparency;

    //一些界面组件
    QCheckBox * showDanmuBut;
    QPushButton * miniBut;
    QPushButton * maxiBut;
    QPushButton * closeBut;
    QCheckBox * pingUp;
    QCheckBox * pingDown;
    QPushButton * connectBut;
    QPushButton * logoBut;
    QPushButton * subMenuBut;

    QLineEdit * hostEdit;
    QLineEdit * portEdit;
    QLineEdit * MaxTotalEdit;
    QLineEdit * MaxTopEdit;
    QLineEdit * MaxBottomEdit;
    QLineEdit * banWordEdit;
    QPushButton * addWordBut;
    QPushButton * updateConfigBut;
    QPushButton * qrencodeBut;

    BanListWidget * banlist;  //敏感词列表

    //一些Qt点记录鼠标的位置的
    QPoint startPos;
    QPoint clickPos;
    QPoint NormalWinPos;

    //一些界面控件
    TitleWidget * titleWidget;
    QWidget * controlWidget;
    HideWidget * hideWidget;
    QWidget * centerWidget;
    QLabel * leftCha;
    QLabel * middleCha;
    QLabel * rightCha;
    MessageWidget * messageWidget;
    QString myFontStr;
    QString myFontStr2;
    QSlider * transparencySlider;                                            //声音滑条
    QSlider * fontSizeSlider;                                                //进度条滑条
    QSlider * speedSlider;
    QLabel * statusLabel;                                                    //状态标签


    //网络模块
    QTcpSocket * myTCPClient;
    QString clientID;
    QLabel * rundoge;
    QString HOST;
    int PORT;
    QFile * configFile;
    bool hiddenShow;
    QPixmap d,e,g;
    QNetworkAccessManager * manager;
    QNetworkRequest req;
};

#endif // MAINWINDOW_H
