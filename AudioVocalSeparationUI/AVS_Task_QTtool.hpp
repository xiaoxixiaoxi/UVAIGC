//
//  AVS_Task_QTtool.hpp
//  UVAIGC
// 用QT一些库 完成文件读出 时间获取工具集合
//  Created by 林溪明 on 2023/5/11.
//

#ifndef AVS_Task_QTtool_hpp
#define AVS_Task_QTtool_hpp

#include <QMutex>
#include <QWaitCondition>
#include <QThread>
#include <QMessageBox>
#include <qeventloop.h>
#include "../Basis/TaskBasis.hpp"
#include "../AudioVocalSeparation/AVS_Task.hpp"

#include <stdio.h>
class AVS_Task_QTtool
{
public:
//    AVS_Task_QTtool();
//    ~AVS_Task_QTtool();
    
    //获取信息
    //判断是否能导入
    static bool if_hasVideo(AVS_Task& obj,QUrl url);
//    bool if_hasVideo(QUrl url);
    
    
    //显示媒体基本信息
    static QString getShowInfo(AVS_Task* obj);
    
    
    //显示在本地文件的方法
    static void showFileFromFinder(QStringList& pathList);
    
    
    
    
    
    
    
};












#endif /* AVS_Task_QTtool_hpp */
