//
//  AVS_SaveFile.cpp
//  UVAIGC
//
//  Created by 林溪明 on 2023/5/12.
//

#include "AVS_SaveFile.hpp"

#include "AudioFile.h"

bool AVS_WAVFileSaveStrategy::save(AVS_SaveParams& params){
   // 使用params中的参数进行保存操作
   // ...
    
    
    //保存下来
    AudioFile<double> out_audioFile;
    out_audioFile.setSampleRate(params.sampleRate);
    AudioFile<double>::AudioBuffer buffer;
    bool ok = out_audioFile.setAudioBuffer (*params.audioData);
    out_audioFile.save(params.filePath);
    
   return ok; // 保存成功返回true，保存失败返回false
}

//j
