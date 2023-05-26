//
//  AVS_SaveFile.hpp
//  UVAIGC
//
//  Created by 林溪明 on 2023/5/12.
// 文件保存模块

#ifndef AVS_SaveFile_hpp
#define AVS_SaveFile_hpp

#include <stdio.h>
#include <vector>
#include <iostream>

struct AVS_SaveParams {
    uint8_t version = 1;//版本控制
    std::string filePath;
    std::vector<std::vector<double>>* audioData;
    int sampleRate;
    // 可以添加其他参数
};


//基础类
class AVS_SaveFileStrategy {
public:
    virtual bool save( AVS_SaveParams& params ) = 0;
};

//基本的wav
class AVS_WAVFileSaveStrategy : public AVS_SaveFileStrategy {
public:
    bool save( AVS_SaveParams& params) override;
};

//将来扩展mp3 mp4等等





#endif /* AVS_SaveFile_hpp */
