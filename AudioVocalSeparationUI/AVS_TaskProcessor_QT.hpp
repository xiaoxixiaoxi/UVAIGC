//
//  AVS_TaskProcessor_QT.hpp
//  UVAIGC
//
//  Created by 林溪明 on 2023/5/12.
//

#ifndef AVS_TaskProcessor_QT_hpp
#define AVS_TaskProcessor_QT_hpp

#include <stdio.h>
#include <QObject>

//处理对象 AVS_TaskProcessor.hpp
#include "../AudioVocalSeparation/AVS_TaskProcessor.hpp"
#include "../AudioVocalSeparation/LoaderFile/AVS_QTLoaderFile.hpp"

#include "../Basis/TaskBasis.hpp"

#include <QTimer>

//定义一个带有信号的处理类
class AVS_TaskProcessor_QT: public QObject
{
    Q_OBJECT
signals:
    void progress_updataSig(int tag);
    void completedSig();//完成

    void stopSig();
    
public:
    AVS_TaskProcessor_QT(QObject *parent = nullptr);
    //TaskListOBJ();
    ~AVS_TaskProcessor_QT();
    
    std::vector<AVS_Task>* avs_Tasks;
    
    TaskProcessor* processor;
    
    void setObj(std::vector<AVS_Task>* _avs_Tasks ,TaskProcessor* _processor){
        avs_Tasks = _avs_Tasks;
        processor = _processor;
    };
    
    QTimer *timer;
    
    
    
public slots:
    void run();
    void stop();
    
    
    
};



#endif /* AVS_TaskProcessor_QT_hpp */
