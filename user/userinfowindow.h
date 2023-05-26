#ifndef USERINFOWINDOW_H
#define USERINFOWINDOW_H

#include <QWidget>
#include "UserModule/UserModule.hpp"

namespace Ui {
class UserInfoWindow;
}

class UserInfoWindow : public QWidget
{
    Q_OBJECT

public:
    explicit UserInfoWindow(QWidget *parent = nullptr);
    ~UserInfoWindow();
    
    
    
    void load_UserModule(UserModule* user_obj);
    
    //请求
    void getUserInfo();
    
    //判断是不是第一次载入用
    void showEvent(QShowEvent* event) override;
    

private:
    Ui::UserInfoWindow *ui;
    UserModule* user_obj = nullptr;
    
    //判断是不是第一次载入用
    bool isFirstLoad = true;
};

#endif // USERINFOWINDOW_H
