#ifndef FFMPEGTEST_H
#define FFMPEGTEST_H

#include <QWidget>
#include <QProcess>

namespace Ui {
class FFMPEGTest;
}

class FFMPEGTest : public QWidget
{
    Q_OBJECT

public:
    explicit FFMPEGTest(QWidget *parent = nullptr);
    ~FFMPEGTest();

private slots:
    void on_pushButton_clicked();
    void onReadyReadStandardOutput();
    void onReadyReadStandardError();
    
    void runFFMPEG(long intime=0 ,long out_time=0);
    void runTask();

private:
    Ui::FFMPEGTest *ui;
    QProcess *ffmpegProcess = nullptr;
    QByteArray videoFrameData;
    
    //数据基本参数
    int frameWidth = 0;
    int frameHeight = 0;
    float frameFps = 0;
    long framess = 0;
    
    
//    //当前已处理的帧数
//    int nowFrame = 0 ;
//    const int framesPerBatch = 100; // 每批处理的帧数
    
//    const int bufferCapacity = 100; // 循环队列的容量，可以根据需要进行调整
//    QVector<QByteArray> frameBuffer; // 循环队列，用于存储帧数据
    int writeIndex = 0; // 当前写入位置
    int readIndex = 0; // 当前读取位置

    
    
};

#endif // FFMPEGTEST_H
