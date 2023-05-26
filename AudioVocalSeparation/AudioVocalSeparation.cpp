//
//  AudioVocalSeparation.cpp
//  UVAIGC
//
//  Created by 林溪明 on 2023/5/6.
//

//音频分离

#include "AudioVocalSeparation.hpp"
#include "AudioFile.h"

#include "AVS_TaskProcessor.hpp"
#include "LoaderFile/AVS_QTLoaderFile.hpp"


AudioVocalSeparation::AudioVocalSeparation(){
    this->run_type = AVS_MDXNet;
    //加载模型
    std::string weightFile = "/Users/linximing/Desktop/models/MDX_Net_Models/UVR-MDX-NET-Inst_HQ_1.onnx";
    std::string weightFile_vocals = "/Users/linximing/Desktop/models/MDX_Net_Models/onnx_A/vocals.onnx";

    
    
//    avs_maxnet_Instrumental_obj.loadOxxnModel(weightFile);
//    avs_maxnet_Vocals_obj.loadOxxnModel(weightFile_vocals);
    
}


AudioVocalSeparation::~AudioVocalSeparation(){
    
}





void AudioVocalSeparation::test(){
    
    std::cout<< "测试函数";
    //音频读取
    //处理
    //音频写入
    
    // 创建音频分离策略
    TaskProcessor processor;
    
    
    
    //新建文件导入模块
    AVS_QTLoaderFile * load = new AVS_QTLoaderFile();
    AVS_MDXNet_ONNX_Strategy  audioSeparationStrategy(load);
//    audioSeparationStrategy.setLoaderFile(load);
    
    
    
    audioSeparationStrategy.loadOxxnModel_Instrumental("/Users/linximing/Movies/UVAIGC/AVSModel/instrumental.onnx");
    audioSeparationStrategy.loadOxxnModel_Vocals("/Users/linximing/Movies/UVAIGC/AVSModel/vocals.onnx");
    
    processor.setProcessingStrategy(&audioSeparationStrategy);
    
    
    std::vector<AVS_Task> audioTasks;
    AVS_Task test_obj;
    test_obj.Engine = AVS_type::AVS_MDXNet;

    test_obj.setUrl("/Users/linximing/Desktop/爱就一个字.mp3");
    test_obj.m_mdxnet_type_list.push_back(MMType_Instrumental);
    test_obj.m_mdxnet_type_list.push_back(MMType_Vocals);
    audioTasks.push_back(test_obj);

  
   
   // 处理音频分离任务列表
   processor.processTasks(audioTasks);
    
    
    
    
    
    
    
    
    
    
    
    
    
}
