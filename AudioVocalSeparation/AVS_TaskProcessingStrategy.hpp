//
//  AVS_TaskProcessingStrategy.hpp
//  UVAIGC
//
//  Created by 林溪明 on 2023/5/10.
//

#ifndef AVS_TaskProcessingStrategy_hpp
#define AVS_TaskProcessingStrategy_hpp

#include <stdio.h>

#include "../Basis/TaskBasis.hpp"

#include "AVS_type.h"
#include <iostream>
#include <cmath>
#include <fftw3.h>
#include <vector>
#include <cstring>
#include <fstream>
#include <complex>

#include "LoaderFile/AVS_LoaderFile.hpp"

#include "SaveFile/AVS_SaveFileModule.hpp"
#include "../Basis/OperationCanceledException.h"

#define M_PI 3.14159265358979323846




class AVS_TaskProcessingStrategy {
public:
    
    //引擎的正常情况
    enum State {
        NotStarted,
        Waiting, //读取
        Running,
        Finished,
        Cancelled,
        Error
    };
    
    
    virtual void processTask(TaskBasis& task) = 0;
    
    //std:
    //定义一个版本控制
    uint8_t versiom = 1;
      
    //类型
    AVS_type type;
    
    State m_state;
    

    
    // 状态内存
    bool* m_is_cancelled = nullptr;
    
    //各种信息输出
    std::string out_info = "";
    
    

    //stft变换 反变换
    std::vector<std::vector<std::complex<double>>> stft_v2(const std::vector<double>& audio, const int fft_size, const int hop_size);
    std::vector<double> istft_v2(const std::vector<std::vector<std::complex<double>>>& stft_matrix, int n_fft,int hop_length);
    
    
    // 归一化到-1,1
    void normalization(std::vector<double>& data);
    
    
    //处理加载类
    AVS_LoaderFile* m_fileLoader = nullptr;
    void setLoaderFile(AVS_LoaderFile* audioLoader){
        m_fileLoader = audioLoader;
    }
    
    //文件保存
    // 使用示例
    AVS_SaveFileModule m_fileSaveModule;
    //设置
    void setSaveStrategy(AVS_SaveFileStrategy* newStrategy){
        m_fileSaveModule.setStrategy(newStrategy);
    }
    
    //停止设置
    void setCancelledP(bool* is_stop){
        m_is_cancelled = is_stop;
    }
    ;
    
    
    
    
};

#endif /* AVS_TaskProcessingStrategy_hpp */
