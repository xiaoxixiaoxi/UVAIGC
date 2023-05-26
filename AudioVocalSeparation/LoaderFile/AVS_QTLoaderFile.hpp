//
//  AVS_QTLoaderFile.hpp
//  UVAIGC
//
//  Created by 林溪明 on 2023/5/11.
// 自定义用QT 自带的库 解码成音频数据

#ifndef AVS_QTLoaderFile_hpp
#define AVS_QTLoaderFile_hpp

#include <stdio.h>
#include "AVS_LoaderFile.hpp"

#include <QString>

#include <QAudioFormat>
#include <QAudioDecoder>
#include <QUrl>
#include <QFile>
#include <QVector>
#include <QEventLoop>
#include <QBuffer>
#include <QAudioSink>


class AVS_QTLoaderFile : public AVS_LoaderFile {
public:
    AVS_QTLoaderFile();
    
    //std::string path , int64_t intime, int64_t outtime
    
    std::vector<std::vector<double>> loadAudioFromFile(const std::string& filePath , int64_t intime, int64_t outtime) override {
        //QT自带库解码 TODO: 不能解码自动抛出异常
        return decoderAudio(filePath, intime, outtime);
    }
    
    
    std::vector<std::vector<double>> decoderAudio(std::string path , int64_t intime, int64_t outtime);
};

#endif /* AVS_QTLoaderFile_hpp */
