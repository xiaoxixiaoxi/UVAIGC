//
//  SR_TaskProcessingStrategy.hpp
//  UVAIGC
//
//  Created by 林溪明 on 2023/6/6.
//

#ifndef SR_TaskProcessingStrategy_hpp
#define SR_TaskProcessingStrategy_hpp

#include <stdio.h>
#include "../Basis/TaskBasis.hpp"
#include "SR_type.h"

//视频图片放大的基本策略

class SR_TaskProcessingStrategy {
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
    SR_type type;
    
    State m_state;
    

    
    // 状态内存
    bool* m_is_cancelled = nullptr;
    
    //各种信息输出
    std::string out_info = "";
    
    

    //stft变换 反变换
    
 
    //处理加载类
//    AVS_LoaderFile* m_fileLoader = nullptr;
//    void setLoaderFile(AVS_LoaderFile* audioLoader){
//        m_fileLoader = audioLoader;
//    }
//
//    //文件保存
//    // 使用示例
//    AVS_SaveFileModule m_fileSaveModule;
//    //设置
//    void setSaveStrategy(AVS_SaveFileStrategy* newStrategy){
//        m_fileSaveModule.setStrategy(newStrategy);
//    }
    
    //停止设置
    void setCancelledP(bool* is_stop){
        m_is_cancelled = is_stop;
    }
    ;
    
    
    
    
};







#endif /* SR_TaskProcessingStrategy_hpp */
