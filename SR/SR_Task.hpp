//
//  SR_Task.hpp
//  UVAIGC
//
//  Created by 林溪明 on 2023/6/6.
//

#ifndef SR_Task_hpp
#define SR_Task_hpp

#include <stdio.h>

#include "../Basis/TaskBasis.hpp"
#include "SR_type.h"
#include <vector>
#include <unordered_map>
#include <QImage>

class SR_Task: public TaskBasis
{
public:
    SR_Task();
    ~SR_Task();
    
    //引擎的正常情况

    
    //定义一个版本控制
    uint8_t versiom = 1;
    
 
    
    //分离类型 信息。需要定位到不同对象
    SR_type Engine;
    
    //字典
    std::unordered_map<std::string, std::string> m_ESRGAN_parameters;
    //模型名称
//    std::string SR_ESRGAN_model_name;
//    std::string SR_ESRGAN_model_path_dir;
//    std::vector<AVS_MDXNet_Models_Type> m_mdxnet_type_list;
    
    
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
    QImage ThumbnailImage;
    void setThumbnailImage(QImage Image){
        ThumbnailImage = Image;
        
    };
    QSize size;
    void setSize(QSize _size){
        size =  _size;
        
    };
    
    //
    //1,引擎类型 2,模型选择名称 3,倍数匹配 4,模型路径? 5,视频输出类型 6,图片输出类型
    QString sr01_model_path_dir;
    QString sr01_model_name;
    QString selection;
    
   
    
    //音频输入数据
    //处理完书记
        
    

};


#endif /* SR_Task_hpp */
