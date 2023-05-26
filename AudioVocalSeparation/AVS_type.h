//
//  AVS_type.h
//  UVAIGC
//
//  Created by 林溪明 on 2023/5/6.
//

#ifndef AVS_type_h
#define AVS_type_h

//识别网络类型
enum AVS_type {
    AVS_MDXNet = 0
};


//识别网络类型下面的模型类型.背景音乐,鼓,人声,其他 等等
enum AVS_MDXNet_Models_Type{
    MMType_Unknown = 100,
    MMType_Instrumental = 0,
    MMType_Vocals = 1,
    MMType_Bass = 2,
    MMType_Drums = 3,
    MMType_Other = 4,
};


// 定义一个状态的
enum AVS_Status
{
    //未知状态
    AVS_S_unknown = 0,
    AVS_S_waiting = 1, //读取
    AVS_S_exoirting = 2, //导出中
    AVS_S_completed = 3, //完成
    AVS_S_failed = 4, //错误
    AVS_S_cancelled = 5, // 取消

    

};



#endif /* AVS_type_h */
