//
//  FFMPEG_Strategy.cpp
//  UVAIGC
//
//  Created by 林溪明 on 2023/6/6.
//

#include "FFMPEG_Strategy.hpp"
#include <QCoreApplication>
#include <QRegularExpression>
#include <QImage>

FFMPEG_Strategy::FFMPEG_Strategy(QObject *parent)
    : QObject{parent}
{


    ffmpegProcess = new QProcess(this);
    
    connect(ffmpegProcess, &QProcess::readyReadStandardError, this, &FFMPEG_Strategy::onReadyReadStandardError);
    connect(ffmpegProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [=](){
        qDebug() << "ffmpeg进程完成";
        emit finished();
    });
    
    
    bin_url = "";
    QString appPath = QCoreApplication::applicationDirPath();
    //TODO: 根据不同平台加载bin路径
    bin_url = appPath + "/../Resources/ffmpeg";
    
    
    //TODO: 判断文件是否存在
    
//    test();

}
FFMPEG_Strategy::~FFMPEG_Strategy(){
    
}



void FFMPEG_Strategy::cancel(){
    
    // 在某个时刻需要取消进程
    if (ffmpegProcess->state() == QProcess::Running) {
        ffmpegProcess->kill();
        ffmpegProcess->waitForFinished();  // 等待进程完全终止
    }
    
}

void FFMPEG_Strategy::onReadyReadStandardError(){
    
    QString output = QString::fromLocal8Bit(ffmpegProcess->readAllStandardError());
//    qDebug() <<output;
    
    // 获取视频长度
    QRegularExpression reDuration("\\sDuration: (\\d\\d):(\\d\\d):(\\d\\d\\.\\d\\d)");
    QRegularExpressionMatch matchDuration = reDuration.match(output);
    if (matchDuration.hasMatch()) {
        int hours = matchDuration.captured(1).toInt();
        int minutes = matchDuration.captured(2).toInt();
        float seconds = matchDuration.captured(3).toFloat();
        float durationInSeconds = hours * 3600 + minutes * 60 + seconds;
        
        framess = durationInSeconds * 1000;
        
//        qDebug() << "Duration: " << durationInSeconds << " seconds";
    }
    
    
    QRegularExpression t_reDuration("\\stime=(\\d\\d):(\\d\\d):(\\d\\d\\.\\d\\d)");
    QRegularExpressionMatch t_matchDuration = t_reDuration.match(output);
    if (t_matchDuration.hasMatch()) {
        int hours = t_matchDuration.captured(1).toInt();
        int minutes = t_matchDuration.captured(2).toInt();
        float seconds = t_matchDuration.captured(3).toFloat();
        float durationInSeconds = hours * 3600 + minutes * 60 + seconds;
        
        float jd = (durationInSeconds * 1000)/framess;
        
        emit progress(jd,tag);
//        qDebug() << "进度" << jd ;
    }
    
    
    QRegularExpression successRegex("Output \\#\\d+, (.+)");
    QRegularExpressionMatch successMatch = successRegex.match(output);

    if (successMatch.hasMatch()) {
        QString outputPath = successMatch.captured(1);
//        qDebug() << "转换成功，输出路径：" << outputPath;
        
        //标记成功
        is_finish = true;
    }
    
    

 
    
    
   
    
    //完成信号
    
   
}

//void SR_ESRGAN_ncnn_Strategy::test(){
//
//    QString inputImagePath = "/Users/linximing/Desktop/AppIcon.png";
//    QString outputImagePath = "/Users/linximing/Downloads/realesrgan-ncnn-vulkan-20220424-macos/output2.png";
//
//    QStringList arguments;
//    arguments << "-i" << inputImagePath;
//    arguments << "-o" << outputImagePath;
//    arguments << "-m" << "/Users/linximing/Movies/UVAIGC/SRModel/realesrmodels";
//    arguments << "-n" << "realesrgan-x4plus";
//    esrganProcess->start(bin_url, arguments);
//
//
//
//}


void FFMPEG_Strategy::runProcess(QStringList incmd,int _tag){
    //
    //标记还没有完成
    is_finish = false;
    tag = _tag;
    
    ffmpegProcess->start(bin_url, incmd);
    //-1 永不超时
    ffmpegProcess->waitForFinished(-1);
    
    
}





//
bool FFMPEG_Strategy::loadFile_sync(QString url_path){
    
    QProcess ls_ffmpegProcess;
    QStringList arguments;
    arguments << "-i" << url_path;
    
    arguments << "-vf" << "select='eq(n,1)',scale=w=160:h=90" << "-vframes" << "1";  // 获取中间一帧
    arguments << "-f" << "image2pipe" << "-pix_fmt" << "rgb24" << "-vcodec" << "rawvideo";  
    arguments << "-";  // 输出到标准输出（管道）

    ls_ffmpegProcess.start(bin_url, arguments);
    //获取返回
    // 等待进程完成
    if (ls_ffmpegProcess.waitForFinished(-1)) {
        // 进程完成后的处理逻辑
        // 获取进程的输出
        QByteArray output =  ls_ffmpegProcess.readAllStandardError();
        ffmoeg_info_out = QString::fromUtf8(output);
        
        qDebug() << ffmoeg_info_out;
        
        //获取图片质量
        
        ThumbnailData = ls_ffmpegProcess.readAllStandardOutput();  // 读取标准输出中的图像数据
//        QImage image;
        
        
        

    } else {
        return false;
    }
    
    return true;
}

QSize FFMPEG_Strategy::getSize(){
    
    
//    QStringList lines = ffmoeg_info_out.split('\n');

    int width = -1;
    int height = -1;
    
    // 提取宽度和高度信息
    QRegularExpression re("\\sStream.*Video:.*\\b(\\d+)x(\\d+)\\b");
    QRegularExpressionMatch match = re.match(ffmoeg_info_out);
    if (match.hasMatch()) {
        width = match.captured(1).toInt();
        height = match.captured(2).toInt();

    }
    


    qDebug() << "Width: " << width;
    qDebug() << "Height: " << height;
    
    return QSize(width,height);

    
}



long FFMPEG_Strategy::getDuration(){
    
    long time = -1;
    
    // 提取宽度和高度信息
    QRegularExpression reDuration("\\sDuration: (\\d\\d):(\\d\\d):(\\d\\d\\.\\d\\d)");
    QRegularExpressionMatch matchDuration = reDuration.match(ffmoeg_info_out);
    if (matchDuration.hasMatch()) {
        int hours = matchDuration.captured(1).toInt();
        int minutes = matchDuration.captured(2).toInt();
        float seconds = matchDuration.captured(3).toFloat();
        float durationInSeconds = hours * 3600 + minutes * 60 + seconds;
        
        time = durationInSeconds * 1000;
    }

    


    qDebug() << "Duration: " << time;
    
    return time;
    
}



QImage FFMPEG_Strategy::ThumbnailImage(int w,int h)
{
    // 创建图像对象
    QImage image(160, 90, QImage::Format_RGB888);

    // 将数据复制到图像对象中
    memcpy(image.bits(), ThumbnailData.data(), ThumbnailData.size());
    
    return image;
    
}


void FFMPEG_Strategy::loadFfmpegBin(){
    
    QProcess ls_ffmpegProcess;
    QStringList arguments;
    arguments << "-version";
//
//    arguments << "-vf" << "select='eq(n,1)',scale=w=160:h=90" << "-vframes" << "1";  // 获取中间一帧
//    arguments << "-f" << "image2pipe" << "-pix_fmt" << "rgb24" << "-vcodec" << "rawvideo";
//    arguments << "-";  // 输出到标准输出（管道）
    ls_ffmpegProcess.start(bin_url, arguments);
    // 等待进程完成
    if (ls_ffmpegProcess.waitForFinished(-1)) {
        // 进程完成后的处理逻辑
        // 获取进程的输出
        QByteArray output =  ls_ffmpegProcess.readAllStandardError();
//        qDebug() << "xxxxxxuuu" << QString::fromUtf8(output);
    }
    
}
