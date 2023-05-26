//
//  AVS_LoaderFile.hpp
//  UVAIGC
//
//  Created by 林溪明 on 2023/5/11.
// 定义一个读出文件的虚类,基础,各个方法基础,继承这个基础,并各自实现 自定义方法

#ifndef AVS_LoaderFile_hpp
#define AVS_LoaderFile_hpp

#include <stdio.h>
#include <vector>
class AVS_LoaderFile {
public:
    virtual std::vector<std::vector<double>> loadAudioFromFile(const std::string& filePath , int64_t intime, int64_t outtime) = 0;
};

#endif /* AVS_LoaderFile_hpp */
