#ifndef WIDGET_H
#define WIDGET_H
#include"aboutdialog.h"//用于点击触犯另一个窗口
#include <QWidget>
#include<QPainter>
#include<QMessageBox>
#include<QRect>

//网络编程头文件
#include<QNetworkAccessManager>//网络访问获取响应，处理错误
#include<QNetworkReply>//上面头文件发送请求之后返回的相应类

//添加多媒体播放器类
#include<QMediaPlayer>//播放暂停...
//添加多媒体播放列表类 管理很多个音频的播放数序
#include<QMediaPlaylist>

//文本读取列表信息
#include<QTextBlock>
#include<QTimer>

//打开本地文件
#include<QFileDialog>

//网上实现信息反馈 json
#include<QJsonParseError>//解析Json数据类型处理类
#include<QJsonObject>//用于管理Json数组的类 可读取，创建

#include<QLCDNumber>//用于显示剩余时间

#include<QJsonArray>
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();


public:
    void paintEvent(QPaintEvent *event);

    //拖动窗口
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent *event);
    bool m_MouseDrag ;//是否拖动
    QPoint mouseStartPoint;
    QPoint windowStartPoi;


    //20250418自定义成员如下 :利于下次开发
    QMediaPlayer *P_PlayerObject;//定义多媒体播放器对象
    QMediaPlaylist * P_PlayerList;//定义多媒体播放器列表 P开头表示指针

    //QString StrLocalMusicPath;

    //歌曲ID 歌手名 歌曲名称
    int I_Music_Id;
    QString StruMusicName , StrSinnerName ;
    int iPlayFlags;//定义音乐播放位置
    //存储搜索信息
    QByteArray QByteArySearchInfo;
    //处理富文本内容 什么是富文本？富文本（Rich Text）是指在文本内容中嵌入格式、样式、图像、链接等多媒体元素的文本格式。与纯文本（Plain Text）相比，富文本不仅包含文字信息，还可以通过不同的字体、颜色、大小、段落格式等来增强内容的表达效果。富文本广泛应用于各种文档编辑、网页设计、邮件发送等场景。
    QTextDocument *docTextObject;//读取文本框的内容
    QTextBlock *qTestline;//处理文本块指针 文本块（Text Block）是富文本处理和编程中的结构化文本单元，主要用于组织和管理多行文本内容

    //网络访问对象
    QNetworkAccessManager * QNetWorkAccessMangers;
//自定义槽函数
public slots:
    //处理请求返回的数据的函数
    void HandleDataBackFunc(QNetworkReply * pReply);
    //处理LCDNUmber灯控制实践变化
    void HandleLCDNumberTimeChanegFunc(qint64 duration);
    //处理进度条控制变化
    void HandleProgessTimeChangeFunc(qint64 dration);
    //处理播放位置变化
    void HandelPositonChangeFunc(qint64 position);
    //处理播放歌曲
    void HandlePlaySongFunc();

private slots:
    void on_pushButton_Close_clicked();

    void on_pushButton_About_clicked();

    void on_pushButton_add_clicked();

    void on_pushButton_play_clicked();

    void on_pushButton_start_clicked();

    void on_pushButton_stop_clicked();

    void on_pushButton_yes_3_clicked();

    void on_pushButton_pre_clicked();

    void on_pushButton_searchSongs_clicked();

    void on_pushButton_next_clicked();

    void on_pushButton_yes_clicked();

    void on_horizontalSlider_Volumn_valueChanged(int value);

    void on_horizontalSlider_Palygrogress_valueChanged(int value);

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
