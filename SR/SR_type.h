//
//  SR_type.h
//  UVAIGC
//
//  Created by 林溪明 on 2023/6/6.
//

#ifndef SR_type_h
#define SR_type_h

#include <iostream>
#include <map>
#include <vector>
//识别网络类型
enum SR_type {
    SR_Real_ESRGAN_NCNN_VULKANT = 0
};


//AVS_MDXNet_Models_Type  先不用。到时候指定列表获取,然后可以再次编辑
enum SR_ESRGAN_NCNN_Models_NAME {
    realesr_animevideov3_x2 = 0,
    realesr_animevideov3_x3,
    realesr_animevideov3_x4 ,
    realesrgan_x4plus_anime,
    realesrgan_x4plus

    
};

//一个字典储存倍数
// 全局常量字典
const std::map<std::string, std::vector<int>> SR_ESRGAN_NCNN_Models_Scaled_DICTIONARY = {
    {"realesr-animevideov3-x2", {2}},
    {"realesr-animevideov3-x3", {3}},
    {"realesr-animevideov3-x4", {4}},
    {"realesrgan-x4plus-anime", {4}},
    {"realesrgan-x4plus", {4}}
};





// 定义一个状态的
enum SR_Status
{
    //未知状态
    SR_S_unknown = 0,
    SR_S_waiting = 1, //读取
    SR_S_exoirting = 2, //导出中
    SR_S_completed = 3, //完成
    SR_S_failed = 4, //错误
    SR_S_cancelled = 5, // 取消

    

};


#endif /* SR_type_h */
