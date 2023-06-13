#ifndef SR_ESRGAN_NCNN_H
#define SR_ESRGAN_NCNN_H

#include <QObject>
#include <QProcess>
#include "SR_TaskProcessingStrategy.hpp"
#include "SR_Task.hpp"
#include "../Basis/FFMPEG_Strategy.hpp"
#include <QTemporaryDir>

class SR_ESRGAN_ncnn_Strategy : public QObject,public SR_TaskProcessingStrategy
{
    Q_OBJECT
public:
    explicit SR_ESRGAN_ncnn_Strategy(QObject *parent = nullptr);
    ~SR_ESRGAN_ncnn_Strategy();
    
    // 实现放大的具体处理逻辑
    virtual void processTask(TaskBasis& task) override;
    
    
    
    
    bool verifyModel(QString model_dir,QString model_name);
    
    
    
    
    void test();
    
    
    
private slots:
    //获取终端输出信息
    void onReadyReadStandardError();
    
    // 取消
    void cancel();
    
    
    
signals:
    void progress(float jd);
    //完成
    void finished();
   
    
private:
    QProcess *esrganProcess = nullptr;
    QString bin_url;
    
    QTemporaryDir* tempDir = nullptr;
    QTemporaryDir* outTempDir = nullptr;
    
    FFMPEG_Strategy* ffmpeg_obj = nullptr;
    
    
    //文件加载前处理
    void  file_loadProcess(SR_Task& avsTask);
    QString file_outProcess(SR_Task& avsTask);
    
    
    //统计转换进度用的
    int esrgan_frames_number_all = 0;
    int esrgan_frames_number_now = 0;
    
    
    //进度条分配 当前/总轨道数*(文件输入处理*0.1 + 推理*0.8 + 文件出处理*0.1) = 1.0
    //设置进度条
    void setProgress();
    SR_Task* avsTask_P = nullptr ;
    float ffmpeg_load_jd = 0;
    float esrgan_jd = 0;
    float ffmpeg_out_jd = 0;
    
    
    float myprogress = 0;
    

};

#endif // SR_ESRGAN_NCNN_H
