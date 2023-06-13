//
//  OperationCanceledException.h
//  UVAIGC
//
//  Created by 林溪明 on 2023/6/7.
//取消引起的异常处理

#ifndef OperationCanceledException_h
#define OperationCanceledException_h

//定义一个取消引起的异常
class OperationCanceledException : public std::runtime_error {
public:
    OperationCanceledException(const std::string& message)
        : std::runtime_error(message) {
    }
};



#endif /* OperationCanceledException_h */
