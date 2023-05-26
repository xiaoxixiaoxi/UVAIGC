//
//  AudioVocalSeparation.hpp
//  UVAIGC
////音频分离调用的是 这个一个音频分离的类
// 通过fftw库 完成音频数据傅立叶变换、
//实现多个类别的引擎onnx
//实现不同模型文件


//添加文件
//开始
//结束
//取消

//基础类别
//应该实现 文件接入c++数组 返回数据 类型名称



//  Created by 林溪明 on 2023/5/6.
//

#ifndef AudioVocalSeparation_hpp
#define AudioVocalSeparation_hpp

#include <stdio.h>
#include <iostream>
#include <cmath>
#include <fftw3.h>
#include <vector>
#include <cstring>
#include <fstream>
#include <complex>


#include "AVS_type.h"





class AudioVocalSeparation
{
public:
    AudioVocalSeparation();
    ~AudioVocalSeparation();
    
    void run(std::vector<std::vector<double>> in_data,std::vector<std::vector<double>>& out_data);
    void test();
    
    
    //类型
    AVS_type run_type;

    

};




#endif /* AudioVocalSeparation_hpp */
