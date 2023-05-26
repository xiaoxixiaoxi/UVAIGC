//
//  AVS_SaveFileModule.hpp
//  UVAIGC
//
//  Created by 林溪明 on 2023/5/12.
//

#ifndef AVS_SaveFileModule_hpp
#define AVS_SaveFileModule_hpp

#include <stdio.h>
#include "AVS_SaveFile.hpp"
//调用
class AVS_SaveFileModule {
private:
    AVS_SaveFileStrategy* strategy;

public:
    void setStrategy(AVS_SaveFileStrategy* newStrategy) {
        strategy = newStrategy;
    }

    bool saveFile(AVS_SaveParams& params) {
        if (strategy) {
            return strategy->save(params);
        }
        return false; // 没有设置策略时，返回保存失败
    }
};

#endif /* AVS_SaveFileModule_hpp */
