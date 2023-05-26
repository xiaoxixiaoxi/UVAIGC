//
//  AVS_Task_QTtool.cpp
//  UVAIGC
//
//  Created by 林溪明 on 2023/5/11.
//

#include "AVS_Task_QTtool.hpp"
#include <QMediaPlayer>
#include <QMediaPlayer>
#include <QMediaMetaData>
#include <QFileInfo>
#include "../Basis/SRTQTime.h"

#include <QProcess>


// 判断是否可以导入
bool AVS_Task_QTtool::if_hasVideo(AVS_Task& obj ,QUrl url)
{
    QMediaPlayer* pl = new QMediaPlayer();
//    QVideoSink* qvsik = new  QVideoSink();
//    pl->setVideoSink(qvsik);

    //QUrl url = QUrl::fromLocalFile("D:/tst.mp4");
    pl->setSource(url);

    // ≈–∂œ”–√ª”–◊¥Ã¨
    if (pl->error() == QMediaPlayer::NoError) {

        QEventLoop* el = new QEventLoop();
        QEventLoop::connect(pl, &QMediaPlayer::mediaStatusChanged, el, &QEventLoop::quit);

        el->exec();    //ø™ º ¬º˛—≠ª∑£¨µ»¥˝Ω· ¯

        delete el;

    }
    
    // v1.1∞Ê±æ–ﬁ∏ƒ
    //bool is_ok = (pl->hasVideo() && (pl->duration() != 0));
    //判断是否有时间信息
    bool is_ok = ((pl->duration() != 0));
    
    if (is_ok){
        
        //获取名称
        QFileInfo fileInfo = QFileInfo(url.path());
        //文件名
        //auto rawFileName = fileInfo.fileName();
        auto rawFileName = fileInfo.baseName();//不带后缀
        
        obj.timeRange_in = 0;
        obj.timeRange_out = pl->duration();
        obj.raw_duration = pl->duration();
        
        obj.setName(rawFileName.toStdString());
        //不要名称
        
        obj.setUrl(url.path().toStdString());
        //设置状态
        obj.set_stateWaiting();
    

        
        
        
        
        
        int raw_duration = pl->duration();
        //this->in_assset->setPosition(this->raw_duration / 2);
        //信息
        auto mediaMetaData = pl->metaData();
        qDebug() << raw_duration,rawFileName;

    }
    
    
    
    
    
    
    

    delete pl;


    return is_ok;
}


QString AVS_Task_QTtool::getShowInfo(AVS_Task* obj){
    
    QString info = "";
    SRTQTime intime(obj->timeRange_in);
    SRTQTime outtime(obj->timeRange_out);

    info += "TimeRange: " + intime.toString("hh:mm:ss") + "<->" + outtime.toString("hh:mm:ss") + "\n";
    
    
//    识别引擎
//    AVS_type Engine;
//    std::vector<AVS_MDXNet_Models_Type> m_mdxnet_type_list;
    QString Engine = "";
    
    
    if (obj->Engine == AVS_type::AVS_MDXNet){
        Engine = "type: " +  QString::number(AVS_type::AVS_MDXNet) + " [";
        
        for(auto i:obj->m_mdxnet_type_list){
            Engine += (" "+ QString::number(i)+",");
            
        }
        Engine +="]\n";
        
        
      
    }
    info += Engine;
    
  
    


    

    
    
    



    return info;
    
}



void AVS_Task_QTtool::showFileFromFinder(QStringList& pathList){
    
    //win
#ifdef Q_OS_WIN
    const QString explorer = "explorer";
    QStringList param;
    for(QString pathIn : pathList){
        if (!QFileInfo(pathIn).isDir())
            param << QLatin1String("/select,");
        param << QDir::toNativeSeparators(pathIn);
        
    }
    QProcess::startDetached(explorer, param);
#endif
    
#ifdef Q_OS_MAC
    
    qDebug()<<"mac";
    const QString explorer = "open";
    QStringList param;
    param << "-R";
    for(QString pathIn : pathList){
        param << pathIn;
    }
    qDebug() << param;
    QProcess::startDetached(explorer, param);
    
    #endif
    
}
