//
//  AVS_TaskProcessor.hpp
//  UVAIGC
//
//  Created by 林溪明 on 2023/5/10.
//

#ifndef AVS_TaskProcessor_hpp
#define AVS_TaskProcessor_hpp

#include <stdio.h>
#include "AVS_MDXNet_ONNX_Strategy.hpp"
#include <vector>

class TaskProcessor {
public:
    void setProcessingStrategy(AVS_TaskProcessingStrategy* strategy) {
        m_strategy = strategy;
    };

    void processTasks( std::vector<AVS_Task>& tasks) {
        
        for ( AVS_Task& task : tasks) {
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
    }

private:
    AVS_TaskProcessingStrategy* m_strategy;
};

#endif /* AVS_TaskProcessor_hpp */
