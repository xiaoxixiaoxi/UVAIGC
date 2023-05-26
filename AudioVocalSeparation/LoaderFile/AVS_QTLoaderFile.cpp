//
//  AVS_QTLoaderFile.cpp
//  UVAIGC
//
//  Created by 林溪明 on 2023/5/11.
//

#include "AVS_QTLoaderFile.hpp"
#include <QAudioDecoder>
AVS_QTLoaderFile::AVS_QTLoaderFile(){
    
}

std::vector<std::vector<double>> AVS_QTLoaderFile::decoderAudio(std::string path , int64_t intime, int64_t outtime)
{
    QObject qobj = QObject();
   
    
    bool shouldExit = false;  // 添加一个标志变量
    
    
    QAudioDecoder* decoder = new QAudioDecoder(&qobj);
    QUrl url = QUrl::fromLocalFile(QString::fromStdString(path));
    decoder->setSource(url);
    
    
    if (decoder->error() != QAudioDecoder::NoError) {
        
        qDebug() << "Failed to open audio file";
        delete decoder;
        
        throw "Failed to open audio file!";
    }
    
    
    
    QObject::connect(decoder, QOverload<QAudioDecoder::Error>::of(&QAudioDecoder::error),
                           [=](QAudioDecoder::Error error){
        
        qDebug() << error;
        
    });
    
    
    
    // 获取音频格式
    const QAudioFormat format = decoder->audioFormat();

    
   
    //
    
    //获取轨道数
    //开始解码

    //TODO: 如何正确获取轨道数
    QAudioFormat desiredFormat;
    desiredFormat.setChannelCount(2);
    desiredFormat.setSampleFormat(QAudioFormat::Float);
    desiredFormat.setSampleRate(44100);

   
    decoder->setAudioFormat(desiredFormat);
    
    
    
    QByteArray data;
    QBuffer buffer(&data);
    buffer.open(QIODevice::WriteOnly);

    QEventLoop loop;
    QAudioDecoder::connect(decoder, &QAudioDecoder::finished, &loop, &QEventLoop::quit);
    
    int64_t sampleCount = 0;
    int channelCount = 0;
    
    
    QAudioDecoder::connect(decoder, &QAudioDecoder::bufferReady, &qobj, [&buffer,&decoder,&intime,&outtime,&sampleCount,&channelCount]() {
        
        
        //  ≈–∂œ ±º‰∑∂Œß
        if (!decoder->bufferAvailable()) {
            return;
        }


        auto frame = decoder->read();
        qint64 nowtime = decoder->position();

        if (intime > nowtime) {
            return;
        }

        if (nowtime > outtime) {
            decoder->stop();
            decoder->finished();
            return;
        }
        
        sampleCount += frame.sampleCount();
        channelCount = frame.format().channelCount();
        buffer.write(frame.constData<char>(), frame.byteCount());
        
        
        
        
        

        });
    
    decoder->start();
    loop.exec();
    buffer.close();

    buffer.open(QIODevice::ReadOnly);

    int64_t all_l = buffer.data().size() / sizeof(float);
    std::vector<float> pcmf32(sampleCount,-1);
    //https://blog.csdn.net/qxpxp/article/details/116308213 øΩ±¥ ˝æ› Ω´∂˛Ω¯÷∆ char  ˝æ› ÷±Ω”◊™ªª≥… float
    memcpy(&pcmf32[0], buffer.data(), buffer.data().size());
    // 将音频数据存储到 std::vector<std::vector<float>> 中
    
    std::vector<std::vector<double>> audioData(channelCount, std::vector<double>(sampleCount/channelCount));
    //判断轨道数
    for (int i = 0; i < sampleCount/channelCount; ++i) {
        for (int j = 0; j < channelCount; ++j) {
            const int sampleIndex = i * channelCount + j;
            audioData[j][i] = double(pcmf32[sampleIndex]);
        }
    }
    
    
    
    
    

    qDebug() << " o122k" << buffer.data().size() << sizeof(float) << pcmf32.size();

   
    delete decoder;

    return audioData;
   
    

   

}
