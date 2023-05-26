//
//  AVS_TaskProcessor_QT.cpp
//  UVAIGC
//
//  Created by 林溪明 on 2023/5/12.
//

#include "AVS_TaskProcessor_QT.hpp"
#include <QThread>



AVS_TaskProcessor_QT::AVS_TaskProcessor_QT(QObject *parent):QObject(parent){
    
//    timer = new QTimer(nullptr);
//
//    connect(timer,&QTimer::timeout , this, [=](){
//        emit progress_updataSig(0);
//        qDebug()<< "uuu";
//    });
    
//
//
    
}
//TaskListOBJ();
AVS_TaskProcessor_QT::~AVS_TaskProcessor_QT(){
    
}

void AVS_TaskProcessor_QT::run(){
    
    //配置一个定时器 循环触发更新信号
//    timer->start(1000);
//   processor->s
   

    processor->processTasks(*avs_Tasks);

    qDebug()<< "完成";
    emit completedSig();
//    timer->stop();
}

void AVS_TaskProcessor_QT::stop(){
    //停止
    
}
