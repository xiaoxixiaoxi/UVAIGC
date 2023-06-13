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
#include <QDesktopServices>
#include <QDir>
#include <QSettings>
#include <QMediaMetaData>
#include "../Basis/FFMPEG_Strategy.hpp"

#include "../SettingSpanel/Settings_info.hpp"
#include "../SR/SR_type.h"
#include <QVideoSink>

// 判断是否可以导入
bool AVS_Task_QTtool::if_hasVideo(AVS_Task& obj ,QUrl url)
{
    QMediaPlayer* pl = new QMediaPlayer();
    QVideoSink qvsik;
    pl->setVideoSink(&qvsik);

    //QUrl url = QUrl::fromLocalFile("D:/tst.mp4");
    pl->setSource(url);

    // ≈–∂œ”–√ª”–◊¥Ã¨
    if (pl->error() == QMediaPlayer::NoError) {

        QEventLoop* el = new QEventLoop();
        QEventLoop::connect(pl, &QMediaPlayer::mediaStatusChanged, el, &QEventLoop::quit);
        pl->play();

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
        
        obj.setUrl(url.toLocalFile().toStdString());
//        qDebug() << "8888" << url;
        //设置状态
        obj.set_stateWaiting();
        
        //判断类型
        
        // 判断媒体类型
        qDebug() << "载入" << qvsik.videoSize();
        if (qvsik.videoSize().width() > 0 && qvsik.videoSize().height() > 0) {
            obj.media_type =TaskBasis::Video;
        }else{
            obj.media_type =TaskBasis::Audio;
        }
       
        
        
        //添加输出格式逻辑处理
        QSettings settings = QSettings(ORGANIZATION_Name, APPLICATION_Name);
        
        std::string suffix = settings.value(QSettings_KEY_AVS_OutType_suffix).toString().toStdString();
        if (suffix == "default" || suffix ==""){
            //跟随输入
            obj.outType = fileInfo.suffix().toStdString();
        }else{
            obj.outType = suffix;
        }
        
        
    

//        qDebug() <<
        
        
        
        
        int raw_duration = pl->duration();
        //this->in_assset->setPosition(this->raw_duration / 2);
        //信息
        auto mediaMetaData = pl->metaData();
        qDebug() << raw_duration,mediaMetaData;

    }
    
    
    
    
    
    
    

    delete pl;


    return is_ok;
}

bool AVS_Task_QTtool::if_hasVideoV2(AVS_Task& obj ,QUrl url)
{
    //改用ffmpeg 判断是否有时间 有就能导入  再判断尺寸
    QSize size = QSize(0,0);
    long duration = 0;

    //排除图片
    //先判断是不是图片
    QImage image(url.toLocalFile());
    if (!image.isNull()) {
        //是图片
        return false;
    }

    FFMPEG_Strategy ffmpeg_obj;

    if (!ffmpeg_obj.loadFile_sync(url.toLocalFile())){
        //不支持 直接返回
        return false;
    }

    size = ffmpeg_obj.getSize();
    duration = ffmpeg_obj.getDuration();

    //判断是否有时间信息
    bool is_ok = ((duration > 0));

    if (is_ok){

        //获取名称
        QFileInfo fileInfo = QFileInfo(url.path());
        //文件名
        //auto rawFileName = fileInfo.fileName();
        auto rawFileName = fileInfo.baseName();//不带后缀

        obj.timeRange_in = 0;
        obj.timeRange_out = duration;
        obj.raw_duration = duration;

        obj.setName(rawFileName.toStdString());
        //不要名称

        obj.setUrl(url.toLocalFile().toStdString());
//        qDebug() << "8888" << url;
        //设置状态
        obj.set_stateWaiting();

        //判断类型

        // 判断媒体类型
//        qDebug() << "载入" << qvsik.videoSize();
        if ( size.width() > 0 &&  size.height() > 0) {
            obj.media_type =TaskBasis::Video;
        }else{
            obj.media_type =TaskBasis::Audio;
        }



        //添加输出格式逻辑处理
        QSettings settings = QSettings(ORGANIZATION_Name, APPLICATION_Name);

        std::string suffix = settings.value(QSettings_KEY_AVS_OutType_suffix).toString().toStdString();
        if (suffix == "default" || suffix ==""){
            //跟随输入
            obj.outType = fileInfo.suffix().toStdString();
        }else{
            obj.outType = suffix;
        }

    }


    return is_ok;
}



// 判断是否可以导入 只接受图片和视频。用ffmpeg
bool AVS_Task_QTtool::SR_if_hasVideo(SR_Task& obj ,QUrl url)
{
    
    QSize size = QSize(0,0);
    long duration = 0;
    SR_Task::Media_type type = SR_Task::Image;
    
    //先判断是不是图片
    QImage image(url.toLocalFile());
    if (image.isNull()) {
        // 加载失败，文件不是图片
        qDebug() << "File is not an image";
        
        FFMPEG_Strategy ffmpeg_obj;
        
        if (!ffmpeg_obj.loadFile_sync(url.toLocalFile())){
            //不支持 直接返回
            return false;
        }
        
        size = ffmpeg_obj.getSize();
        duration = ffmpeg_obj.getDuration();
        if(size.width()<=0 || size.height() <= 0 || duration <= 0){
            //可能是音频 或者其他的
            return false;
        }
        
        
        type = SR_Task::Video;
        
        obj.setThumbnailImage(ffmpeg_obj.ThumbnailImage(0, 0));
        
        
    } else {
        // 加载成功，文件是图片
        qDebug() << "File is an image";
        size =  QSize(image.width(),image.height());
        
        type = SR_Task::Image;
        obj.setThumbnailImage(image.scaled(160, 90,Qt::KeepAspectRatio, Qt::SmoothTransformation));
        
        
    }
    
    
    //获取名称
    QFileInfo fileInfo = QFileInfo(url.path());
    //文件名
    //auto rawFileName = fileInfo.fileName();
    auto rawFileName = fileInfo.baseName();//不带后缀

    obj.timeRange_in = 0;
    obj.timeRange_out =duration;
    obj.raw_duration = duration;

    obj.setName(rawFileName.toStdString());
    //不要名称

    obj.setUrl(url.toLocalFile().toStdString());
    //        qDebug() << "8888" << url;
    //设置状态
    obj.set_stateWaiting();
    
    //TODO: 设置类型
    obj.setMedia_type(type);
    //大小
    obj.setSize(size);
    
    
    
    
    //从设置窗口读出配置
    //1,引擎类型 2,模型选择名称 3,倍数匹配 4,模型路径? 5,视频输出类型 6,图片输出类型
    
    QSettings settings = QSettings(ORGANIZATION_Name, APPLICATION_Name);
    
    obj.sr01_model_path_dir = settings.value(QSettings_KEY_SR_model_save_path).toString();
    obj.sr01_model_name = settings.value(QSettings_KEY_SR01_selection_model_name).toString();
//    obj.selection =
    
    
    
    if (obj.media_type == SR_Task::Media_type::Video){
        std::string suffix = settings.value(QSettings_KEY_SR_Video_OutType).toString().toStdString();
        
        if (suffix == "default" || suffix ==""){
            obj.outType = fileInfo.suffix().toStdString();
        }else{
            obj.outType = suffix;
        }
        
        //如果是跟随 处理
       
        
    }else if (obj.media_type == SR_Task::Media_type::Image)
    {
        std::string  suffix = settings.value(QSettings_KEY_SR_Image_OutType).toString().toStdString();
        if (suffix == "jpg"){
           
        }else{
            suffix = "png";
        }
        
        obj.outType = suffix;
        
    }
    
    //获取有添加引擎需要在这边分离
    obj.Engine = SR_Real_ESRGAN_NCNN_VULKANT;
    
    

    
    
    
    
   
    
  
    










    return true;
}
//显示文字信息 不同任务显示不同信息
QString AVS_Task_QTtool::getShowInfo(AVS_Task* obj){
    
    QString info = "";
    SRTQTime intime(obj->timeRange_in);
    SRTQTime outtime(obj->timeRange_out);

    info += "TimeRange: " + intime.toString("hh:mm:ss") + "<->" + outtime.toString("hh:mm:ss") + "\n";
    
    QString media = "";
    if (obj->media_type ==TaskBasis::Video){
        media = "video";
    }else if (obj->media_type == TaskBasis::Audio){
        media = "audio";
    }
    info += "media: " + media + "\n";
        
    
    
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
    
  
    //输出格式
    info += "out type: " + QString::fromStdString(obj->outType)  + "\n";


    

    
    
    



    return info;
    
}

QString AVS_Task_QTtool::getShowInfo(TaskBasis* obj){
    return getShowInfo(obj);
}

QString AVS_Task_QTtool::getShowInfo(SR_Task* obj){
    
    QString info = "";
    SRTQTime intime(obj->timeRange_in);
    SRTQTime outtime(obj->timeRange_out);
    
    
    //媒体类型
    QString media = "";
    if (obj->media_type ==  SR_Task::Video){
        media = "video";
        
        info += "TimeRange: " + intime.toString("hh:mm:ss") + "<->" + outtime.toString("hh:mm:ss") + "\n";
        
    }else if (obj->media_type ==  SR_Task::Image){
        media = "image";
    }
    info += "media: " + media + "\n";

   
    
    
//    识别引擎
//    AVS_type Engine;
//    std::vector<AVS_MDXNet_Models_Type> m_mdxnet_type_list;
    QString Engine = "";
    
    
    
    if (obj->Engine == SR_type::SR_Real_ESRGAN_NCNN_VULKANT){
        Engine = "type: " +  QString::number(SR_Real_ESRGAN_NCNN_VULKANT) + " [";
        
        
        Engine += obj->sr01_model_name;
//        for(auto i:obj->m_mdxnet_type_list){
//            Engine += (" "+ QString::number(i)+",");
//            
//        }
        Engine +="]\n";
        
        //名称
        
        
        
        
        
      
    }
    info += Engine;
    
    //显示倍数
    std::string key = obj->sr01_model_name.toStdString();
    auto it = SR_ESRGAN_NCNN_Models_Scaled_DICTIONARY.find(key);
    if (it != SR_ESRGAN_NCNN_Models_Scaled_DICTIONARY.end()) {
        // 键存在，可以访问对应的值
        const std::vector<int>& values = it->second;
        info += "scaled: " + QString::number(values[0]) + "\n";

    } else {
        // 键不存在的情况
        // ...
    }

    
   
    
    //输出格式
    info += "out type: " + QString::fromStdString(obj->outType)  + "\n";
    
    
  
    


    

    
    
    



    return info;
    
}


void AVS_Task_QTtool::showFileFromFinder(QStringList& pathList){

//    for (const QString& filePath : pathList) {
//        QUrl fileUrl = QUrl::fromLocalFile(filePath);
//        QDesktopServices::openUrl(fileUrl.adjusted(QUrl::RemoveFilename));

//    }
    
    //win
#ifdef Q_OS_WIN
    //TODO: 用qt接口
    const QString explorer = "explorer";
    QStringList param;
    for(QString& pathIn : pathList){

        QUrl fileUrl = QUrl::fromLocalFile(pathIn);
         qDebug() << pathIn << fileUrl << fileUrl.toLocalFile();
        if (!QFileInfo(fileUrl.toLocalFile()).isDir())
            param << QLatin1String("/select,");
        param << QDir::toNativeSeparators(fileUrl.toLocalFile());
        
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
