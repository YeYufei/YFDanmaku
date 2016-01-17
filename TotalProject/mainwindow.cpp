#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QRenWidget.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);      //利用UI文件建立UI，但其实我的UI文件是空窗口，控件都是自己设置布局，所以就没有改这里的代码。

    //变量初始化
    danmushow=true;
    current2=0;
    current3=0;
    currentNum = 0;
    currentDoge = 0;
    DSpeed = 25000;
    mstatus = 0;
    Transparency = 255.0/255.0;
    fontSize = 20;
    danmushow = true;
    bottomdanmushow = true;
    topdanmushow = true;
    isLeftPressDown = false;
    isconnectToServer = false;
    hiddenShow = false;
    clientID="";
    PORT = 0;
    HOST = "";
    MaxTotal = 0;
    MaxTop = 0;
    MaxBottom = 0;


    int loadedFontID = QFontDatabase::addApplicationFont(":/project/dingding.fon");     //加载字体到字体数据库，字体文件放在资源文件中
    QStringList loadedFontFamilies = QFontDatabase::applicationFontFamilies(loadedFontID);      //在字体数据库中找我们的这个字体，找到就调用。
    if(!loadedFontFamilies.empty())
       myFontStr = loadedFontFamilies.at(0);

    setWindowTitle("DanmakuSystem");    //设置窗口标题
    //设置这个,去系统自带边框
    setWindowFlags( Qt::FramelessWindowHint|Qt::WindowSystemMenuHint /*|Qt::WindowStaysOnTopHint*/| Qt::WindowMinimizeButtonHint);
    //设置自动填充背景为真
    setAutoFillBackground(true);

    //设置窗体最小规格
    setMinimumSize(900,600);

    //设置为鼠标按下,方便追踪鼠标事件
    this->setMouseTracking(true);
    //设置为获得焦点状态
    this->setFocus();

    desktopWidget = QApplication::desktop();                   //获取桌面设备
    screenRect = desktopWidget->screenGeometry();              //获取桌面大小的矩形

    //生成随机数用
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    mtimer = new QTimer(this);
    connect(mtimer,SIGNAL(timeout()),SLOT(updateMessageWidget()));
    manager = new QNetworkAccessManager;


    move(200,50);       //窗口左上角为(0,0)，这里让程序移动到一定位置

    mtimer->start(60000);
    this->centralWidget()->setMouseTracking(true);      //这里设置为true是为了让中心窗口自动监听鼠标事件，若不设置true 则只有鼠标在centralWidget按住不松手才处理鼠标事件
    initUI();       //重点:主界面布局代码

    readConfigFile();

    if(HOST==""||PORT==0){
        HOST=DEFAULT_HOST;
        PORT=DEFAULT_PORT;
    }

    if(PORT<1024||PORT>5000){
        PORT=DEFAULT_PORT;
    }

    if(MaxTotal == 0){
        MaxTotal = 500;
        MaxBottom = 10;
        MaxTop = 10;
    }

    //qDebug()<<HOST<<","<<PORT<<endl;

    hostEdit->setText(HOST);
    portEdit->setText(QString("%1").arg(PORT));
    MaxTotalEdit->setText(QString("%1").arg(MaxTotal));
    MaxTopEdit->setText(QString("%1").arg(MaxTop));
    MaxBottomEdit->setText(QString("%1").arg(MaxBottom));
    //下面这个函数是为主窗口设置stylesheet  stylesheet是用QSS写的 是Qt自定义的Css 作用很神奇 具体可以搜索引擎搜索。
    this->setStyleSheet("QMainWindow,QDialog{"
                        "background:white;"
                        "border:1px solid gray;"
                "}");

    //设置完样式了，重绘桌面
    this->repaint();

    //创建客户端
    myTCPClient = new QTcpSocket();     //Qt的Socket连接
    //关联信号
    connect(myTCPClient, SIGNAL(connected()), this, SLOT(OnSuccConnect()));
    connect(myTCPClient, SIGNAL(readyRead()), this, SLOT(ReadStructDataAndShow()));
    connect(myTCPClient,SIGNAL(disconnected()),this,SLOT(disconnectTCPServer()));
    connect(addWordBut,SIGNAL(clicked()),this,SLOT(addBanWordList()));
    connect(updateConfigBut,SIGNAL(clicked()),this,SLOT(updateConfig()));
}

void MainWindow::initUI(){

    /*Qt 最琐碎的地方就在这里
     *
     * 这里面的都是主界面的布局代码
     *
     * 在这里为了思路清晰我只用了水平布局与垂直布局
     *
     * 主界面是垂直布局  为三层  titleWidget(标题栏)  MessageWidget(蓝色背景展示层)  centrolWidget(内含控制层)
     *
     * 内部控件主要是水平布局，也有垂直布局嵌套。
     *
     * 布局之间有嵌套，不过我相信你能看懂
     *
     */

    subMenuBut = new QPushButton(this);
    subMenuBut->setFixedSize(29,32);
    subMenuBut->setStyleSheet("QPushButton{border-image:url(:/project/subtitle);}"
                               "QPushButton:hover{border-image: url(:/project/subtitle1);}"
                               "QPushButton:pressed{border-image: url(:/project/subtitle2);}");
    connect(subMenuBut,SIGNAL(clicked()),this,SLOT(changeInterface()));

    //新建一个最小化按钮  设置为固定大小  设置样式  注意这里的样式 用到了三张图  这样就做好了一个简单的三态按钮。 之后连接信号与槽函数
    miniBut = new QPushButton(this);
    //miniBut->setText("最小化");
    miniBut->setFixedSize(29,32);
    miniBut->setStyleSheet("QPushButton{border-image:url(:/project/min1);}"
                           "QPushButton:hover{border-image: url(:/project/min2);}"
                           "QPushButton:pressed{border-image: url(:/project/min3);}");
    QObject::connect(miniBut,SIGNAL(clicked()),this,SLOT(showMinimized()));

    //新建一个关闭按钮  设置为固定大小  设置样式 之后连接信号与槽函数
    closeBut = new QPushButton(this);
    //closeBut->setText("关闭");
    closeBut->setFixedSize(38,32);
    QObject::connect(closeBut,SIGNAL(clicked()),this,SLOT(closeWidget()));
    closeBut->setStyleSheet("QPushButton{border-image: url(:/project/close1);}"
                           "QPushButton:hover{border-image: url(:/project/close2);}"
                           "QPushButton:pressed{border-image: url(:/project/close3);}");

    //新建一个标志按钮  设置为固定大小  这个标志按钮 没有连接槽函数，所以点击是没用的，我图的就是三个状态(*＾-＾*)
    logoBut = new QPushButton(this);
    logoBut->setFixedSize(376, 46);
    logoBut->setStyleSheet("QPushButton{border-image: url(:/project/danmuLogo1);}"
                           "QPushButton:hover{border-image: url(:/project/danmuLogo);}"
                           "QPushButton:pressed{border-image: url(:/project/danmuLogo1);}");

    //新建QCheckBox控件 隐藏弹幕用 第一个参数是它要显示的信息，第二个参数是传递父窗口指针给它。 设置字体 然后连接它的click信号与相应的槽函数。
    showDanmuBut = new QCheckBox("隐藏弹幕",this);
    showDanmuBut->setFont(QFont(myFontStr,20,1));
    //showDanmuBut->setText("隐藏弹幕");
    QObject::connect(showDanmuBut, SIGNAL(clicked()), this, SLOT(changeDanmuShowStatus()));

    //新建QCheckBox控件  屏蔽顶端用 设置字体 然后连接它的click信号与相应的槽函数。
    pingUp = new QCheckBox("屏蔽顶端",this);
    pingUp->setFont(QFont(myFontStr,20,1));
    //pingUp->setText("屏蔽顶端");
    QObject::connect(pingUp,SIGNAL(clicked()),this,SLOT(changeTopDanmuShowState()));

    //新建QCheckBox控件  屏蔽底部用 设置字体 然后连接它的click信号与相应的槽函数。
    pingDown = new QCheckBox("屏蔽底部",this);
    pingDown->setFont(QFont(myFontStr,20,1));
    //pingDown->setText("屏蔽底部");
    QObject::connect(pingDown,SIGNAL(clicked()),this,SLOT(changeBottomDanmuShowState()));

    //新建一个连接服务器按钮  设置为固定大小  设置字体 之后连接信号与槽函数
    connectBut = new QPushButton(this);
    connectBut->setText("连接服务器");
    connectBut->setFont(QFont(myFontStr,20,1));
    connectBut->setFixedSize(200,80);
    //connect(connectBut,SIGNAL(clicked()),this,SLOT(connectOrNot()));
    connect(connectBut,SIGNAL(clicked()),this,SLOT(connectToTCPServer()));

    //这里是Qt画图 是 展示层用的
    d.load(":/project/meiyouDanmu");
    e.load(":/project/bafan1");
    g.load(":/project/dadanmu1");
    leftCha = new QLabel(this);         //用Label绘图 一个左边(果然。。。)  一个右边(大弹幕时代。。。)
    leftCha->setPixmap(d);
    leftCha->setFixedSize(d.size());
    middleCha = new QLabel(this);
    middleCha->setPixmap(e);
    middleCha->setFixedSize(e.size());
    middleCha->hide();
    rightCha = new QLabel(this);
    rightCha->setPixmap(g);
    rightCha->setFixedSize(g.size());

    //这个时候创建四个Widget
    titleWidget = new TitleWidget(this);
    controlWidget = new QWidget(this);
    centerWidget = new QWidget(this);
    messageWidget = new MessageWidget(this);
    hideWidget = new HideWidget(this);
    messageWidget->setFixedHeight(350);


    QPalette pa;        //Qt的调色盘类，这里用来设置背景颜色用
    pa.setColor(QPalette::Background,QColor(34,34,34));
    QPalette pa2;
    pa2.setColor(QPalette::Background,QColor(51,122,183));


    titleWidget->setFixedHeight(50);
    titleWidget->setCursor(Qt::ArrowCursor);        //设置鼠标样式
    QHBoxLayout * titleLayout = new QHBoxLayout();      //新建了一个QHBoxLayout水平布局，这个水平布局是用来放Logo按钮  最小化按钮 还有关闭按钮的~
    titleLayout->addSpacing(5);     //加个间隔，不让logo按钮太贴边
    titleLayout->addWidget(logoBut);        //加入logo按钮
    titleLayout->addStretch(0);       //加个弹簧，弹簧默认把控件往两端挤
    titleLayout->addWidget(subMenuBut);
    titleLayout->addWidget(miniBut);    //加最小化
    titleLayout->addWidget(closeBut);   //加关闭按钮
    titleLayout->setSpacing(0);      //设置水平布局默认间隔为0  默认不为0 会有点难看
    titleLayout->setMargin(2);      //设置控件间隔
    titleWidget->setLayout(titleLayout);        //把这个水平布局放入titleWidget中


    QVBoxLayout * controlLayout = new QVBoxLayout();        //新建了一个QVBoxLayout垂直布局  就是放那三个勾选框用的  相信加上上面的例子  你已经知道水平布局和垂直布局是什么作用的了。
    controlLayout->addWidget(showDanmuBut);     //加隐藏弹幕勾选框
    controlLayout->addWidget(pingUp);       //加屏蔽顶端勾选框
    controlLayout->addWidget(pingDown);     //加屏蔽底端勾选框

    QVBoxLayout * controlLayout2 = new QVBoxLayout();       //新建一个垂直布局  用来放那两个进度条 改变透明度和字体大小用的。

    transparencySlider = new QSlider(Qt::Horizontal);       //Horizontal参数表明滚动条是水平的
    transparencySlider->setStyleSheet(
                "QSlider::groove:horizontal {                                "
                "     border: 1px   solid #999999;                           "
                "     height: 1px;                                           "
                "     margin: 0px 0py;                                       "
                "     left: 9px; right: 9px;                                 "
                " }                                                          "
                "QSlider::handle:horizontal {                                "
                "     border: 1px solid #5c5c5c;                             "
                " border-image:url(:/project/whiteHandle);                   "
                "     width: 20px;                                           "
                "     margin: -11px -11px -11px -11px;                       "
                " }                                                          "
                "QSlider::handle:hover:horizontal{                           "
                "     border: 1px solid #5c5c5c;                             "
                " border-image:url(:/project/whiteHandle);                   "
                "     width: 20px;                                           "
                "     margin: -11px -11px -11px -11px;                       "
                "}                                                           "
                "QSlider::handle:pressed:horizontal{                         "
                "     border: 1px solid #5c5c5c;                             "
                " border-image:url(:/project/whiteHandle);                   "
                "     width: 20px;                                           "
                "     margin: -11px -11px -11px -11px;                       "
                "}                                                           "
                "                                                            "
                "QSlider::sub-page:horizontal{                               "
                " background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(11, 119, 225, 255), stop:0.65 rgba(11,119, 225, 255), stop:1 rgba(92, 184, 254, 255));                      "
                "}                                                           "
                "QSlider::add-page:horizontal{                               "
                " background: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:1, stop:0 rgba(201,201,201, 255));  "
                "}"
                );
    transparencySlider->setRange(0,255);        //设置数值范围
    transparencySlider->setFixedWidth(150);     //最长长度
    transparencySlider->setValue(255);      //设置现在的数值
    connect(transparencySlider,SIGNAL(valueChanged(int)),this,SLOT(changeTransparency(int)));       //一旦进度条的值有变就执行changeTransparency函数  同时 信号valueChanged(int) 这里有int参数  会把进度条的值传出去 传到你自己写的changeTransparency函数中


    fontSizeSlider = new QSlider(Qt::Horizontal);
    fontSizeSlider->setStyleSheet(
                "QSlider::groove:horizontal {                                "
                "     border: 1px   solid #999999;                           "
                "     height: 1px;                                           "
                "     margin: 0px 0py;                                       "
                "     left: 9px; right: 9px;                                 "
                " }                                                          "
                "QSlider::handle:horizontal {                                "
                "     border: 1px solid #5c5c5c;                             "
                " border-image:url(:/project/whiteHandle);                   "
                "     width: 20px;                                           "
                "     margin: -11px -11px -11px -11px;                       "
                " }                                                          "
                "QSlider::handle:hover:horizontal{                           "
                "     border: 1px solid #5c5c5c;                             "
                " border-image:url(:/project/whiteHandle);                   "
                "     width: 20px;                                           "
                "     margin: -11px -11px -11px -11px;                       "
                "}                                                           "
                "QSlider::handle:pressed:horizontal{                         "
                "     border: 1px solid #5c5c5c;                             "
                " border-image:url(:/project/whiteHandle);                   "
                "     width: 20px;                                           "
                "     margin: -11px -11px -11px -11px;                       "
                "}                                                           "
                "                                                            "
                "QSlider::sub-page:horizontal{                               "
                " background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(11, 119, 225, 255), stop:0.65 rgba(11,119, 225, 255), stop:1 rgba(92, 184, 254, 255));                      "
                "}                                                           "
                "QSlider::add-page:horizontal{                               "
                " background: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:1, stop:0 rgba(201,201,201, 255));  "
                "}"
                );
    fontSizeSlider->setRange(0,100);
    fontSizeSlider->setFixedWidth(150);
    fontSizeSlider->setValue(20);
    connect(fontSizeSlider,SIGNAL(valueChanged(int)),this,SLOT(changeFontSize(int)));

    speedSlider = new QSlider(Qt::Horizontal);       //Horizontal参数表明滚动条是水平的
    speedSlider->setStyleSheet(
                "QSlider::groove:horizontal {                                "
                "     border: 1px   solid #999999;                           "
                "     height: 1px;                                           "
                "     margin: 0px 0py;                                       "
                "     left: 9px; right: 9px;                                 "
                " }                                                          "
                "QSlider::handle:horizontal {                                "
                "     border: 1px solid #5c5c5c;                             "
                " border-image:url(:/project/whiteHandle);                   "
                "     width: 20px;                                           "
                "     margin: -11px -11px -11px -11px;                       "
                " }                                                          "
                "QSlider::handle:hover:horizontal{                           "
                "     border: 1px solid #5c5c5c;                             "
                " border-image:url(:/project/whiteHandle);                   "
                "     width: 20px;                                           "
                "     margin: -11px -11px -11px -11px;                       "
                "}                                                           "
                "QSlider::handle:pressed:horizontal{                         "
                "     border: 1px solid #5c5c5c;                             "
                " border-image:url(:/project/whiteHandle);                   "
                "     width: 20px;                                           "
                "     margin: -11px -11px -11px -11px;                       "
                "}                                                           "
                "                                                            "
                "QSlider::sub-page:horizontal{                               "
                " background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(11, 119, 225, 255), stop:0.65 rgba(11,119, 225, 255), stop:1 rgba(92, 184, 254, 255));                      "
                "}                                                           "
                "QSlider::add-page:horizontal{                               "
                " background: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:1, stop:0 rgba(201,201,201, 255));  "
                "}"
                );
    speedSlider->setRange(1000,60000);        //设置数值范围
    speedSlider->setFixedWidth(150);     //最长长度
    speedSlider->setValue(25000);      //设置现在的数值
    connect(speedSlider,SIGNAL(valueChanged(int)),this,SLOT(changeSpeed(int)));

    //这里有两个水平布局layout1与layout2 都是为了在进度条两端加"大"字 和 "小"字的
    QHBoxLayout * layout1 = new QHBoxLayout();
    QLabel * label1 = new QLabel("全透明",this);
    //label1->setFont(QFont(myFontStr,12,1));
    QLabel * label2 = new QLabel("不透明",this);
    //label2->setFont(QFont(myFontStr,12,1));
    layout1->setSpacing(0);
    layout1->setMargin(0);
    layout1->addWidget(label1);
    layout1->addWidget(transparencySlider);
    layout1->addWidget(label2);
    //controlLayout2->addWidget(transparencySlider);
    QLabel * label3 = new QLabel("弹幕透明度",this);     //加了提示字
    label3->setFont(QFont(myFontStr,20,1));



    QHBoxLayout * layout2 = new QHBoxLayout();
    QLabel * label4 = new QLabel("最小值",this);
    //label1->setFont(QFont(myFontStr,12,1));
    QLabel * label5 = new QLabel("最大值",this);
    //label2->setFont(QFont(myFontStr,12,1));
    layout2->setSpacing(0);
    layout2->setMargin(0);
    layout2->addWidget(label4);
    layout2->addWidget(fontSizeSlider);
    layout2->addWidget(label5);
    //controlLayout2->addWidget(transparencySlider);
    QLabel * label6 = new QLabel("霸气程度",this);          //同样是提示字
    label6->setFont(QFont(myFontStr,20,1));

    //这里有两个水平布局layout1与layout2 都是为了在进度条两端加"大"字 和 "小"字的
    QHBoxLayout * speedlayout = new QHBoxLayout();
    QLabel * speedlabel1 = new QLabel("非常慢",this);
    //label1->setFont(QFont(myFontStr,12,1));
    QLabel * speedlabel2 = new QLabel("超级快",this);
    //label2->setFont(QFont(myFontStr,12,1));
    speedlayout->setSpacing(0);
    speedlayout->setMargin(0);
    speedlayout->addWidget(speedlabel1);
    speedlayout->addWidget(speedSlider);
    speedlayout->addWidget(speedlabel2);
    //controlLayout2->addWidget(transparencySlider);
    QLabel * speedlabel3 = new QLabel("弹幕速度",this);     //加了提示字
    speedlabel3->setFont(QFont(myFontStr,20,1));

    controlLayout2->addWidget(label3);
    controlLayout2->setMargin(0);
    controlLayout2->setSpacing(0);
    controlLayout2->addLayout(layout1);
    controlLayout2->addWidget(label6);
    controlLayout2->addLayout(layout2);
    controlLayout2->addWidget(speedlabel3);
    controlLayout2->addLayout(speedlayout);


    QHBoxLayout * controlLayout3 = new QHBoxLayout();
    controlLayout3->addLayout(controlLayout);
    controlLayout3->addSpacing(30);
    controlLayout3->addLayout(controlLayout2);
    controlWidget->setLayout(controlLayout3);       //控制窗口加上相应布局

    QLabel * fenge = new QLabel(this);
    QPixmap f;f.load(":/project/fenge");        //加了条分隔线
    fenge->setPixmap(f);
    fenge->setFixedSize(f.size());

    statusLabel = new QLabel("当前状态:未连接",this);
    statusLabel->setFixedWidth(285);
    statusLabel->setFont(QFont(myFontStr,20,1));

    qrencodeBut = new QPushButton(this);
    qrencodeBut->setFixedSize(50,50);
    qrencodeBut->setStyleSheet("QPushButton{border-image: url(:/new/erwei3);}"
                               "QPushButton:hover{border-image: url(:/new/erwei3);}"
                               "QPushButton:pressed{border-image: url(:/new/erwei3);}");
    connect(qrencodeBut,SIGNAL(clicked()),this,SLOT(drawQrenCodePic()));
    qrencodeBut->setEnabled(false);

    QHBoxLayout * qrenLayout = new QHBoxLayout();
    qrenLayout->setSpacing(0);
    qrenLayout->addWidget(statusLabel);
    qrenLayout->addWidget(qrencodeBut);
    QVBoxLayout * layout3 = new QVBoxLayout();
    layout3->addStretch();
    layout3->addWidget(connectBut);
    layout3->addSpacing(20);
    layout3->addLayout(qrenLayout);
    //layout3->addWidget(statusLabel);
    layout3->addStretch();

    QHBoxLayout * centerLayout = new QHBoxLayout();
    centerLayout->addStretch(0);
    //centerLayout->addWidget(connectBut);
    centerLayout->addLayout(layout3);
    centerLayout->addStretch();
    centerLayout->addWidget(fenge);
    centerLayout->addStretch(0);
    centerLayout->addWidget(controlWidget);
    centerLayout->addStretch(0);
    centerWidget->setLayout(centerLayout);      //最终把总布局放入相应的层
    centerWidget->setMaximumHeight(200);


    QHBoxLayout * messageLayout = new QHBoxLayout;
    messageLayout->setSpacing(0);
    messageLayout->addWidget(leftCha);
    messageLayout->addWidget(middleCha);
    messageLayout->addStretch();
    messageLayout->addWidget(rightCha);
    messageWidget->setLayout(messageLayout);        //最终把总布局放入相应的层

    QLabel * hostLabel = new QLabel("域名:端口",this);
    hostLabel->setFont(QFont("Microsoft Yahei",15,1));
    hostEdit = new QLineEdit(this);
    hostEdit->setStyleSheet("QLineEdit{font: bold  large \"Times New Roman\";font-size:18px;color:rgb(0,0,0);height:40px;border:4px solid rgb(155,200,33);border-radius:2px;selection-color:pink;background-color: rgb(255,255, 255);}"
                            "QLineEdit:hover{ border: 2px groove rgb(26,58,93);color:rgb(0,0,0);  background-color: rgb(255,255, 255); }"
                            "QLineEdit:!hover{border: 2px groove rgb(36,37, 39);color:rgb(0,0,0);  background-color: rgb(255,255, 255);}"
                            "QLineEdit:focus {border: 2px groove rgb(0,0,255);color:rgb(0,0,255);  background-color: rgb(255,255, 255););}"
                            );
    hostEdit->setPlaceholderText("请输入域名");
    QRegExp ipRx("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-4]|[01]?\\d\\d?)");
    QRegExpValidator *pIpValidator = new QRegExpValidator(ipRx,this);  //^[1-9][0-9]*$ 和 ^[1-9]{1}[/d]*$
    hostEdit->setValidator(pIpValidator);
    QLabel * portLabel = new QLabel(":",this);
    portLabel->setFont(QFont("Microsoft Yahei",15,1));
    portEdit = new QLineEdit(this);
    portEdit->setValidator(new QIntValidator(1024, 65535, this));
    portEdit->setPlaceholderText("请输入端口号");
    portEdit->setStyleSheet("QLineEdit{font: bold  large \"Times New Roman\";font-size:18px;color:rgb(0,0,0);height:40px;border:4px solid rgb(155,200,33);border-radius:2px;selection-color:pink;background-color: rgb(255,255, 255);}"
                            "QLineEdit:hover{ border: 2px groove rgb(26,58,93);color:rgb(0,0,0);  background-color: rgb(255,255, 255); }"
                            "QLineEdit:!hover{border: 2px groove rgb(36,37, 39);color:rgb(0,0,0);  background-color: rgb(255,255, 255);}"
                            "QLineEdit:focus {border: 2px groove rgb(0,0,255);color:rgb(0,0,255);  background-color: rgb(255,255, 255););}"
                            );
    QLabel * MaxTotalLabel = new QLabel("最大同屏弹幕数",this);
    MaxTotalLabel->setFont(QFont("Microsoft Yahei",15,1));
    MaxTotalEdit = new QLineEdit(this);
    MaxTotalEdit->setValidator(new QIntValidator(20,500, this));
    MaxTotalEdit->setPlaceholderText("20-500");
    MaxTotalEdit->setStyleSheet("QLineEdit{font: bold  large \"Times New Roman\";font-size:18px;color:rgb(0,0,0);height:40px;border:4px solid rgb(155,200,33);border-radius:2px;selection-color:pink;background-color: rgb(255,255, 255);}"
                                "QLineEdit:hover{ border: 2px groove rgb(26,58,93);color:rgb(0,0,0);  background-color: rgb(255,255, 255); }"
                                "QLineEdit:!hover{border: 2px groove rgb(36,37, 39);color:rgb(0,0,0);  background-color: rgb(255,255, 255);}"
                                "QLineEdit:focus {border: 2px groove rgb(0,0,255);color:rgb(0,0,255);  background-color: rgb(255,255, 255););}"
                                );
    QLabel * MaxTopLabel = new QLabel("最大顶端弹幕数",this);
    MaxTopLabel->setFont(QFont("Microsoft Yahei",15,1));
    MaxTopEdit = new QLineEdit(this);
    MaxTopEdit->setValidator(new QIntValidator(0, 20, this));
    MaxTopEdit->setPlaceholderText("0-20");
    MaxTopEdit->setStyleSheet("QLineEdit{font: bold  large \"Times New Roman\";font-size:18px;color:rgb(0,0,0);height:40px;border:4px solid rgb(155,200,33);border-radius:2px;selection-color:pink;background-color: rgb(255,255, 255);}"
                              "QLineEdit:hover{ border: 2px groove rgb(26,58,93);color:rgb(0,0,0);  background-color: rgb(255,255, 255); }"
                              "QLineEdit:!hover{border: 2px groove rgb(36,37, 39);color:rgb(0,0,0);  background-color: rgb(255,255, 255);}"
                              "QLineEdit:focus {border: 2px groove rgb(0,0,255);color:rgb(0,0,255);  background-color: rgb(255,255, 255););}"
                              );
    QLabel * MaxBottomLabel = new QLabel("最大底部弹幕数",this);
    MaxBottomLabel->setFont(QFont("Microsoft Yahei",15,1));
    MaxBottomEdit = new QLineEdit(this);
    MaxBottomEdit->setValidator(new QIntValidator(0, 20, this));
    MaxBottomEdit->setPlaceholderText("0-20");
    MaxBottomEdit->setStyleSheet("QLineEdit{font: bold  large \"Times New Roman\";font-size:18px;color:rgb(0,0,0);height:40px;border:4px solid rgb(155,200,33);border-radius:2px;selection-color:pink;background-color: rgb(255,255, 255);}"
                                 "QLineEdit:hover{ border: 2px groove rgb(26,58,93);color:rgb(0,0,0);  background-color: rgb(255,255, 255); }"
                                 "QLineEdit:!hover{border: 2px groove rgb(36,37, 39);color:rgb(0,0,0);  background-color: rgb(255,255, 255);}"
                                 "QLineEdit:focus {border: 2px groove rgb(0,0,255);color:rgb(0,0,255);  background-color: rgb(255,255, 255););}"
                                 );
    banlist = new BanListWidget(this);
    banlist->setFocusPolicy(Qt::NoFocus);
    banlist->setFixedHeight(185);
    banlist->setStyleSheet("QToolTip {border: 2px solid rgb(43,43,45);color:white;background-color: rgb(27,27,28);padding: 5px; border-radius: 3px;opacity: 200;}"
                                       "QListWidget{border: 4px groove rgb(43,43,43); border-radius:4px;font: 75 15pt ;color: rgb(0, 0, 0);background-color: rgb(255, 255, 255,255);}"
                                       "QListWidget::item{font: 75 20pt ;color: rgb(7, 7, 7);background-color: rgb(255, 255, 255,255);}"
                                       "QListWidget::item:selected{font: 75 20pt ;color: rgb(255, 255, 255);background-color: rgb(11,119,225);}"
                                       "QListWidget::item:hover:!selected {font: 75 20pt;color:rgb(255,255,255);background-color:rgb(90,182,253);}"
                                       "QListWidget::item:hover:selected {font: 75 20pt;color:rgb(255, 255, 255);background-color:rgb(90,182,253);}"
                                       "QListWidget::scrollbar {background-color:rgb(27,27,28);}" );
    //列表的滑条样式美化样式表
    banlist->verticalScrollBar()->setStyleSheet("QScrollBar:vertical {"
                                                "background: transparent;"
                                                    "width:  10px;"
                                                    "margin: 22px 0px 22px 0px;"
                                                "}"

                                               " QScrollBar::handle:vertical {"
                                                   " border: 0px solid #3d7bad;"
                                                    "background-color:rgb(27,27,28);"
                                                    "border-radius: 3px;"
                                                    "min-height: 5px;"
                                                    "margin: 0px -1px 0 -0.5px;"
                                                "}"

                                                "QScrollBar::handle:hover{"
                                                "background: rgb(47, 47, 47);"
                                                "min-height: 20px;"
                                                "margin: 0px -1px 0px -0.5px;"
                                                "border-radius: 3px;"
                                                "border: none;"
                                                   "}"
                                                "QScrollBar::handle:pressed{"
                                                      "background-color:rgb(47,47,47);"
                                                   "}"
                                                "QScrollBar::add-line:vertical {"
                                                "subcontrol-origin: margin;"
                                                " border: 0px solid green;"
                                                " height: 20px;"
                                                    "subcontrol-position: bottom;"
                                                  "  subcontrol-origin: margin;"
                                               "}"

                                               " QScrollBar::sub-line:vertical {"
                                                "subcontrol-origin: margin;"
                                                " border: 0px solid red;"
                                                " height: 20px;"
                                                    "subcontrol-position: top;"
                                                  "  subcontrol-origin: margin;"
                                                "}"
                                            "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {"
                                                "background: none;"
                                            "};");
    QLabel * banWordLabel = new QLabel("屏蔽词汇:",this);
    banWordLabel->setFont(QFont("Microsoft Yahei",15,1));
    banWordEdit = new QLineEdit(this);
    banWordEdit->setStyleSheet("QLineEdit{font: bold  large \"Times New Roman\";font-size:18px;color:rgb(0,0,0);height:40px;border:4px solid rgb(155,200,33);border-radius:2px;selection-color:pink;background-color: rgb(255,255, 255);}"
                              "QLineEdit:hover{ border: 2px groove rgb(26,58,93);color:rgb(0,0,0);  background-color: rgb(255,255, 255); }"
                              "QLineEdit:!hover{border: 2px groove rgb(36,37, 39);color:rgb(0,0,0);  background-color: rgb(255,255, 255);}"
                              "QLineEdit:focus {border: 2px groove rgb(0,0,255);color:rgb(0,0,255);  background-color: rgb(255,255, 255););}"
                              );
    addWordBut = new QPushButton("添加",this);
    QHBoxLayout * banWordLayout = new QHBoxLayout;
    banWordLayout->addWidget(banWordLabel);
    banWordLayout->addWidget(banWordEdit);
    banWordLayout->addWidget(addWordBut);

    updateConfigBut = new QPushButton("应用",this);
    QHBoxLayout * adjustConfig = new QHBoxLayout;
    adjustConfig->addStretch();
    adjustConfig->addWidget(updateConfigBut);

    QVBoxLayout * hideWidgetLayout3 = new QVBoxLayout;
    hideWidgetLayout3->addLayout(banWordLayout);
    hideWidgetLayout3->addWidget(banlist);
    hideWidgetLayout3->addLayout(adjustConfig);

    QHBoxLayout * hostLayout = new QHBoxLayout;
    hostLayout->addWidget(hostLabel);
    hostLayout->addWidget(hostEdit);
    hostLayout->addWidget(portLabel);
    hostLayout->addWidget(portEdit);

    QHBoxLayout * MaxTotalLayout = new QHBoxLayout;
    MaxTotalLayout->addWidget(MaxTotalLabel);
    MaxTotalLayout->addWidget(MaxTotalEdit);

    QHBoxLayout * MaxTopLayout = new QHBoxLayout;
    MaxTopLayout->addWidget(MaxTopLabel);
    MaxTopLayout->addWidget(MaxTopEdit);

    QHBoxLayout * MaxBottomLayout = new QHBoxLayout;
    MaxBottomLayout->addWidget(MaxBottomLabel);
    MaxBottomLayout->addWidget(MaxBottomEdit);

    QVBoxLayout * hideWidgetLayout1 = new QVBoxLayout;
    hideWidgetLayout1->addLayout(hostLayout);
    hideWidgetLayout1->addLayout(MaxTotalLayout);
    hideWidgetLayout1->addLayout(MaxTopLayout);
    hideWidgetLayout1->addLayout(MaxBottomLayout);

    QHBoxLayout * hideWidgetLayout2 = new QHBoxLayout;
    hideWidgetLayout2->addSpacing(50);
    hideWidgetLayout2->addLayout(hideWidgetLayout1);
    hideWidgetLayout2->addLayout(hideWidgetLayout3);
    hideWidgetLayout2->addSpacing(50);

    hideWidget->setLayout(hideWidgetLayout2);

    QVBoxLayout * mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    //mainLayout->addSpacing(2);
    mainLayout->addWidget(titleWidget);
    mainLayout->addWidget(messageWidget);
    //mainLayout->addStretch(1);
    mainLayout->setMargin(0);
    //mainLayout->addWidget(showDanmuBut);
    mainLayout->addWidget(centerWidget);
    hideWidget->hide();
    mainLayout->addWidget(hideWidget);
    //mainLayout->addStretch(1);
    //mainLayout->addWidget(controlWidget);

    this->centralWidget()->setLayout(mainLayout);       //最终把总布局放入相应的层，界面布局完成
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addDanmu(QString content,QString color,int type,QFont danmuFont){      //添加弹幕的方法，根据类型，除横飞外，其他要做坐标调整

        if(this->currentNum>MaxTotal){
            return ;
        }
        if(!danmushow)
            return ;
        //qDebug()<<content<<","<<color<<endl;
        screenRect = desktopWidget->screenGeometry();
        Danmu * fdanmu=new Danmu(NULL,content,color,type,screenRect,danmuFont,this->Transparency,this->DSpeed);//新建一个label
        QObject::connect(showDanmuBut, SIGNAL(clicked()), fdanmu, SLOT(changeDanmuShow()));
        if(fdanmu->getType()==2){
            if(current2>MaxTop||topdanmushow==false){
                delete fdanmu;
                fdanmu = NULL;
                return;
            }
            int x = (screenRect.width()-fdanmu->width())/2;
            int y = current2*(fdanmu->height())%(screenRect.height()-(fdanmu->height()));
            fdanmu->move(x,y);
            fdanmu->getanimation()->setStartValue(QPoint(x,y));
            fdanmu->getanimation()->setEndValue(QPoint(x,y));
            fdanmu->getanimation()->start();
            fdanmu->show();
            connect(fdanmu->getanimation(),SIGNAL(finished()),this,SLOT(setCurrent2()));
            connect(this->pingUp,SIGNAL(clicked()),fdanmu,SLOT(changeTopShow()));
            current2++;
            //qDebug()<<x<<","<<y<<endl;
            //DanmuList.append(fdanmu);//追加进列表中
            return ;
        }else if(fdanmu->getType()==3){
            if(current3>MaxBottom||bottomdanmushow==false){
                delete fdanmu;
                fdanmu = NULL;
                return;
            }
            int x = (screenRect.width()-fdanmu->width())/2;
            int y = (screenRect.height()-(current3+1)*(fdanmu->height()))%(screenRect.height());
            fdanmu->move(x,y);
            fdanmu->getanimation()->setStartValue(QPoint(x,y));
            fdanmu->getanimation()->setEndValue(QPoint(x,y));
            fdanmu->getanimation()->start();
            fdanmu->show();
            connect(fdanmu->getanimation(),SIGNAL(finished()),this,SLOT(setCurrent3()));
            connect(this->pingDown,SIGNAL(clicked()),fdanmu,SLOT(changeBottomShow()));
            current3++;
            //DanmuList.append(fdanmu);//追加进列表中
            return ;
        }else{
            connect(fdanmu->getanimation(),SIGNAL(finished()),this,SLOT(setCurrentNum()));
        }
        currentNum++;
}

void MainWindow::addDoge(QPixmap * pic,int qType){
    if(currentDoge>20){
        return ;
    }
    screenRect = desktopWidget->screenGeometry();
    Doge * fdoge=new Doge(NULL,pic,screenRect,4,this->Transparency,this->DSpeed,qType);//新建一个label
    QObject::connect(showDanmuBut, SIGNAL(clicked()), fdoge, SLOT(changeDogeShow()));
    connect(fdoge->getgroup(),SIGNAL(finished()),this,SLOT(setCurrentDoge()));
    if(!danmushow){
        delete fdoge;
        fdoge = NULL;
        return;
    }
    currentDoge++;
}

void MainWindow::addDoge(QMovie *movie2){
    if(currentDoge>20){
        return ;
    }
    screenRect = desktopWidget->screenGeometry();
    Doge * fdoge=new Doge(NULL,movie2,screenRect,4,this->Transparency,this->DSpeed);//新建一个label
    QObject::connect(showDanmuBut, SIGNAL(clicked()), fdoge, SLOT(changeDogeShow()));
    connect(fdoge->getanimation(),SIGNAL(finished()),this,SLOT(setCurrentDoge()));
    if(!danmushow){
        delete fdoge;
        fdoge = NULL;
        return;
    }
    currentDoge++;
    //DogeList.append(fdoge);//追加进列表中
}

void MainWindow::changeDanmuShowStatus(){       //设置是否隐藏弹幕
    if(danmushow){
        showDanmuBut->setChecked(true);
        this->Transparency = 0.0;
        danmushow = false;
    }else{
        showDanmuBut->setChecked(false);
        this->Transparency = transparencySlider->value()*1.0/255.0;
        danmushow = true;
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)        //Qt的鼠标时间函数重载，下面三个函数共同实现了一个功能，那就是点击窗口任意位置可以拖动窗口移动
{
    if(event->button()&Qt::LeftButton){
    startPos = this->pos();
    clickPos = mapToGlobal(event->pos());
    isLeftPressDown = true;
    this->setFocus();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
        if (this->isMaximized())
            return;
        if(isLeftPressDown)
        this->move(startPos+event->globalPos()-clickPos);
            event->accept();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    //鼠标释放事件
    //自身重绘
    repaint();
    //如果是左键
    if(event->button() == Qt::LeftButton) {
        //左键按下参数为空
        isLeftPressDown = false;
    }
}


void MainWindow::contextMenuEvent(QContextMenuEvent * event)        //右键菜单，暂时没什么卵用，学习界面装饰用
{
    //设置好右键菜单代码，包括添加动作，链接函数，美化
    QAction * cAction    = new QAction("连接/断开服务器", this);
    QAction * hAction    =new QAction("隐藏弹幕",this);
    QAction * tAction    = new QAction("屏蔽顶端", this);
    QAction * bAction  = new QAction("屏蔽底部", this);
    QAction * gAction  = new QAction("高级设置", this);
    QAction * Quit = new QAction("退出",this);
    QAction * aboutThis = new QAction("关于本软件",this);
    QAction * aboutTeam = new QAction("关于作者",this);
    QAction * aboutTime = new QAction("创作初衷(笑)",this);
    QMenu * editMenu=new QMenu();
    QMenu * aboutMenu=new QMenu("关于");
    editMenu->setAutoFillBackground(true);
    editMenu->addAction(cAction);
    editMenu->addAction(hAction);
    editMenu->addAction(tAction);
    editMenu->addAction(bAction);
    editMenu->addSeparator();
    editMenu->addMenu(aboutMenu);
    editMenu->addSeparator();
    aboutMenu->addAction(aboutThis);
    aboutMenu->addAction(aboutTeam);
    aboutMenu->addSeparator();
    aboutMenu->addAction(aboutTime);
    aboutMenu->setStyleSheet("QMenu{"
                                  "padding:5px;"
                                           "background:rgb(46,46,46);"
                            "color:white;"
                                  "border:1px solid gray;"
                                   "}"
                                  " QMenu::item{"
                                  "padding:0px 40px 0px 30px;"
                                  "height:25px;"
                                   "}"
                                   "QMenu::item:selected:enabled{"
                                   "       background:rgb(25,25,25);"
                                   "       color:white;"
                                   "}"
                                   " QMenu::item:selected:!enabled{"
                                   "       background:transparent;"
                                   "}"
                                   "QMenu::separator{"
                                          "height:1px;"
                                          "background:rgb(25,25,25);"
                                  "margin:5px 0px 5px 0px;"
                                   "}");
    editMenu->addAction(gAction);
    editMenu->addAction(Quit);
    editMenu->setStyleSheet("QMenu{"
                                  "padding:5px;"
                                           "background:white;"
                                  "border:1px solid gray;"
                                   "}"
                                  " QMenu::item{"
                                  "padding:0px 40px 0px 30px;"
                                  "height:25px;"
                                   "}"
                                   "QMenu::item:selected:enabled{"
                                   "       background:rgb(90,182,253);"
                                   "       color:white;"
                                   "}"
                                   " QMenu::item:selected:!enabled{"
                                   "       background:transparent;"
                                   "}"
                                   "QMenu::separator{"
                                          "height:1px;"
                                          "background:lightgray;"
                                  "margin:5px 0px 5px 0px;"
                                   "}");


    connect(Quit,SIGNAL(triggered()),this,SLOT(closeWidget()));       //链接动作与槽函数
    connect(cAction,SIGNAL(triggered()),this,SLOT(connectToTCPServer()));
    connect(hAction,SIGNAL(triggered()),this,SLOT(changeDanmuShowStatus()));
    connect(tAction,SIGNAL(triggered()),this,SLOT(changeTopDanmuShowState()));
    connect(bAction,SIGNAL(triggered()),this,SLOT(changeBottomDanmuShowState()));
    connect(gAction,SIGNAL(triggered()),this,SLOT(changeInterface()));
    connect(aboutTeam,SIGNAL(triggered()),this,SLOT(aboutAuthor()));
    connect(aboutThis,SIGNAL(triggered()),this,SLOT(aboutApp()));
    connect(aboutTime,SIGNAL(triggered()),this,SLOT(aboutAim()));

    //在鼠标位置显示菜单并且开启事件循环，此时主窗口会失去焦点
    editMenu->exec(event->globalPos());
    editMenu->deleteLater();

}

bool MainWindow::closeWidget()      //关闭按钮关联的槽函数
{
    writeConfigFile();
    if(clientID!=NULL||clientID!=""){
            req.setUrl(QUrl("http://"+HOST+":8001/update_port?clientId="+this->clientID+"&me=kill&te=0"));
            manager->get(req);
            clientID = "";
    }
    //界面动画，改变透明度的方式消失1 - 0渐变
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(1500);
    animation->setStartValue(1);
    animation->setEndValue(0);
    animation->start();
    connect(animation, SIGNAL(finished()), this, SLOT(close()));

   return true;
}

void MainWindow::paintEvent(QPaintEvent *event)     //主界面的绘制时间，可重构
{
    event->accept();
}

void MainWindow::connectOrNot(){        //是否连接上服务器
    if(isconnectToServer){
        isconnectToServer = false;
        connectBut->setText("连接服务器");
        qrencodeBut->setStyleSheet("QPushButton{border-image: url(:/new/erwei3);}"
                                   "QPushButton:hover{border-image: url(:/new/erwei3);}"
                                   "QPushButton:pressed{border-image: url(:/new/erwei3);}");
        qrencodeBut->setEnabled(false);

        statusLabel->setText("当前状态:未连接");
    }else{
        isconnectToServer = true;
        danmushow = true;
        connectBut->setText("断开服务器");
        if(clientID!=NULL&&clientID!="")
        statusLabel->setText("客户端ID"+clientID);
    }
}

void MainWindow::changeTopDanmuShowState(){     //是否屏蔽顶端
    if(topdanmushow){
        pingUp->setChecked(true);
        topdanmushow = false;
    }else{
        pingUp->setChecked(false);
        topdanmushow = true;
    }
}

void MainWindow::changeBottomDanmuShowState(){      //是否屏蔽底端
    if(bottomdanmushow){
        pingDown->setChecked(true);
        bottomdanmushow = false;
    }else{
        pingDown->setChecked(false);
        bottomdanmushow = true;
    }
}

void MainWindow::changeTransparency(int transparency){      //修改透明度
    if(transparency>=0&&transparency<=255)
    this->Transparency = transparency*1.0/255.0;
}

void MainWindow::changeSpeed(int speed){
    if(speed>=1000&&speed<=60000){
        this->DSpeed = 61000-speed;
    }

}

void MainWindow::changeFontSize(int fontSize){      //修改字体大小
    if(fontSize>=0&&fontSize<=50)
    this->fontSize = fontSize;
}

void MainWindow::OnSuccConnect()        //连接上服务器时执行
{
    isconnectToServer = true;
    connectBut->setText("断开服务器");
}

void MainWindow::ReadStructDataAndShow()        //连接服务器成功时或者有新的socket消息传过来时会调用这个方法  判断消息长度  小于10 就解析数据为客户端ID  否则就解析为弹幕内容
{
    QByteArray dataArray = myTCPClient->readAll();
    if(dataArray.length() <=10)
    {
        clientID = dataArray.data();
        req.setUrl("http://"+HOST+":8001/update_port?clientId="+clientID+"&me=add&te=0");
        manager->get(req);
        if(clientID!=NULL&&clientID!=""){
            statusLabel->setText("客户端ID : "+clientID);
            qrencodeBut->setStyleSheet("QPushButton{border-image: url(:/new/erwei1);}"
                                           "QPushButton:hover{border-image: url(:/new/erwei1);}"
                                           "QPushButton:pressed{border-image: url(:/new/erwei2);}");
            qrencodeBut->setEnabled(true);
        }

        //qDebug()<<QString(dataArray.data());
        //
        return;
    }else{
        //接收长度正确
        QString danmujson1 = QString(dataArray.data());
        QStringList list = danmujson1.split("\n");
        //qDebug()<<"我的数据是"<<danmujson1;
        for(int i = 0;i<list.length();i++){             //此处用Qt的json解析函数解析danmujson
            QString danmujson = list.at(i);
            //qDebug()<<"弹幕"<<danmujson;
            QByteArray dataArray1 = danmujson.toUtf8();
            QJsonParseError json_error;
            QJsonDocument parse_doucment = QJsonDocument::fromJson(dataArray1, &json_error);
            if(json_error.error == QJsonParseError::NoError)
            {
                if(parse_doucment.isArray())
                {
            QJsonArray array = parse_doucment.array();
            int size = array.size();
            for(int i=0; i<size;i++){
                QJsonValue value = array.at(i);
                if(value.isObject())
                {
                    QJsonObject obj = value.toObject();//Qt5新类
                    QString content,clientId,color = "White";
                    int type = 0;
                    if(obj.contains("content"))
                    {
                        QJsonValue content_value = obj.take("content");
                        if(content_value.isString())
                        {
                                content = content_value.toString();
                                //qDebug()<<content<<endl;
                        }
                    }
                    if(obj.contains("color"))
                    {
                        QJsonValue color_value = obj.take("color");
                        if(color_value.isString())
                        {
                                color = color_value.toString();
                                //qDebug()<<color<<endl;
                        }
                    }
                    if(obj.contains("type"))
                    {
                        QJsonValue type_value = obj.take("type");
                        if(type_value.isDouble())
                        {
                                type = type_value.toInt();
                                //qDebug()<<type<<endl;
                        }
                    }
                    if(obj.contains("clientId"))
                    {
                        QJsonValue clientId_value = obj.take("clientId");
                        if(clientId_value.isString())
                        {
                                clientId = clientId_value.toString();
                                //qDebug()<<clientId<<endl;
                        }
                    }
                    if(type == 4){
                        //int ran = qrand()%2;
                        QMovie * movie2 = NULL;
                        //if(ran == 0){
                        movie2 = new QMovie(":/project/rundoge");
                        //}else{
                        //    movie2 = new QMovie(":/project/doge2");
                        //}
                        this->addDoge(movie2);
                    }else if(type == 7){
                        QPixmap * picXN = new QPixmap(":/new/llleft");
                        this->addDoge(picXN);
                        QPixmap * picXN1 = new QPixmap(":/new/llright");
                        this->addDoge(picXN1,1);
                        QPixmap * picXN2 = new QPixmap(":/new/lltright");
                        this->addDoge(picXN2,2);
                        QPixmap * picXN3 = new QPixmap(":/new/lltleft");
                        this->addDoge(picXN3,3);
                    }else if(clientId!=NULL&&clientId!=""){
                        if(content!=NULL&&content!="")
                            for(int i=0;i<banlist->count();i++){
                                if(content.contains(banlist->item(i)->text(),Qt::CaseInsensitive)){
                                    return ;
                                }
                            }
                            this->addDanmu(content,color,type,QFont("SimHei",this->fontSize,100));      //加弹幕
                        //qDebug()<<"Send弹幕前"<<content<<","<<color<<","<<type<<endl;
                    }
                }
            }
                }
            }
        }
    }

}

void MainWindow::connectToTCPServer(){      //连接到服务端，这里可以设置连接到哪里的服务器
    if(isconnectToServer){
        QMessageBox box;
        box.setWindowTitle("断开连接");
        box.setIcon(QMessageBox::Question);
        box.setFixedSize(187,123);
        box.setText("确定要断开连接吗？");
        box.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
        box.move((this->pos().x()+(this->width()-box.width())/2),this->pos().y()+(this->height()-box.height())/2);
        if(box.exec()==QMessageBox::Yes){
            isconnectToServer = false;
            myTCPClient->close();
            connectBut->setText("连接服务器");
            statusLabel->setText("当前状态:未连接");
            qrencodeBut->setEnabled(false);
            qrencodeBut->setStyleSheet("QPushButton{border-image: url(:/new/erwei3);}"
                                           "QPushButton:hover{border-image: url(:/new/erwei3);}"
                                           "QPushButton:pressed{border-image: url(:/new/erwei3);}");
        }
    }else{
        //myTCPClient->connectToHost(QString("121.42.205.237"), 2000);
        //myTCPClient->connectToHost(QString("45.63.122.169"),2000);      //这个ip是我租的服务器,你也可以租自己的服务器
        //myTCPClient->connectToHost(QString("127.0.0.1"),2000);        //开发调试的时候可以用这个本机ip测试
        myTCPClient->connectToHost(HOST,PORT);
    }

}

void MainWindow::disconnectTCPServer(){     //断开与服务器的连接
    isconnectToServer = false;
    req.setUrl(QUrl("http://"+HOST+":8001/update_port?clientId="+this->clientID+"&me=kill&te=0"));
    manager->get(req);
    clientID = "";
    connectBut->setText("连接服务器");
    statusLabel->setText("当前状态:未连接");
}

void MainWindow::writeConfigFile()
{
    //写文件
    QString banword;

    //如果包含文件路径的fileList以读写的方式打开成功
    if (configFile->open(QIODevice::ReadWrite | QIODevice::Text))
    {
     //创建文件流
     QTextStream floStream(configFile);
     //设置保存格式，此处选择UTF-8
     QTextCodec *codec=QTextCodec::codecForName("UTF-8");
     floStream.setCodec(codec);
     configFile->resize(0);
     floStream<<HOST<<endl;
     floStream<<PORT<<endl;
     floStream<<MaxTotal<<endl;
     floStream<<MaxTop<<endl;
     floStream<<MaxBottom<<endl;

     //若列表没有项,清空文件
     if(banlist->count()==0)
     {
         return ;
     }
     else
     {
         //否则便利列表写入路径进文件
         for(int i=0;i<banlist->count();i++)
         {
             banword=banlist->item(i)->data(Qt::UserRole).toString();
             floStream<<banword<<endl;
         }
     }
    }

    //关闭文件
    configFile->close();
}

void MainWindow::readConfigFile()
{
    //读取文件列表
    QString Listline;
    //存文件路径
    configFile=new QFile(QCoreApplication::applicationDirPath()+"//YuFeiDanmu.config");

    //如果以读写方式打开成功
    if (configFile->open(QIODevice::ReadWrite | QIODevice::Text))
    {
        //创建读文件流
        QTextStream floStream(configFile);
        //设置读的方式,此处用UTF-8读取
        QTextCodec *codec=QTextCodec::codecForName("UTF-8");
        floStream.setCodec(codec);
        HOST = floStream.readLine();
        PORT = floStream.readLine().toInt();
        MaxTotal = floStream.readLine().toInt();
        MaxTop = floStream.readLine().toInt();
        MaxBottom = floStream.readLine().toInt();
        //当文件没有到尾
        while ( floStream.atEnd()==0 )
        {

            //读一行
            Listline =floStream.readLine();

            //查看获取路径
            //qDebug()<<Listline<<endl;


            //增添列表项,设置列表的文字显示为简单文件名
            QListWidgetItem * list1 = new QListWidgetItem(Listline, banlist);
            //数据存路径
            list1->setData(Qt::UserRole,Listline);
            list1->setToolTip(Listline);
            banlist->insertItem(Qt::UserRole, list1);
        }
    }
    //文件关闭
    configFile->close();
}

void MainWindow::addBanWordList(){
    if(banWordEdit->text()==""){
        return ;
    }
    for(int i=0;i<banlist->count();i++){
        if(banWordEdit->text()==banlist->item(i)->data(Qt::UserRole).toString()){
            return ;
        }
    }

    QListWidgetItem * item =new QListWidgetItem(banWordEdit->text(),banlist);
    item->setData(Qt::UserRole,banWordEdit->text());
    item->setToolTip(banWordEdit->text());
    banlist->insertItem(Qt::UserRole, item);
    banWordEdit->setText("");
}

void MainWindow::updateConfig(){
    HOST = hostEdit->text();
    if(HOST==""){
        hostEdit->setText(DEFAULT_HOST);
        HOST = DEFAULT_HOST;
    }
    PORT = portEdit->text().toInt();
    if(PORT==0){
        PORT=DEFAULT_PORT;
        portEdit->setText(QString("%1").arg(DEFAULT_PORT));
    }
    if(MaxTotalEdit->text().toInt()<20){
        MaxTotal = 50;
        MaxTotalEdit->setText(QString("%1").arg(MaxTotal));
    }else{
        MaxTotal = MaxTotalEdit->text().toInt();
        MaxTotalEdit->setText(QString("%1").arg(MaxTotal));
    }
    MaxTop = MaxTopEdit->text().toInt();
    if(MaxTop == 0){
        MaxTop = 10;
        MaxTopEdit->setText(QString("%1").arg(MaxTop));
    }
    MaxBottom = MaxBottomEdit->text().toInt();
    if(MaxBottom == 0){
        MaxBottom = 10;
        MaxBottomEdit->setText(QString("%1").arg(MaxBottom));
    }
    QMessageBox box;
    box.setWindowTitle("通知");
    box.setIcon(QMessageBox::Information);
    box.setText("修改成功~");
    box.setStandardButtons(QMessageBox::Yes);
    box.setFixedSize(187,123);
    box.move((this->pos().x()+(this->width()-box.width())/2),this->pos().y()+(this->height()-box.height())/2);
    if(box.exec()==QMessageBox::Yes){
    }
}

void MainWindow::changeInterface(){
    if(hiddenShow){
        hideWidget->hide();
        messageWidget->show();
        hiddenShow = false;
    }else{
        messageWidget->hide();
        hideWidget->show();
        hiddenShow = true;
    }
}

void MainWindow::updateMessageWidget(){
    switch(mstatus){
    case 0:
        e.load(":/project/bafan1");
        g.load(":/project/caijia1");
        messageWidget->setPaintStatus(0);
        leftCha->show();
        middleCha->setPixmap(e);
        middleCha->setFixedSize(e.size());
        rightCha->setPixmap(g);
        rightCha->setFixedSize(g.size());
        middleCha->show();
        rightCha->show();
        messageWidget->repaint();
        mstatus++;
        break;
    case 1:
        e.load(":/project/caijia2");
        g.load(":/project/caimuzuo1");
        messageWidget->setPaintStatus(0);
        leftCha->show();
        middleCha->setPixmap(e);
        middleCha->setFixedSize(e.size());
        rightCha->setPixmap(g);
        rightCha->setFixedSize(g.size());
        middleCha->show();
        rightCha->show();
        messageWidget->repaint();
        mstatus++;
        break;
    case 2:
        e.load(":/project/caimuzuo2");
        g.load(":/project/pingzhong1");
        messageWidget->setPaintStatus(0);
        leftCha->show();
        middleCha->setPixmap(e);
        middleCha->setFixedSize(e.size());
        middleCha->show();
        rightCha->setPixmap(g);
        rightCha->setFixedSize(g.size());
        rightCha->show();
        messageWidget->repaint();
        mstatus++;
        break;
    case 3:
        e.load(":/project/pingzhong2");
        g.load(":/project/shaxi1");
        messageWidget->setPaintStatus(0);
        leftCha->show();
        middleCha->setPixmap(e);
        middleCha->setFixedSize(e.size());
        middleCha->show();
        rightCha->setPixmap(g);
        rightCha->setFixedSize(g.size());
        rightCha->show();
        messageWidget->repaint();
        mstatus++;
        break;
    case 4:
        e.load(":/project/shaxi2");
        g.load(":/project/hailaoming1");
        messageWidget->setPaintStatus(0);
        leftCha->show();
        middleCha->setPixmap(e);
        middleCha->setFixedSize(e.size());
        middleCha->show();
        rightCha->setPixmap(g);
        rightCha->setFixedSize(g.size());
        rightCha->show();
        messageWidget->repaint();
        mstatus++;
        break;
    case 5:
        e.load(":/project/hailaoming2");
        g.load(":/project/youmeizi1");
        messageWidget->setPaintStatus(0);
        leftCha->show();
        middleCha->setPixmap(e);
        middleCha->setFixedSize(e.size());
        rightCha->setPixmap(g);
        rightCha->setFixedSize(g.size());
        messageWidget->repaint();
        mstatus++;
        break;
    case 6:
        e.load(":/project/youmeizi2");
        g.load(":/project/yeshan1");
        messageWidget->setPaintStatus(0);
        leftCha->show();
        middleCha->setPixmap(e);
        middleCha->setFixedSize(e.size());
        middleCha->show();
        rightCha->setPixmap(g);
        rightCha->setFixedSize(g.size());
        rightCha->show();
        messageWidget->repaint();
        mstatus++;
        break;
    case 7:
        e.load(":/project/yeshan2");
        g.load(":/project/caiyu1");
        messageWidget->setPaintStatus(0);
        leftCha->show();
        middleCha->setPixmap(e);
        middleCha->setFixedSize(e.size());
        middleCha->show();
        rightCha->setPixmap(g);
        rightCha->setFixedSize(g.size());
        rightCha->show();
        messageWidget->repaint();
        mstatus++;
        break;
    case 8:
        e.load(":/project/caiyu2");
        g.load(":/project/yangnai1");
        messageWidget->setPaintStatus(0);
        leftCha->show();
        middleCha->setPixmap(e);
        middleCha->setFixedSize(e.size());
        middleCha->show();
        rightCha->setPixmap(g);
        rightCha->setFixedSize(g.size());
        rightCha->show();
        messageWidget->repaint();
        mstatus++;
        break;
    case 9:
        e.load(":/project/yangnai2");
        g.load(":/project/xiaoting1");
        messageWidget->setPaintStatus(0);
        leftCha->show();
        middleCha->setPixmap(e);
        middleCha->setFixedSize(e.size());
        middleCha->show();
        rightCha->setPixmap(g);
        rightCha->setFixedSize(g.size());
        rightCha->show();
        messageWidget->repaint();
        mstatus++;
        break;
    case 10:
        e.load(":/project/xiaoting2");
        g.load(":/project/jieyi1");
        messageWidget->setPaintStatus(0);
        leftCha->show();
        middleCha->setPixmap(e);
        middleCha->setFixedSize(e.size());
        middleCha->show();
        rightCha->setPixmap(g);
        rightCha->setFixedSize(g.size());
        rightCha->show();
        messageWidget->repaint();
        mstatus++;
        break;
    case 11:
        e.load(":/project/jieyi2");
        g.load(":/project/xuenai1");
        messageWidget->setPaintStatus(0);
        leftCha->show();
        middleCha->setPixmap(e);
        middleCha->setFixedSize(e.size());
        middleCha->show();
        rightCha->setPixmap(g);
        rightCha->setFixedSize(g.size());
        rightCha->show();
        messageWidget->repaint();
        mstatus++;
        break;
    case 12:
        e.load(":/project/xuenai2");
        g.load(":/project/bafan2");
        messageWidget->setPaintStatus(0);
        leftCha->show();
        middleCha->setPixmap(e);
        middleCha->setFixedSize(e.size());
        middleCha->show();
        rightCha->setPixmap(g);
        rightCha->setFixedSize(g.size());
        rightCha->show();
        messageWidget->repaint();
        mstatus++;
        break;
    case 13:
        middleCha->hide();
        rightCha->hide();
        leftCha->hide();
        messageWidget->setPaintStatus(1);
        messageWidget->repaint();
        mstatus++;
        break;
    case 14:
        middleCha->hide();
        rightCha->hide();
        leftCha->hide();
        messageWidget->setPaintStatus(2);
        messageWidget->repaint();
        mstatus++;
        break;
    case 15:
        middleCha->hide();
        rightCha->hide();
        leftCha->hide();
        messageWidget->setPaintStatus(3);
        messageWidget->repaint();
        mstatus++;
        break;
    case 16:
        middleCha->hide();
        rightCha->hide();
        leftCha->hide();
        messageWidget->setPaintStatus(4);
        messageWidget->repaint();
        mstatus++;
        break;
    case 17:
        middleCha->hide();
        rightCha->hide();
        leftCha->hide();
        messageWidget->setPaintStatus(5);
        messageWidget->repaint();
        mstatus++;
        break;
    case 18:
        middleCha->hide();
        rightCha->hide();
        leftCha->hide();
        messageWidget->setPaintStatus(6);
        messageWidget->repaint();
        mstatus++;
        break;
    case 19:
        middleCha->hide();
        rightCha->hide();
        leftCha->hide();
        messageWidget->setPaintStatus(7);
        messageWidget->repaint();
        mstatus++;
        break;
    case 20:
        middleCha->hide();
        rightCha->hide();
        leftCha->hide();
        messageWidget->setPaintStatus(8);
        messageWidget->repaint();
        mstatus++;
        break;
    case 21:
        middleCha->hide();
        rightCha->hide();
        leftCha->hide();
        messageWidget->setPaintStatus(9);
        messageWidget->repaint();
        mstatus++;
        break;
    case 22:
        middleCha->hide();
        rightCha->hide();
        leftCha->hide();
        messageWidget->setPaintStatus(10);
        messageWidget->repaint();
        mstatus++;
        break;
    case 23:
        middleCha->hide();
        messageWidget->setPaintStatus(0);
        messageWidget->setPaintStatus(0);
        d.load(":/project/meiyouDanmu");
        g.load(":/project/dadanmu1");
        leftCha->setPixmap(d);
        leftCha->setFixedSize(d.size());
        leftCha->show();
        rightCha->setPixmap(g);
        rightCha->setFixedSize(g.size());
        rightCha->show();
        messageWidget->repaint();
        mstatus++;
        break;

    default:
        mstatus = 0;
    }
}

void MainWindow::aboutApp(){
    MsgBox * msg = new MsgBox(NULL,"YuFeiDanmaku","欢迎使用本软件哦~希望通过这款软件，你和你的朋友能够享受愉(si)快(ming)吐(si)槽(bi)的感觉poi~\n  如果有兴趣，欢迎看看关于作者与这款软件制作的初衷与心路历程(笑)\n\n默认域名:121.42.205.237,默认端口2000");
    msg->move(this->pos().rx()+(this->width()-msg->width())/2,this->pos().ry()+80);
    msg->exec();
    msg->deleteLater();
}

void MainWindow::aboutAim(){
    MsgBox * msg = new MsgBox(NULL,"YuFeiDanmaku","无论是网页，桌面应用还是将来可能做得手机应用都是坚持一个初衷——宣传《我的青春恋爱喜剧果然有问题》小说与动画都是优秀的作品，在国内小众，但真的很有普及的意义~我会加油滴~");
    msg->move(this->pos().rx()+(this->width()-msg->width())/2,this->pos().ry()+80);
    msg->exec();
    msg->deleteLater();
}

void MainWindow::aboutAuthor(){
    MsgBox * msg = new MsgBox(NULL,"YuFeiDanmaku","  华南理工大学 软件学院 2013级 2班  叶宇飞 QQ:635560243 (欢迎花式骚扰哦~不过太烦的话，我会报警的）励志成为春学家(壮哉我大春物~)");
    msg->move(this->pos().rx()+(this->width()-msg->width())/2,this->pos().ry()+80);
    msg->exec();
    msg->deleteLater();
}

void MainWindow::setCurrentNum(){
    this->currentNum--;
    if(currentNum<0){
        currentNum=0;
    }
}

void MainWindow::setCurrent2(){
    this->current2--;
    this->currentNum--;
    if(current2<0){
        current2 = 0;
    }
    if(currentNum<0){
        currentNum=0;
    }
}

void MainWindow::setCurrent3(){
    this->current3--;
    this->currentNum--;
    if(current3<0){
        current3=0;
    }
    if(currentNum<0){
        currentNum=0;
    }
}

void MainWindow::setCurrentDoge(){
    this->currentDoge--;
    if(currentDoge<0){
        currentDoge=0;
    }
}

void MainWindow::wheelEvent(QWheelEvent * event)
{

    //就收鼠标滑轮的信号
    double numdegrees=event->delta()/60;

    //改变声音大小
    if(numdegrees<0){
        if(mstatus>23)
            mstatus = 0;
        updateMessageWidget();
    }else{
        mstatus-=2;
        if(mstatus==-2){
            mstatus=22;
        }
        if(mstatus==-1){
            mstatus=23;
        }
        //qDebug()<<mstatus;
        updateMessageWidget();
    }
    event->accept();

}

void MainWindow::drawQrenCodePic(){

    QRenWidget * w = new QRenWidget();
    w->setURL(QString("http://"+HOST+":8001/?clientId="+clientID));
    w->setWindowFlags(Qt::WindowSystemMenuHint |Qt::MaximizeUsingFullscreenGeometryHint| Qt::WindowMinimizeButtonHint|Qt::WindowCloseButtonHint);
    w->setRect(screenRect);
    w->createQRCode();
    w->show();
    w->setAttribute(Qt::WA_DeleteOnClose);
}
