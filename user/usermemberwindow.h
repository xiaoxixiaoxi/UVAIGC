#ifndef USERMEMBERWINDOW_H
#define USERMEMBERWINDOW_H

#include <QWidget>
#include "UserModule/UserModule.hpp"
#include "UserDeviceCustomDialog.hpp"

namespace Ui {
class UserMemberWindow;
}

class UserMemberWindow : public QWidget
{
    Q_OBJECT

public:
    explicit UserMemberWindow(QWidget *parent = nullptr);
    ~UserMemberWindow();
    
    void load_UserModule(UserModule* user_obj);
    
    //请求会员信息
    void getWeb();
    
    //判断是不是第一次载入用
    void showEvent(QShowEvent* event) override;
    //同步界面显示
    void upadtaMyUi();
    //刷新tonken
    void tokenRefresh();
    //请求会员检查
    void isPro();
    
    
    
    

private slots:
    void on_to_openPro_pushButton_clicked();

private:
    Ui::UserMemberWindow *ui;
    UserModule* user_obj = nullptr;
    //判断是不是第一次载入用
    bool isFirstLoad = true;
    
    //自定义列表窗口
    CustomDialog deviceDialog;
};

#endif // USERMEMBERWINDOW_H
