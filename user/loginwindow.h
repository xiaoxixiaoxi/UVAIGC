#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>
#include "UserModule/UserModule.hpp"
#include "../SettingSpanel/SettingSpanel_main.h"
#include "registerwndow.h"

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QDialog
{
    Q_OBJECT
    
signals:
    //去注册窗口
    void toRegisterWndow();

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    
    ~LoginWindow();
    
    void init_userModule(UserModule* user_obj);
    
    void login();
    void showpassword();
    void saveEmail();
    

private:
    Ui::LoginWindow *ui;
    UserModule* user_obj;
//    RegisterWndow registerWin;
    
    
    //记住邮箱 没有勾选 直接清空记录
    QString Key_QSettings_userEmail = "user_info/userEmail";
    QString Key_QSettings_isSaveEmail = "user_info/isSaveEmail";
    QSettings settings = QSettings(ORGANIZATION_Name, APPLICATION_Name);
};

#endif // LOGINWINDOW_H
