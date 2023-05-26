//
//  TaskBasis.cpp
//  UVAIGC
//
//  Created by 林溪明 on 2023/5/10.
//

#include "TaskBasis.hpp"

//判断文件是否存在
#include <iostream>
#include <fstream>
#include <sstream>

TaskBasis::TaskBasis(const std::string& name, const std::string& description) :
    m_name(name), m_description(description), m_state(NotStarted)
{
}

TaskBasis::~TaskBasis()
{
}

const std::string& TaskBasis::getName() const
{
    return m_name;
}

void TaskBasis::setName(const std::string& name)
{
    m_name = name;
}




const std::string& TaskBasis::getUrl() const
{
    return m_url;
}

void TaskBasis::setUrl(const std::string& url)
{
    m_url = url;
}



const std::string& TaskBasis::getDescription() const
{
    return m_description;
}

void TaskBasis::setDescription(const std::string& description)
{
    m_description = description;
}


const std::string& TaskBasis::getOutInfo() const
{
    return m_outInfo;
}

void TaskBasis::setOutInfo(const std::string& into)
{
    m_outInfo = into;
}








TaskBasis::State TaskBasis::getState() const
{
    return m_state;
}
void TaskBasis::set_stateWaiting()
{
    m_state = Waiting;
}


void TaskBasis::set_stateFinished()
{
    if (m_state == Running) {
        m_state = Finished;
    }
    
   
}

void TaskBasis::set_stateStart()
{
    m_state = Running;
}

void TaskBasis::set_stateCancel()
{
    m_state = Cancelled;
}

void TaskBasis::set_stateError(){
    m_state = Error;
}

double TaskBasis::getProgress() const
{
    return m_progress;
}

void TaskBasis::setProgress(double progress)
{
    m_progress = progress;
}


// 判断文件是否存在的方法
bool TaskBasis::fileExists(const std::string& filePath){
    std::ifstream file(filePath);
    return file.good();
    
}
//文件存在自动加一
std::string TaskBasis::getAvailableFilePath(const std::string& directory, const std::string& filename, const std::string& extension) {
    std::ostringstream oss;
    int counter = 1;
    
    std::string filePath = directory + "/" + filename + "." + extension;
    while (fileExists(filePath)) {
        oss.str(""); // 清空字符串流
        oss << counter;
        std::string numberedFilename = filename + "_" + oss.str();
        filePath = directory + "/" + numberedFilename + "." + extension;
        counter++;
    }
    
    return filePath;
}
