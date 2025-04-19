#include "widget.h"
#include "ui_widget.h"

//#pragma execution_character_set("utf-8");//utf-8 当出现乱码
Widget::Widget(QWidget *parent)//初始话构造函数 槽函数关联咋此完成
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    //去掉标题
    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setFixedSize(this->width(),this->height());

    //将光标 设置在搜素框后面 lineEdit 搜索狂名称
    ui->lineEdit->setFocus();
    //初始化网路访问对象
    QNetWorkAccessMangers = new QNetworkAccessManager(this);
    iPlayFlags = 0;
    //初始文本对对象 plainTextEdit_singlist 是组件名
    docTextObject = ui->plainTextEdit_singlist->document();
    //设置控件为只读
    ui->plainTextEdit_singlist->setReadOnly(true);

    P_PlayerObject = new QMediaPlayer(this);
    P_PlayerList = new QMediaPlaylist(this);
    //播放列表在播放器中间
    P_PlayerObject->setMedia(P_PlayerList);
    //设置播放列表模式
    P_PlayerList->setPlaybackMode(QMediaPlaylist::Loop);//循环播放
    //信号与槽函数处理如下; sender：信号的发送者。signal：发送的信号(信号函数)。receiver：信号的接受者。method：接受信号的槽函数。
    connect(P_PlayerObject,SIGNAL(positionChanged(qint64)),this,SLOT(HandleLCDNumberTimeChanegFunc(qint64)));//比如播放位置发生变化 当播放器发生变化 能够使水晶灯变化
    connect(P_PlayerObject,SIGNAL(positionChanged(qint64)),this,SLOT(HandelPositonChangeFunc(qint64)));//比如播放位置发生变化
    connect(P_PlayerObject,SIGNAL(positionChanged(qint64)),this,SLOT(HandleProgessTimeChangeFunc(qint64)));//比如播放位置发生变化
    connect(QNetWorkAccessMangers,SIGNAL(finished(QNetworkReply*)),this,SLOT(HandleDataBackFunc(QNetworkReply *)));//比如播放位置发生变化connect(P_PlayerObject,SIGNAL(positionChanged(qint64)),this,SLOT());//比如播放位置发生变化
//完成访问请求槽函数
}

Widget::~Widget()
{
    delete ui;
}
void Widget::paintEvent(QPaintEvent *event)
{
    QPainter qPainter(this);
    qPainter.drawPixmap(0,0,width(),height(),QPixmap( ));
}
//关闭
void Widget::on_pushButton_Close_clicked()
{
    this->close();
}
//左键拖动功能
//是否点左键
void Widget::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        m_MouseDrag = true ;
        // 记录鼠标按下的全局坐标（相对于屏幕左上角）
        mouseStartPoint = event->globalPos();

                // 获取当前窗口左上角的屏幕坐标
                // this->frameGeometry() 返回包含窗口边框的几何信息
                // topLeft() 提取窗口左上角坐标
        windowStartPoi = this->frameGeometry().topLeft();
    }
}

void Widget::mouseMoveEvent(QMouseEvent * event)
{
    if(m_MouseDrag)
    {
        //获取鼠标移动距离
        QPoint m_Distance = event->globalPos() - mouseStartPoint;
        this->move(windowStartPoi+m_Distance);
    }
}
void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    //释放鼠标左键
    if(event->button()==Qt::LeftButton)
    {
        m_MouseDrag = false;
    }
}
void Widget::on_pushButton_About_clicked()
{
    //QMessageBox::information(this, "提示", "关于",QMessageBox::Yes);//创建一个心得对话框 上面是提示 下面是 data 不能用ang结尾
    //创建一个AboutDialog类的实例，该对话框用于显示关于信息
    // this指针表示将MainWindow作为AboutDialog的父窗口，确保对话框在MainWindow的上下文中显示
    AboutDialog *pAboutDialogs = new AboutDialog(this);

    //设置对话框的位置
    QRect qRect = geometry();
    int iX = qRect.x() + 79.5;
    int iY = qRect.y() + 87;
    pAboutDialogs->setGeometry(iX,iY,513,436);
    // 计算关于对话框的X坐标，基于当前窗口的X坐标向右偏移144个单位

  // 计算关于对话框的Y坐标，基于当前窗口的Y坐标向下偏移158个单位
        // 执行模态对话框，调用exec()方法使对话框成为模态
        // 这意味着用户必须先关闭
    pAboutDialogs->exec();//exec 不能操作 show 可以拖动
}

//处理请求返回的数据的函数
void Widget::HandleDataBackFunc(QNetworkReply * pReply)
{
     QByteArySearchInfo=pReply->readAll();//读取网络返回数据  用途‌：通常用于接收API返回的JSON或二进制数据‌ json 从网络响应对象 QNetworkReply 中获取所有返回数据，存储为 QByteArray 类型‌
     QJsonParseError JsonPError;//定义错误信息对象 Qt提供的类，用于捕获JSON解析时的错误类型和位置‌
     //将网络数据(Json文本 "{\"name\":\"张三\",\"age\":25}")转化为Json文本对象({ name: "张三", age: 25 }  // 解析后的对象，可直接操作属) JSON（JavaScript Object Notation）是一种‌轻量级的文本数据格式‌，用于存储和交换结构化数据。它基于 JavaScript 的对象语法，但独立于编程语言，支持跨平台使用‌
     QJsonDocument JSonDoc_RecvData=QJsonDocument::fromJson(QByteArySearchInfo,&JsonPError);//‌静态方法，将JSON格式的 QByteArray 转换为 QJsonDocument 对象‌
     if(JsonPError.error != QJsonParseError::NoError)//如果有错
     {
         QMessageBox::critical(this,"prompt","提示：歌曲Json格式错误",QMessageBox::Yes);//新疆窗口表示是否出错
         return ;
     }
//JSonDoc_RecvData中存储的是一个完整的JSON文档，object()方法会忽略这些额外信息，只返回JSON对象本身。
    QJsonObject TotalJsonObject = JSonDoc_RecvData.object();

    //列出json所有key
    QStringList strKeys=TotalJsonObject.keys();
    //如果有数据信息
    if(strKeys.contains("result"))//调用网易云的接口
    {
        //将带有result的数据内容提取转化成对象，就是和key相关联的数据 要看官网API 从而知道传回来的数据该怎么操作！！！
        QJsonObject resultobject=TotalJsonObject["result"].toObject();
        //存储所有的keys keys证明能搜索到歌曲
        QStringList strResultKeys=resultobject.keys();
        if(strResultKeys.contains("songs"))//如果关键字含有目标数据
        {
            QJsonArray array = resultobject["songs"].toArray();// 数组‌的类，封装了 JSON 数组的常见操作，如插入、删除、遍历等‌
            //为什么需要数组 因为数组本身利于操作 其次唱歌的人不只一个 每个array包含一个音乐对象
            for(auto isong:array)
            {
                QJsonObject jsonobject1=isong.toObject();
                //获取歌曲ID 歌名 歌手
                I_Music_Id = jsonobject1["id"].toInt();
                StruMusicName=jsonobject1["name"].toString();

                //集中jsonobject1所有的键
                QStringList strkeys = jsonobject1.keys();
                if(strkeys.contains("artists"))//如果strkeys包含"artists"键，说明当前歌曲对象包含歌手信息。
                {
                    QJsonArray artistsjsonarray=jsonobject1["artists"].toArray();//将所有歌手信息集合到一个数组
                    for(auto js:artistsjsonarray)
                    {
                        QJsonObject jsonobject2 = js.toObject();
                        StrSinnerName = jsonobject2["name"].toString();
                    }
                }
            }
        }
    }
    //显示音乐
    QString strURL;//找到163音乐的地址
    //arg函数用于替换占位符
    strURL=QString("https://music.163.com/song/media/outer/url?id=%0").arg(I_Music_Id);//%0为站位符
    //把地址放到播器里去
     P_PlayerList->addMedia(QUrl(strURL));
     //把歌名 歌手添加出来
     ui->plainTextEdit_singlist->appendPlainText(StruMusicName+"-"+StrSinnerName);
}

//处理LCDNUmber灯控制实践变化
void Widget::HandleLCDNumberTimeChanegFunc(qint64 duration)//传入毫秒参数
{
    int int_second=duration/1000;
    int int_Minute=int_second/60;
    int_second=int_second%60;
    QString strSongTime=QString::asprintf("%d:%d",int_Minute,int_second);//时间转换成字符串
    ui->lcdNumber_playerTime->display(strSongTime);

}

//处理进度条控制变化
void Widget::HandleProgessTimeChangeFunc(qint64 dration){

}

//处理播放位置变化
void Widget::HandelPositonChangeFunc(qint64 position)
{

}

//处理播放歌曲
void Widget::HandlePlaySongFunc()//注意不要漏了Widget这很重哟
{

}
//添加本地歌曲

void Widget::on_pushButton_add_clicked()
{
    QString strCurrentPaths = QDir::homePath();//当前用户的主目录 选择导入音乐文件的位置 常用的位置
    QString strDlgTitle = "请选择音乐文件";
    QString strFilters = "所有文件(*.*);;音频文件(*.mp3);;mp3文件(*.mp3)";//用户会看到一个下拉菜单，包含以下选项：所有文件‌（显示所有文件）‌音频文件‌（仅显示 .mp3 文件）‌mp3文件‌（仅显示 .mp3 文件）
    QStringList strMp4FilrList = QFileDialog::getOpenFileNames(this,strDlgTitle,strCurrentPaths,strFilters);//该方法弹出一个模态文件选择对话框，允许用户‌多选‌符合过滤条件的文件，并返回选中文件的‌完整路径列表‌
   /*this‌

类型：QWidget*
作用：指定父窗口，确保对话框居中显示并受父窗口生命周期管理‌12。
‌strDlgTitle‌

类型：QString
作用：设置对话框标题文本（如“请选择MP3音乐文件”）‌15。
‌strCurrentPaths‌

类型：QString
作用：指定初始目录路径（如 QDir::homePath() 表示用户主目录）‌12。
若为空或无效路径，默认使用系统当前目录。
‌strFilters‌

类型：QString
作用：定义文件类型过滤器，格式为 "描述(*.扩展名);;..."，例如：*/
    if (strMp4FilrList.count()<1) return ;
    for (int i = 0 ; i < strMp4FilrList.count() ; i++)
    {
        QString strFiles = strMp4FilrList.at(i);// 获取当前文件的路径
        P_PlayerList->addMedia(QUrl::fromLocalFile(strFiles));// 将音乐文件添加到播放列表
        QFileInfo qFileInfo(strFiles);//获取信息
        ui->plainTextEdit_singlist->appendPlainText(qFileInfo.fileName());
    }
    //添加完毕 立即播放
    if(P_PlayerObject->state()!=QMediaPlayer::PlayingState)
        P_PlayerList->setCurrentIndex(0);//设置播放列表首个列为指定
    P_PlayerObject->play();//通过播放列表

}
//播放歌曲
void Widget::on_pushButton_play_clicked()
{
    if(iPlayFlags==0)
    {
        P_PlayerObject->play();
        iPlayFlags=1;
    }
    else if(P_PlayerList->currentIndex()<0)
    {
        P_PlayerList->setCurrentIndex(0);
    }
    P_PlayerObject->play();//因为有些歌曲不给下载 所以没有播放


}
//暂停播放
void Widget::on_pushButton_start_clicked()
{
    if(iPlayFlags==1)
    {
        P_PlayerObject->pause();
        iPlayFlags=1;

    }
}
//停止播放冲头开始
void Widget::on_pushButton_stop_clicked()
{
     P_PlayerObject->stop();
}

void Widget::on_pushButton_yes_3_clicked()
{

}

void Widget::on_pushButton_pre_clicked()
{
    int m_index=P_PlayerList->currentIndex();
    int m_indexSum=P_PlayerList->mediaCount();
    if(m_index>0)
    {
        m_index=m_index-1;
        P_PlayerList->setCurrentIndex(m_index);
        P_PlayerObject->play();
    }
    else
    {
        m_index=m_indexSum-1;
        P_PlayerList->setCurrentIndex(m_index);
        P_PlayerObject->play();
    }
}
void Widget::on_pushButton_next_clicked()
{
    int m_index=P_PlayerList->currentIndex();
    int m_indexSum=P_PlayerList->mediaCount();
    if(m_index<m_indexSum)
    {
        m_index=m_index+1;
        P_PlayerList->setCurrentIndex(m_index);
        P_PlayerObject->play();
    }
    else
    {
        m_index=0;
        P_PlayerList->setCurrentIndex(m_index);
        P_PlayerObject->play();
    }
}

//搜索歌曲实现
void Widget::on_pushButton_searchSongs_clicked()
{
    QString str1 , str2 ;
    str1 = ui->lineEdit->text();//str1读取linetext的内容
    //QMessageBox::information(this,"prompt",str1,QMessageBox::Yes);

    str2="http://music.163.com/api/search/get/web?csrf_token=hlpretag=&hlposttag=&s={"+str1+"}&type=1&offset=0&total=true&limit=1";//生成URL‌URL（统一资源定位符）详解‌‌URL（Uniform Resource Locator，统一资源定位符）‌ 是互联网上用于‌唯一标识和定位资源‌（如网页、图片、API接口等）的字符串。它告诉浏览器或应用程序如何访问特定的网络资源。
    //设置请求对象
    QNetworkRequest networkRequest;
    networkRequest.setUrl(str2);//将请求格式设置给请求对象
   //怎么发送
    QNetWorkAccessMangers->get(networkRequest);//利用QNet..类对应api‌API（应用程序编程接口） 发送GET请求并获取数据
}


//实现静音状态，图标切换问题
void Widget::on_pushButton_yes_clicked()
{
    bool bMutexState=P_PlayerObject->isMuted();//bool判断当前是否静音
    P_PlayerObject->setMuted(!bMutexState);//转换静音模式
    if(!bMutexState)
    {
        ui->pushButton_yes->setIcon(QIcon(":/new/prefix1/images/mute.bmp"));
    }
    else
     {
         ui->pushButton_yes->setIcon(QIcon(":/new/prefix1/images/volumn.bmp"));
    }
}
//播放歌曲拖动控制

//音量控制 拖动控件就会传输int action 注意槽函数是valueChanged别选了第一个
void Widget::on_horizontalSlider_Volumn_valueChanged(int value)
{
    P_PlayerObject->setVolume(value);
}
//注意了刚才出错 因为之前的第一个槽函数还在.h文件 在cpp没有编译出差

void Widget::on_horizontalSlider_Palygrogress_valueChanged(int value)
{
     P_PlayerObject->setPosition(value);
}
