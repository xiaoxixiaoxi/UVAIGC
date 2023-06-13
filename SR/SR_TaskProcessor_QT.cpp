//
//  SR_TaskProcessor_QT.cpp
//  UVAIGC
//
//  Created by 林溪明 on 2023/6/7.
//

#include "SR_TaskProcessor_QT.hpp"

#include "../Basis/OperationCanceledException.h"


SR_TaskProcessor_QT::SR_TaskProcessor_QT(QObject *parent, bool* is_stop)
    : QObject(parent)
{
    // 构造函数的实现，使用is_stop参数
    m_strategy = new SR_ESRGAN_ncnn_Strategy(this);
    m_strategy->setCancelledP(is_stop);
}

//TaskListOBJ();
SR_TaskProcessor_QT::~SR_TaskProcessor_QT(){
    
}

void SR_TaskProcessor_QT::run(){
    
    //配置一个定时器 循环触发更新信号
//    timer->start(1000);
//   processor->s
    //触发执行任务
    
    
   

    processTasks(*avs_Tasks);

//    qDebug()<< "完成";
    emit completedSig();
//    timer->stop();
}

void SR_TaskProcessor_QT::stop(){
    //停止
    
}


void SR_TaskProcessor_QT::processTasks( std::vector<SR_Task>& tasks) {
    
   
   
    for (SR_Task& task : tasks) {
        // 使用选择的策略进行任务处理
        if (task.getState() == TaskBasis::Waiting  ) {
            
            if(*m_strategy->m_is_cancelled==true){
                break;
            }
            
            //异常捕捉
            try {
                m_strategy->processTask(task);
            }catch (const OperationCanceledException& e) {
                //std::cout << "Canceled exception caught: " << e.what() << std::endl;
                // 处理取消操作引起的异常
            } catch (const std::exception& e) {
                task.set_stateError();
                task.setOutInfo(e.what());
                // 处理其他异常
            }
                
                
                

            
            
            
        }
       
    }
    
    
//    delete m_strategy;
}

