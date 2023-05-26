//
//  TaskBasis.hpp
//  UVAIGC
//
//  Created by 林溪明 on 2023/5/10.
//

#ifndef TaskBasis_hpp
#define TaskBasis_hpp

#include <stdio.h>
#include <iostream>

class TaskBasis {
public:
    enum State {
        NotStarted,
        Waiting, //读取
        Running,
        Finished,
        Cancelled,
        Error
    };

    TaskBasis(const std::string& name, const std::string& description);
    virtual ~TaskBasis();

    const std::string& getName() const;
    void setName(const std::string& name);

    const std::string& getDescription() const;
    void setDescription(const std::string& description);

    State getState() const;
//    void setState(State _state);

    virtual void set_stateWaiting();
    virtual void set_stateStart();
    virtual void set_stateCancel();
    virtual void set_stateError();
    virtual void set_stateFinished();
    
    
    double getProgress() const;
    void setProgress(double progress);
    
    
    //路径
    const std::string& getUrl() const;
    void setUrl(const std::string& url);
    
    
    // 判断文件是否存在的方法
    bool fileExists(const std::string& filePath);
    //文件存在自动加一
    std::string getAvailableFilePath(const std::string& directory, const std::string& filename, const std::string& extension);
    
    
    // 设置信息输出。错误提示,等等
    const std::string& getOutInfo() const;
    void setOutInfo(const std::string& into);
    
    
    


protected:
    std::string m_name;
    std::string m_description;
    
    //路径
    std::string m_url;
    State m_state;
    double m_progress;  // 进度值
    
    //文件类型
    std::string m_fileMIMEType;
    //输出信息
    std::string m_outInfo;
};







#endif /* TaskBasis_hpp */
