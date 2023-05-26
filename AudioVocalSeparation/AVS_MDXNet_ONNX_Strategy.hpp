//
//  AVS_MDXNet_ONNX_Strategy.hpp
//  UVAIGC
//
//  Created by 林溪明 on 2023/5/10.
//

#ifndef AVS_MDXNet_ONNX_Strategy_hpp
#define AVS_MDXNet_ONNX_Strategy_hpp

#include <stdio.h>
#include "AVS_TaskProcessingStrategy.hpp"

#include "onnxruntime_cxx_api.h"
#include "AVS_type.h"
#include <iostream>
#include <cmath>
#include <fftw3.h>
#include <vector>
#include <cstring>
#include <fstream>
#include <complex>

#include "AVS_Task.hpp"


class AVS_MDXNet_ONNX_Strategy : public AVS_TaskProcessingStrategy {
    
public:
    AVS_MDXNet_ONNX_Strategy(AVS_LoaderFile* m_fileLoader = nullptr);
    ~AVS_MDXNet_ONNX_Strategy();
    
    // 实现音频分离的具体处理逻辑
    virtual void processTask( TaskBasis& task) override;
    
    
    Ort::Env env;
//    Ort::Session * session = nullptr;
    
//    int input_Shape_01 = 0;
//    int input_Shape_02 = 0;
//    int input_Shape_03 = 0;
    
    //模型类型
//    AVS_MDXNet_Models_Type model_type = MMType_Unknown;
    
    
//    Ort::Env env_Instrumental;
    Ort::Session * session_Instrumental = nullptr;
    
    
//    Ort::Env env_Vocals;
    Ort::Session * session_Vocals = nullptr;
    
     
    //通用调用
//    void process() override;
    
//    //处理加载类
//    AVS_LoaderFile* m_fileLoader = nullptr;
//    void setLoaderFile(AVS_LoaderFile* audioLoader){
//        m_fileLoader = audioLoader;
//    }
    
    //保存类
    
    
    
    
    
    
//    加载模型
    bool loadOxxnModel_Instrumental(std::string modelPath);
    bool loadOxxnModel_Vocals(std::string modelPath);
    bool loadOxxnModel(Ort::Session *&session,std::string modelPath);
    
    //判断模型 是不是 可以价值
    bool isLoadModel(AVS_MDXNet_Models_Type type);
    
    //验证模型是否可以用
    bool verifyModel(std::string modelPath);
    
    
    
    //获取input数据
    std::vector<int64_t> getInPutGetShape(Ort::Session *session);
    
    
    //推理
    void run_oxxn(Ort::Session* session,std::vector<std::vector<std::complex<double>>>& spectrogram_l,std::vector<std::vector<std::complex<double>>>& spectrogram_r,int offset);
    
    //数据准备
    void run_ls(std::vector<std::vector<double>> in_data,std::vector<std::vector<double>>& out_data);
    void run( AVS_Task& avsTask,Ort::Session* session,std::vector<std::vector<double>>& in_data,std::vector<std::vector<double>>& out_data);
    
    
    //进度条分配 当前/总轨道数*(stft*0.1 + 推理*0.8 + istft*0.1) = 1.0
    //设置进度条
    void setProgress(AVS_Task& avsTask,float gd_bl,float stft_jd,float onnxrun_jd,float istft_jd);
    double myprogress = 0;
    
    
    //封装一个文件保存
    bool savefile(AVS_Task& avsTask,std::vector<std::vector<double>>& signal,std::string& name2);
    
    
    
    
    
    
    
};

#endif /* AVS_MDXNet_ONNX_Strategy_hpp */
