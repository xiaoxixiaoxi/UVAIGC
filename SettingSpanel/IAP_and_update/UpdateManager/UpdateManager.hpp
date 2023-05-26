//
//  UpdateManager.hpp
//  IsProC
//
//  Created by 林溪明 on 2022/12/26. 检查更新
//

#pragma once

#ifndef UpdateManager_hpp
#define UpdateManager_hpp

//#include "MyIAPinfo.h"

#include <iostream>
#include <stdio.h>
#include "../MyIAP_C/aes/plusaes/json.hpp"


#include "../MyIAP_C/http/httplib.h"
#include <QSettings>

#include "../../Settings_info.hpp"

#include <QSysInfo>
// 采用多线程

class UpdateManager
{
    public:
    //// 绑定网站的别名
    std::string appID_license = "";
    std::string to_url_license = "";
    
   
    
    //本地版本
    double appStoreVersion_bd = 0.0;
    // 是否跳过版本
    double  skip_version = 0.0;
    
    // 是否弹窗窗口
    bool is_Alert = false;
    
    double zxVersion = 0.0;
    std::string new_content = "";
    
    
    // 保存路径
    std::string save_path = "";

    //qt保存
    QSettings settings = QSettings(ORGANIZATION_Name, APPLICATION_Name);;

    
    UpdateManager(double v_bd);
    
    
    //请求检查更新 有新版本 就返回真。UI界面在外部处理
    bool my_init_license(){
        
        
       
        // 延迟几秒
        
        
        
        
        // 发起网络请求
        std::string gat_url = "/wp-json/wp/v2/pages?slug=" + appID_license;
        //"https://www.yssck.com/wp-json/wp/v2/pages?slug="
        
        httplib::Client cli("http://www.yssck.com");
        
        
        if (auto res = cli.Get(gat_url)) {
          if (res->status == 200) {
              //res->body
            
              
              std::string out_s = res->body;
              
              try
              {
                  
                  nlohmann::json in_data = nlohmann::json::parse(out_s)[0];
                  
                  if (in_data != nullptr){
                      
                      // 解析数据
                      nlohmann::json  acf = in_data["acf"];
                      //TODO: win和mac 分开处理
                      // 获取当前平台的产品类型
                      
                      
                      
                      QString productType = QSysInfo::productType();
                      // 获取当前平台的内核类型
                      QString kernelType = QSysInfo::kernelType();
                      double appStoreVersion_num = 0;
                      // 有新版 窗口提示 是否去更新
                      std::string info = "";
                      // 判断当前平台
                     if (productType.contains("windows", Qt::CaseInsensitive)) {
                         // Windows 平台
                         // 执行 Windows 平台相关代码
                         auto appStoreVersion = acf["win_version"];
                         if (appStoreVersion != nullptr){
                             appStoreVersion_num = acf["win_version"];
                             info = acf["win_new_content"];
                         }
                         
                         
                         
                     } else if (productType.contains("mac", Qt::CaseInsensitive) && kernelType.contains("darwin", Qt::CaseInsensitive)) {
                         // macOS 平台
                         // 执行 macOS 平台相关代码
                         auto appStoreVersion = acf["mac_version"];
                         if (appStoreVersion != nullptr){
                             appStoreVersion_num = acf["mac_version"];
                             info = acf["mac_new_content"];
                         }
                         
                         
                     } else {
                         // 其他平台
                         // 执行其他平台相关代码
                     }
                      
                      
                      
                      
                      
                      
                      if (appStoreVersion_num != skip_version && appStoreVersion_num > appStoreVersion_bd){
                          // 有新版 窗口提示 是否去更新
//                          std::string info = acf["win_new_content"];
                          
                          Alert(info, appStoreVersion_num);
                          //std::cout << "有新版本"<< appStoreVersion_num << std::endl;
                          
                      }else{
                          std::cout << "没有新版本或者跳过"<< appStoreVersion_num << std::endl;
                      }
                      
                      
                     
                      
                  }
               
                  
                  
                  
              }
              catch (nlohmann::json::parse_error& ex)
              {
                  std::cerr << "解析发生错误parse error at byte " << ex.byte << std::endl;
              }
                  
                  
              
              
              
             
              
              
          }
        } else {
          auto err = res.error();
          std::cout << "HTTP error: " << httplib::to_string(err) << std::endl;
        }
        
        
        return is_Alert;
        
        
        
    };
    
    // 窗口弹窗提示
    bool Alert(std::string info ,double _zxVersion){
        
        is_Alert = true;
        zxVersion = _zxVersion;
        new_content = info;
        
        std::cout << "有新版本:"<< zxVersion << "本地版本为："<< appStoreVersion_bd <<"\n" <<std::endl;
        std::cout << "更新内容:"<< info << std::endl;
        
        return is_Alert;
        
        
    
        
        
        // 去更新
        
        // 取消
        
        // 跳过本版本更新 把本版本号存起来
        
        
        
    };
    
    
//    /https://blog.csdn.net/qq_33236491/article/details/108321230
    // 可以保留2位 //方法2：通过sprintf(...)库函数来实现2位小数转换
    std::string doubleToString(const double &val)
    {
        char* chCode;
        chCode = new char[20];
        sprintf(chCode, "%.1lf", val);
        std::string str(chCode);
        delete[]chCode;
        return str;
    };
    
    
    // 跳过版本保存
    void save_skip_version();
    
    void read_skip_version();
    
    
    
};

#endif /* UpdateManager_hpp */
