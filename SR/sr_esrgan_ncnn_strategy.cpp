#include "sr_esrgan_ncnn_strategy.h"
#include <QDebug>
#include <QCoreApplication>
#include <QRegularExpression>
#include <QFile>
#include <QTemporaryDir>
#include "../Basis/OperationCanceledException.h"
#include <QImage>


SR_ESRGAN_ncnn_Strategy::SR_ESRGAN_ncnn_Strategy(QObject *parent)
    : QObject{parent},SR_TaskProcessingStrategy()
{
    // 类型
    type = SR_Real_ESRGAN_NCNN_VULKANT;

    qDebug() << "加载SR模块";
    
   
    esrganProcess = new QProcess(this);
    
    connect(esrganProcess, &QProcess::readyReadStandardError, this, &SR_ESRGAN_ncnn_Strategy::onReadyReadStandardError);
    connect(esrganProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [=](){
        qDebug() << "SR进程完成";
        emit finished();
    });
        
        
        
    ffmpeg_obj  = new  FFMPEG_Strategy(this);
        
      //进度计算
    connect(ffmpeg_obj,&FFMPEG_Strategy::progress,this,[=](float jd ,int tag){
        //
        if (tag == 1){
            ffmpeg_load_jd = jd;
            
        }else if (tag == 2){
            ffmpeg_out_jd = jd;
        }
//        qDebug() << "进d: " << jd << tag;
        setProgress();
        
    });
    
    
    
    
   
    
    bin_url = "";
    QString appPath = QCoreApplication::applicationDirPath();
    //TODO: 根据不同平台加载bin路径
    bin_url = appPath + "/../Resources/realesrgan-ncnn-vulkan";
//    bin_url = "/Users/linximing/Documents/du/xiaoxi/qt/UVAIGC/Lib/MACLib/realesrgan/未签名/realesrgan-ncnn-vulkan";
    
    
    //TODO: 判断文件是否存在
    
//    test();
    
    
    
    

}


SR_ESRGAN_ncnn_Strategy::~SR_ESRGAN_ncnn_Strategy(){
    //释放
    delete esrganProcess;
    delete ffmpeg_obj;
    
    if (this->tempDir != nullptr){
        delete tempDir;
        qDebug() << "释放SR_ESRGAN_ncnn_Strategy";
    }
    
    
    
    if (this->outTempDir != nullptr){
        delete outTempDir;
    }
    
    
    
    
}


void SR_ESRGAN_ncnn_Strategy::cancel(){
    
    // 在某个时刻需要取消进程
    if (esrganProcess->state() == QProcess::Running) {
        esrganProcess->kill();
        esrganProcess->waitForFinished();  // 等待进程完全终止
    }
    
}

void SR_ESRGAN_ncnn_Strategy::onReadyReadStandardError(){
    
    QString output = QString::fromLocal8Bit(esrganProcess->readAllStandardError());
//    qDebug() <<output;
    

    
    
    //解析进度条
    QRegularExpression re("(\\d+\\.\\d+)%");
    QRegularExpressionMatch match = re.match(output);

    if (match.hasMatch()) {
        QString progressStr = match.captured(1);
        float progress_f = progressStr.toFloat();
        if (progress_f == 0){
            esrgan_frames_number_now += 1;
        }
        //根据总张数 和0 为第一个判断个数来统计
        float bl = (esrgan_frames_number_now-1)/(esrgan_frames_number_all*1.0);
       
        float progress_f2 = bl + ((progress_f)/100)/(esrgan_frames_number_all);
        
        //更新进度到对象
        esrgan_jd = progress_f2;
        setProgress();
//        emit progress(progress_f2);
//        qDebug() << "Progress:" << progress_f2 ;

        
    }
    
   
}

void SR_ESRGAN_ncnn_Strategy::test(){
    
    
    
//    QStringList arguments;
//    //arguments << "-i" << "/private/var/folders/vd/1x31td3j3ln33pt2y6sq03l40000gn/T/UVAIGC-CWYckX/frame00000001.jpg";
//    arguments << "-i" << "/Users/linximing/Downloads/realesrgan-ncnn-vulkan-20220424-macos/input.jpg";
//    //arguments << "-o" << "/private/var/folders/vd/1x31td3j3ln33pt2y6sq03l40000gn/T/UVAIGC-OyUoAx/frame00000001.png";
//    arguments << "-o" << "/Users/linximing/Downloads/realesrgan-ncnn-vulkan-20220424-macos/out.png";
//    arguments << "-m" << "/Users/linximing/Movies/UVAIGC/SRModel/realesrmodels";
//    arguments << "-n" << "realesrgan-x4plus";
////    arguments << "-v" ;
////    arguments << "-f" << "jpg";
//    esrganProcess->start(bin_url, arguments);
//
//    esrganProcess->waitForFinished();
//    return;
    
    
    
    
    
    
    
    
    
    //QString inputImagePath = "/Users/linximing/Desktop/AppIcon.png";
    QString inputImagePath = "/Users/linximing/Downloads/realesrgan-ncnn-vulkan-20220424-macos/0秒.mp4";
    QString outputImagePath = "/Users/linximing/Downloads/realesrgan-ncnn-vulkan-20220424-macos/output2.png";
    
//    QStringList arguments;
//    arguments << "-i" << inputImagePath;
//    arguments << "-o" << outputImagePath;
//    arguments << "-m" << "/Users/linximing/Movies/UVAIGC/SRModel/realesrmodels";
//    arguments << "-n" << "realesrgan-x4plus";
//    esrganProcess->start(bin_url, arguments);
    
    SR_Task tetsobj = SR_Task();
    tetsobj.setUrl(inputImagePath.toStdString());
    tetsobj.setMedia_type(SR_Task::Media_type::Video);
    tetsobj.out_path_dir = "/Users/linximing/Movies/UVAIGC/out";
    processTask(tetsobj);
    
    
//    file_loadProcess(tetsobj);
    
    
    
    
}
//文件处理
void SR_ESRGAN_ncnn_Strategy::file_loadProcess(SR_Task& avsTask){
    //新开始 统计进度用
    esrgan_frames_number_all = 0;
    esrgan_frames_number_now = 0;
    
    QString raw_path = QString::fromStdString(avsTask.getUrl()) ;
    SR_Task::Media_type media_type = avsTask.media_type;
    //判断文件是否存在 不存在返回
    //创建临时文件夹
    //判断是不是视频 如果是image 图片拷贝原路径到文件夹 如何是video 进场ffmpeg处理
    //返回创建的临时文件夹路径
    
    // 判断文件是否存在
   if (!QFile::exists(raw_path)) {
       qDebug() << "File not found: " << raw_path;
       return ; // 返回空路径表示文件不存在
   }
    
    
    if (this->tempDir !=nullptr){
        delete tempDir;
    }else{
        tempDir =  new QTemporaryDir();
    }

   QString temp_folder =  tempDir->path(); // 创建临时文件夹
    qDebug() << "临时文件夹" << temp_folder;
   
   if (media_type == SR_Task::Media_type::Image) {
       // 如果是图片，直接拷贝原路径到临时文件夹
       QString target_path = temp_folder + "/" + QFileInfo(raw_path).fileName();
       
       // 加载原始图片
       QImage image(raw_path);
       if (image.isNull()) {
           // 加载失败，处理错误
           avsTask.set_stateError();
           QString info = tr("不支持图片格式");
           avsTask.setOutInfo(info.toStdString());
       } else {
           // 保存为PNG格式的图像文件
           if (!image.save(target_path, "PNG")) {
               // 保存失败，处理错误
               avsTask.set_stateError();
               QString info = tr("不支持图片格式");
               avsTask.setOutInfo(info.toStdString());
               
           } else {
               // 保存成功

           }
       }
       
//       if (!QFile::copy(raw_path, target_path)) {
//           qDebug() << "Failed to copy image file: " << raw_path;
//           return ; // 返回空路径表示文件处理失败
//       }
//
       esrgan_frames_number_all = 1;
   }
   else if (media_type == SR_Task::Media_type::Video) {
       // 如果是视频，使用FFmpeg进行处理
       QString output_folder = temp_folder + "/frames";
       //ffmpeg处理  需要封装成一个类
       // 准备好参数
       QStringList cmd;
       cmd << "-ss" << QString::number(avsTask.timeRange_in/1000.0,'f',3) ;
       cmd << "-to" << QString::number(avsTask.timeRange_out/1000.0,'f',3);
       cmd << "-i" << raw_path ;
       cmd << "-qscale:v" << "1" ;
       cmd << "-qmin" << "1" ;
       cmd << "-qmax" << "1" ;
       cmd << "-vsync" << "0" ;
       cmd << temp_folder + "/frame%08d.jpg" ;
       
       
       //绑定信号然后等待完成
       ffmpeg_obj->runProcess(cmd,1);
       
       if (!ffmpeg_obj->getIs_finish()){
           // 没有完成处理
           avsTask.set_stateError();
           avsTask.setOutInfo(tr("错误!视频文件加载失败").toStdString());
       }
       
       
//       delete tempDir;
       //获取文件夹下面有几个文件
       QDir folder(temp_folder);
       QFileInfoList fileInfoList = folder.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
       esrgan_frames_number_all = fileInfoList.count();
       
       

   }
   else {
       qDebug() << "Unsupported media type.";
       return ; // 返回空路径表示不支持的媒体类型
   }
    
//    准备临时的输出文件夹
    
    if (this->outTempDir !=nullptr){
        delete outTempDir;
    }else{
        outTempDir =  new QTemporaryDir();
    }
    qDebug() << "临时输出文件夹" <<  outTempDir->path();
    
    
   
//   return temp_folder; // 返回创建的临时文件夹路径
    
    
    
}

//文件处理
QString SR_ESRGAN_ncnn_Strategy::file_outProcess(SR_Task& avsTask){
    
    QString out_path_dir = QString::fromStdString(avsTask.out_path_dir) ;
    SR_Task::Media_type media_type = avsTask.media_type;
    
    QString raw_path = QString::fromStdString(avsTask.getUrl()) ;
    
    std::string outpath = avsTask.getAvailableFilePath(avsTask.out_path_dir, avsTask.getName(), avsTask.outType);
    
    if (media_type == SR_Task::Media_type::Image) {
        
        
        QString fileName = QFileInfo(raw_path).fileName();
        QString baseName = fileName.section(".", 0, -2);  // 获取不带后缀的文件名
        
        QString sourceFilePath = outTempDir->path() + "/" + baseName +"."+ "png";

        QString destinationFilePath = QString::fromStdString(outpath);
        
        //如果是png 直接拷贝。不是处理一下
        if (avsTask.outType == "png"){
            // 移动文件
            QFile::rename(sourceFilePath, destinationFilePath);
        }else{
            
            QImage out_image( sourceFilePath);
            if (out_image.isNull()) {
                // 加载失败，处理错误
                // ...
            } else {
                // 保存为PNG格式的图像文件
                if (!out_image.save(destinationFilePath)) {
                    // 保存失败，处理错误
                    // ...
                } else {
                    // 保存成功
                    // ...
                }
            }
            
            
        }
        
     
       


    }
    else if (media_type == SR_Task::Media_type::Video) {
        
        //    ffmpeg -i out_frames/frame%08d.jpg -i onepiece_demo.mp4 -map 0:v:0 -map 1:a:0 -c:a copy -c:v libx264 -r 23.98 -pix_fmt yuv420p output_w_audio.mp4
        // 如果是视频，使用FFmpeg进行处理
//        QString output_folder = temp_folder + "/frames";
        //输出格式
//        std::string suffix = "mp4";
//        if
        
        //ffmpeg处理  需要封装成一个类
        // 准备好参数 输出顺序不要弄错
        QStringList cmd;
        cmd << "-i" << outTempDir->path() + "/frame%08d.jpg" ;
        cmd << "-ss" << QString::number(avsTask.timeRange_in/1000.0,'f',3) ;
        cmd << "-to" << QString::number(avsTask.timeRange_out/1000.0,'f',3);
        cmd << "-i" << raw_path ;
        cmd << "-map" << "0:v:0" ;
        cmd << "-map" << "1:a:0" ;
        cmd << "-c:a" << "copy" ;
//        cmd << "-c:v" << "0" ;
        cmd << "-pix_fmt" << "yuv420p" ;
        cmd << QString::fromStdString(outpath) ;
//        qDebug()<< cmd;
        
        //绑定信号然后等待完成
        ffmpeg_obj->runProcess(cmd,2);
        
        if (!ffmpeg_obj->getIs_finish()){
            // 没有完成处理
            avsTask.set_stateError();
            avsTask.setOutInfo(tr("错误!视频文件合成失败").toStdString());
        }
        
        
        

    }
    
    
    
    //验证输出文件是否存在。没有提示失败
    
    QFile file(QString::fromStdString(outpath));
    if (file.exists()) {
        
        avsTask.out_path_list.clear();
        avsTask.out_path_list.push_back(outpath);

    } else {
        //标记错误
        avsTask.set_stateError();
        avsTask.setOutInfo(tr("错误!最后处理失败").toStdString());

    }
    

    
    return "";
    
 
    
    
}

//处理逻辑
void SR_ESRGAN_ncnn_Strategy::processTask(TaskBasis& task){
    //判断类型 图和视频分开处理
    //创建临时文件 是否需要调用ff (输入用拷贝,输出用移动)
    //处理完成
    
    // 实现音频分离的具体处理逻辑
    
    // 使用 AVS_task 对象进行音频处理的具体实现
    SR_Task& avsTask = static_cast< SR_Task&>(task);
    
    //进度用
    avsTask_P = &avsTask;
    ffmpeg_load_jd = 0;
    esrgan_jd = 0;
    ffmpeg_out_jd = 0;
    

    //设置状态
    avsTask.set_stateStart();
    avsTask.setProgress(0.01);
    
    //输入处理
    file_loadProcess(avsTask);
    
//转换处理
    QStringList arguments;
    arguments << "-i" << tempDir->path();
    arguments << "-o" << outTempDir->path();
    arguments << "-m" <<  avsTask.sr01_model_path_dir ;
    arguments << "-n" << avsTask.sr01_model_name;
    //放大倍数处理
    
    //显示倍数
    QString scaled = "4";
    std::string key = avsTask.sr01_model_name.toStdString();
    auto it = SR_ESRGAN_NCNN_Models_Scaled_DICTIONARY.find(key);
    if (it != SR_ESRGAN_NCNN_Models_Scaled_DICTIONARY.end()) {
        // 键存在，可以访问对应的值
        const std::vector<int>& values = it->second;
        scaled = QString::number(values[0]);

    }
    arguments << "-s" << scaled;
    
    //视频和图片分开处理
    if (avsTask.media_type ==  SR_Task::Video){
        arguments << "-f" << "jpg";
    }else if (avsTask.media_type ==  SR_Task::Image){
        arguments << "-f" << "png";

    }
    
    
   
    esrganProcess->start(bin_url, arguments);
    esrganProcess->waitForFinished(-1);
    
    
    //输出处理合成视频
    file_outProcess(avsTask);
    
    
    //设置完成。如果状态不是运行 不会标记成完成
    avsTask.set_stateFinished();
    
    
    if (avsTask.getState() == TaskBasis::State::Finished){
        qDebug() << "完成";
    }else{
        qDebug() << "失败";
        qDebug() << QString::fromStdString(avsTask.getOutInfo());
    }
    
    
  
    
    

    
    delete outTempDir;
    outTempDir = nullptr;
    delete tempDir;
    tempDir = nullptr;
        
        
       
    
    
    
    
    
    
    
}


//设置进度条 单任务
void SR_ESRGAN_ncnn_Strategy::setProgress(){
    
  
    
    
    //判断是否取消。取消就引发异常
    if(this->m_is_cancelled != nullptr){
        if(*this->m_is_cancelled == true){
            
            // 状态修改
            avsTask_P->set_stateCancel();
            
            //取消主任务后台执行
            this->cancel();
            ffmpeg_obj->cancel();
            
            
            //清空临时文件
            delete outTempDir;
            outTempDir = nullptr;
            delete tempDir;
            tempDir = nullptr;
            
            //取消引起的异常
            throw OperationCanceledException("Operation canceled.");
            
            
        }
    }
    
    //再添加一个因素
    double progress = ffmpeg_load_jd*0.1 + esrgan_jd*0.8 +  ffmpeg_out_jd*0.1;
    avsTask_P->setProgress(progress);
    
}



bool SR_ESRGAN_ncnn_Strategy::verifyModel(QString model_dir,QString model_name){
    //生成一张图片,在临时文件储存 然后验证是否可以用,没有收到反馈就是失败
    //创建一个临时文件夹
    QTemporaryDir _tempDir;
    QString v_tempDir = _tempDir.path();

    //通过QT创建一张15*15的jpg图像,并保存在临时文件夹下
    QString imageFilePath =  v_tempDir + "/my_image.jpg";
    QImage image(15, 15, QImage::Format_RGB32);
    image.fill(Qt::white);
    image.save(imageFilePath, "JPG");

    QString out_imageFilePath =  v_tempDir + "/out_my_image.jpg";
    //获取图片的地址
    qDebug()<<v_tempDir;
    
    QProcess v_esrganProcess;
    QStringList arguments;
    arguments << "-i" << imageFilePath;
    arguments << "-o" << out_imageFilePath;
    arguments << "-m" << model_dir;
    arguments << "-n" << model_name;
    arguments << "-f" << "jpg";
//    arguments << "-s" << "2";
    qDebug()<< bin_url <<arguments;
    v_esrganProcess.start(bin_url, arguments);
    //获取返回
    // 等待进程完成
    if (v_esrganProcess.waitForFinished()) {
        // 进程完成后的处理逻辑
        
//        // 获取进程的输出
//        QByteArray output = v_esrganProcess.readAllStandardError();
//        qDebug()<<QString::fromUtf8(output);
//        // 使用正则表达式提取百分比
//        QRegularExpression re("(\\d+\\.\\d+)%");
//        QRegularExpressionMatch match = re.match(QString::fromUtf8(output));
//        if (match.hasMatch()) {
//            QString percentage = match.captured(1);
//            qDebug() << "Percentage: " << percentage;
//        } else {
//            qDebug() << "Percentage not found.";
//        }
//
        
        //判断输出文件是否存在
        QFile file(out_imageFilePath);

        // 检查文件是否存在
        if (file.exists()) {
            qDebug() << "File exists.";
                
        } else {
            qDebug() << "File does not exist.";
            return false;
        }
        


    } else {
        return false;
        qDebug() << "Error: Failed to wait for the process to finish.";
    }
    
    
    return true;
    
    
}

