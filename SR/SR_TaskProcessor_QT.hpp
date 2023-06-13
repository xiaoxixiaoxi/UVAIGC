//
//  SR_TaskProcessor_QT.hpp
//  UVAIGC
//
//  Created by 林溪明 on 2023/6/7.
//

#ifndef SR_TaskProcessor_QT_hpp
#define SR_TaskProcessor_QT_hpp

#include <stdio.h>
#include <QObject>

//处理对象 AVS_TaskProcessor.hpp
//#include "../AudioVocalSeparation/AVS_TaskProcessor.hpp"
//#include "../AudioVocalSeparation/LoaderFile/AVS_QTLoaderFile.hpp"

#include "../Basis/TaskBasis.hpp"
#include "../SR/SR_type.h"
#include <QTimer>
#include <vector>
#include "../SR/sr_esrgan_ncnn_strategy.h"

//定义一个带有信号的处理类
class SR_TaskProcessor_QT: public QObject
{
    Q_OBJECT
signals:
    void progress_updataSig(int tag);
    void completedSig();//完成

    void stopSig();
    
public:
//    SR_TaskProcessor_QT(QObject *parent = nullptr);
    SR_TaskProcessor_QT(QObject *parent,bool* is_stop);
    //TaskListOBJ();
    ~SR_TaskProcessor_QT();
    
    std::vector<SR_Task>* avs_Tasks;
    
    SR_TaskProcessingStrategy* m_strategy;
    
    void setObj(std::vector<SR_Task>* _avs_Tasks){
        avs_Tasks = _avs_Tasks;
        
//        m_strategy = _m_strategy;
    };
    
    void processTasks( std::vector<SR_Task>& tasks);
    
    QTimer *timer;
//    // 状态内存
//    bool* m_is_cancelled = nullptr;
    
public slots:
    void run();
    void stop();
    
    
    
};




#endif /* SR_TaskProcessor_QT_hpp */
