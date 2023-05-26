//
//  AVS_Task.hpp
//  UVAIGC
//
//  Created by 林溪明 on 2023/5/10.
//

#ifndef AVS_Task_hpp
#define AVS_Task_hpp

#include <stdio.h>

#include "../Basis/TaskBasis.hpp"
#include "AVS_type.h"
#include <vector>


class AVS_Task: public TaskBasis
{
public:
    AVS_Task();
    ~AVS_Task();
    
    //定义一个版本控制
    uint8_t versiom = 1;
    
    //分离类型 信息。需要定位到不同对象
    AVS_type Engine;
    std::vector<AVS_MDXNet_Models_Type> m_mdxnet_type_list;
    
    
    //长度
    int64_t raw_duration = -1;
    
    //出点
    int64_t timeRange_in = 0;
    int64_t timeRange_out = 0;
    
    
    //输出路径列表。可能多个
    std::vector<std::string> out_path_list;
    //输出目录 在开始前配置一下
    std::string out_path_dir;
    

    
    //显示信息
    std::string getShowInfo();
    
    //重置状态//重置
    void reset_state(){
        set_stateWaiting();
    };
    
    //重置
    void reset_obj(){
        
        reset_state();
        timeRange_in = 0;
        timeRange_out = raw_duration;
        out_path_list.clear();
        
    };
    
    void setInTimeRange(int64_t intime){
        timeRange_in = intime;
    };
    
    void setOutTimeRange(int64_t outtime){
        timeRange_out = outtime;
    };
    
    
    //s缩略图
    
    
    
    //音频输入数据
    //处理完书记
        
    

};




#endif /* AVS_Task_hpp */
