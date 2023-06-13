//
//  SR_Task.cpp
//  UVAIGC
//
//  Created by 林溪明 on 2023/6/6.
//

#include "SR_Task.hpp"
SR_Task::SR_Task():TaskBasis("name", "description"){
    
    
    //必须要有的
    //模型相关
    m_ESRGAN_parameters["-m"] = "";
    m_ESRGAN_parameters["-n"] = "";
    m_ESRGAN_parameters["-f"] = "jpg";
    m_ESRGAN_parameters["-s"] = "4";
    
}
SR_Task::~SR_Task(){
    
}
