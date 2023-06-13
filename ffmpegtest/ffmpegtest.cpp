#include "ffmpegtest.h"
#include "ui_ffmpegtest.h"
#include <QRegularExpression>
#include <QThread>
#include <QTimer>
#include <QBuffer>
#include "../SR/sr_esrgan_ncnn_strategy.h"


class WorkerThread : public QThread
{
public:
    void run() override
    {
        for (int i = 0; i  < 30; i++ ) {
            
            QThread::msleep(1000);//阻塞延时50ms
            qDebug()<< i;
            
            
            
        }
        
        
    }
};



FFMPEGTest::FFMPEGTest(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FFMPEGTest)
{
    ui->setupUi(this);
    
    ffmpegProcess = new QProcess(this);
    connect(ffmpegProcess, &QProcess::readyReadStandardOutput, this, &FFMPEGTest::onReadyReadStandardOutput);
    connect(ffmpegProcess, &QProcess::readyReadStandardError, this, &FFMPEGTest::onReadyReadStandardError);








}

FFMPEGTest::~FFMPEGTest()
{
    delete ui;
}

void FFMPEGTest::onReadyReadStandardOutput(){
//    QByteArray data = ffmpegProcess->readAllStandardOutput();
//    qDebug() << data.size();
    // 假设视频帧是RGB24格式，分辨率为640x480
//    int frameWidth = 1280;
//    int frameHeight = 720;

    // 计算图像字节数 RGB888 (24)
    int imageSize = frameWidth * frameHeight * 3;

    // 检查数据大小是否正确
//    if (data.size() != imageSize)
//    {
//        qDebug() << "Invalid frame data";
//        return;
//    }
    
//    QString output = QString::fromLocal8Bit(ffmpegProcess->readAllStandardOutput());
//    qDebug() <<output ;
    
    QByteArray newData = ffmpegProcess->readAllStandardOutput();
   
    //应该把一下基本信息排除掉
    videoFrameData.append(newData);

    // 判断视频帧数据是否完整，例如通过某种标志符或帧大小进行判断
    if (videoFrameData.size() % imageSize == 0) {
        qDebug() << videoFrameData.size() << videoFrameData.size()/imageSize;
        writeIndex += 1;
        
        if (readIndex == 0) {
            //执行处理
//            QTimer::singleShot(1, this, &FFMPEGTest::runTask);
        }
        
        
    }
    

   
    
}

void FFMPEGTest::onReadyReadStandardError(){
    QString output = QString::fromLocal8Bit(ffmpegProcess->readAllStandardError());
    qDebug() <<output;
    ui->label->setText(output);
    ui->label->setFixedSize(this->width(), this->height());
    
    // 获取视频长度
    QRegularExpression reDuration("\\sDuration: (\\d\\d):(\\d\\d):(\\d\\d\\.\\d\\d)");
    QRegularExpressionMatch matchDuration = reDuration.match(output);
    if (matchDuration.hasMatch()) {
        int hours = matchDuration.captured(1).toInt();
        int minutes = matchDuration.captured(2).toInt();
        float seconds = matchDuration.captured(3).toFloat();
        float durationInSeconds = hours * 3600 + minutes * 60 + seconds;
        
        framess = durationInSeconds * 1000;
        
        qDebug() << "Duration: " << durationInSeconds << " seconds";
    }

    // 获取视频宽度和高度
    QRegularExpression reResolution("\\sStream.*Video:.*\\b(\\d+)x(\\d+)\\b");
    QRegularExpressionMatch matchResolution = reResolution.match(output);
    if (matchResolution.hasMatch()) {
        int width = matchResolution.captured(1).toInt();
        int height = matchResolution.captured(2).toInt();
        
        frameWidth = width;
        frameHeight = height;
        
        
        qDebug() << "Resolution: " << width << "x" << height;
    }

    // 获取视频帧数
    QRegularExpression reFrameCount("\\sStream.*Video:.*\\b(\\d+)\\s+fps\\b");
    QRegularExpressionMatch matchFrameCount = reFrameCount.match(output);
    if (matchFrameCount.hasMatch()) {
        float frameCount = matchFrameCount.captured(1).toFloat();
        frameFps = frameCount;
        qDebug() << "Frame Count: " << frameCount;
    }
    
//    开始解码
    
    
    
    
    
    
    

    QRegularExpression re("\\stime=([\\d:.]+)");
    QRegularExpressionMatch match = re.match(output);
    if (match.hasMatch()) {
        QString timeStr = match.captured(1);
        // 进度百分比解析逻辑
        // ...
    }
    
    
//
//    int frameWidth = 1280;
//    int frameHeight = 720;
//
//    // 计算图像字节数
//    int imageSize = frameWidth * frameHeight * 3;
//
//    // 创建图像对象
//    QImage image(frameWidth, frameHeight, QImage::Format_RGB888);
//
//    // 将数据复制到图像对象中
//    memcpy(image.bits(), videoFrameData.data(), imageSize);
//
//    // 将图像显示在QLabel上
//    ui->label->setPixmap(QPixmap::fromImage(image));
//    ui->label->setFixedSize(frameWidth, frameHeight);
//
//
//
//    // 清空已处理的视频帧数据
//    videoFrameData.clear();
    
    
    
    
    
    
    
}

void FFMPEGTest::runTask(){
    //开始处理帧
    int imageSize = frameWidth * frameHeight * 3;
    for (int i = 0; i  < 30*frameFps; i++ ) {
        
        
        //为假不允许循环
        while (readIndex <= writeIndex) {
            QThread::msleep(500);//阻塞延时50ms
        }
        
        // 创建图像对象
        QImage image(frameWidth, frameHeight, QImage::Format_RGB888);

        // 将数据复制到图像对象中
        memcpy(image.bits(), &videoFrameData[(i)*imageSize], imageSize);

        // 将图像显示在QLabel上
        ui->label->setPixmap(QPixmap::fromImage(image));
        ui->label->setFixedSize(frameWidth, frameHeight);
        QThread::msleep(1000);//阻塞延时50ms
        
        
        
    }
    
    
}

void FFMPEGTest::on_pushButton_clicked()
{
    
    
    SR_ESRGAN_ncnn_Strategy sr_obj;
    return;
    
    
//    readIndex = 0;
//    writeIndex = 0;
////    runFFMPEG(0,30*1000);
//
//
//
//    // 创建 WorkerThread 对象
//    WorkerThread* workerThread = new WorkerThread();
//
//    // 启动后台线程
//    workerThread->start();
    
//    测试新的
    
    QString appPath = QCoreApplication::applicationDirPath();
    QString macosPath = appPath + "/../Resources/realesrgan-ncnn-vulkan";
    
    



////    QString command = command_ffmpeg ;// + "";  // 以命令行方式执行的FFmpeg命令
//    QStringList arguments;
//    arguments << "-i" ;
//    ffmpegProcess->start(macosPath, arguments);
    
    
    
    // 读取输入图像
       QString inputImagePath = "/Users/linximing/Desktop/AppIcon.png";
       QString outputImagePath = "/Users/linximing/Downloads/realesrgan-ncnn-vulkan-20220424-macos/output2.png";
       QImage inputImage(inputImagePath);
       if (inputImage.isNull()) {
           qWarning("Failed to load input image.");
           return ;
       }

       // 将输入图像转换为PNG编码
       QByteArray pngData;
       QBuffer buffer(&pngData);
       buffer.open(QIODevice::WriteOnly);
       inputImage.save(&buffer, "PNG");
       buffer.close();

       // 调用 realesrgan-ncnn-vulkan 进行处理
       QString realesrganPath = macosPath;
       QStringList arguments;
       arguments << "-i" << inputImagePath;
       arguments << "-o" << outputImagePath;
       arguments << "-m" << "/Users/linximing/Movies/UVAIGC/SRModel/realesrmodels";
       arguments << "-n" << "realesrgan-x4plus";
    ffmpegProcess->start(macosPath, arguments);
    

//       QProcess process;
//       process.start(realesrganPath, arguments);
//
//       // 将转换后的PNG编码写入 realesrgan-ncnn-vulkan 的标准输入
//       process.write(pngData);
//       process.closeWriteChannel();
//
//       // 读取处理后的图像数据
//       QByteArray processedImageData = process.readAllStandardOutput();
//
//       // 创建输出图像
//       QImage outputImage;
//       outputImage.loadFromData(processedImageData, "PNG");
//
//       // 在 QLabel 中展示输出图像
////       QLabel label;
////       label.setPixmap(QPixmap::fromImage(outputImage));
//
//
//    ui->label->setPixmap(QPixmap::fromImage(outputImage));
////    ui->label->setFixedSize(frameWidth, frameHeight);
//
//    qDebug()<< "完成";
    

    
    
    
    
    
    
    
    
    
    
    
    


}



void FFMPEGTest::runFFMPEG(long intime ,long out_time){
    
    if (intime == 0 && out_time ==0) {
        
    }
    
    
    videoFrameData.clear();
    QString appPath = QCoreApplication::applicationDirPath();
    QString macosPath = appPath + "/../Resources/ffmpeg";



//    QString command = command_ffmpeg ;// + "";  // 以命令行方式执行的FFmpeg命令
    QStringList arguments;
    arguments << "-i" << "/Users/linximing/Downloads/下载2.mp4" << "-ss" <<  QString("%1").arg(intime/1000.0)<<"-t" << QString("%1").arg((out_time - intime)/1000.0)<< "-f" << "image2pipe" << "-pix_fmt" << "rgb24" << "-vcodec" << "rawvideo" << "-";
    ffmpegProcess->start(macosPath, arguments);
    
    
    
    
    
}

