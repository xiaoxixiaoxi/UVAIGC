#ifndef USERMAINWINDOW_H
#define USERMAINWINDOW_H

#include <QWidget>
#include "UserModule/UserModule.hpp"
#include "loginwindow.h"

namespace Ui {
class UserMainWindow;
}

class UserMainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit UserMainWindow(QWidget *parent = nullptr);

    ~UserMainWindow();

    void init_UserModule();
    
    void toLoginWindow();
    void toRegisterWndow();
//    void toSignOut();
    void upadtaMyUi();
    
    void getUserInfo();



private:
    Ui::UserMainWindow *ui;
    UserModule* user_obj;
    LoginWindow loginwin;
    
    RegisterWndow registerWin;
};

#endif // USERMAINWINDOW_H
