//
//  UpdateManager.cpp
//  Skeleton
//
//  Created by 林溪明 on 2022/12/26.
//
//#include "MyIAPinfo.h"

#include "UpdateManager.hpp"

#include <QSettings>



//
UpdateManager::UpdateManager(double v_bd){
    
   
   
    
    appStoreVersion_bd = v_bd;
//    skip_version = v_skip;
    //my_init_license();
    
    
    appID_license = APPID_license;
    to_url_license = TO_URL_license;
    //平台判断
//    save_path = MAC_INIT_PATH;
    
    // 获取保存的跳过版本
    read_skip_version();
    
    
    
    
}

void UpdateManager::save_skip_version(){

    //最新版本保存下来  作为跳过版本

    //保存
    settings.setValue("info/skipVersion", this->zxVersion);
}

void UpdateManager::read_skip_version(){
    this->skip_version = settings.value("info/skipVersion").toDouble();  
}


