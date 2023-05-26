//
//  Settings_info.hpp
//  UVAIGC
//
//  Created by 林溪明 on 2023/5/9.
//

#ifndef Settings_info_hpp
#define Settings_info_hpp

#include <stdio.h>
#include <iostream>
#include <QString>




//定义一个全局存储对象字符串 公司名称
extern const QString ORGANIZATION_Name;
extern const QString APPLICATION_Name;
extern const QString CLIENT_TYPE_USK;

//存储信息
extern const QString QSettings_KEY_AVS_model_save_path;

//路径下模型文件的名称
extern const QString QSettings_KEY_AVS_Instrumental_selection_name;
extern const QString QSettings_KEY_AVS_Vocals_selection_name;
extern const QString QSettings_KEY_AVS_Bass_selection_name;
extern const QString QSettings_KEY_AVS_Drums_selection_name;
extern const QString QSettings_KEY_AVS_Other_selection_name;
//默认识别的列表
extern const QString QSettings_KEY_AVS_OutType_list;

//extern const QString QSettings_KEY_AVS_selection_language;



//更新信息用
extern const std::string APPID_license;
extern const std::string TO_URL_license;

extern const std::string MAC_APP_Identifier;

//
extern const std::string TO_URL_AVS_Download_Help;


//购买激活码连接
extern const std::string GETPRO_URL;

//更多软件url
extern const std::string TO_URL_MoreAPP;
extern const std::string TO_URL_ABOUTUS;






#endif /* Settings_info_hpp */
