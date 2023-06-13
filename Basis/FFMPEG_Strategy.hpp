//
//  FFMPEG_Strategy.hpp
//  UVAIGC
//
//  Created by 林溪明 on 2023/6/6.
//封装了命令行ffmoeg调用类

#ifndef FFMPEG_Strategy_hpp
#define FFMPEG_Strategy_hpp

#include <stdio.h>
#include <QObject>
#include <QProcess>
#include <QSize>

class FFMPEG_Strategy : public QObject
{
    Q_OBJECT
public:
    explicit FFMPEG_Strategy(QObject *parent = nullptr);

    ~FFMPEG_Strategy();
    
//    void test();
    
    void runProcess(QStringList incmd,int _tag);
    bool getIs_finish(){
        return is_finish;
    };
    // 取消
    void cancel();
    
    
    //获取参数 直接传入地址
    bool loadFile_sync(QString url_path);
    QSize getSize();
    long getDuration();
    QImage ThumbnailImage(int w,int h);
//    QString getMedia_type();
    
    QString ffmoeg_info_out;
    //获取缩略图
    QByteArray ThumbnailData;
    
    
    
    // 后台预加载
    void loadFfmpegBin();
    
    
    
    
    
    
    
private slots:
    //获取终端输出信息
    void onReadyReadStandardError();
    
   
    
    
    
signals:
    void progress(float jd,int tag);
    //完成
    void finished();
    
    
    
private:
    QProcess *ffmpegProcess = nullptr;
    QString bin_url;
    
    //毫秒
    long framess;
    
    //标记释放成功
    bool is_finish = false;
    
    //任务标识,方便不同任务传入不同标识追踪
    int tag = -1;
    
  

};



#endif /* FFMPEG_Strategy_hpp */
