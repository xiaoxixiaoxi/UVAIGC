#ifndef REGISTERWNDOW_H
#define REGISTERWNDOW_H

#include <QDialog>

#include "UserModule/UserModule.hpp"
#include <QLabel>

namespace Ui {
class RegisterWndow;
}

class RegisterWndow : public QDialog
{
    Q_OBJECT
    
signals:
    //去注册窗口
    void toLoginWindow();

public:
    explicit RegisterWndow(QWidget *parent = nullptr);
    ~RegisterWndow();
    
    void init_userModule(UserModule* user_obj);
    
    
    //验证是不是符合基本要求
    bool verify_format();
    
    //获取邮箱验证码
    void getWebEmailCodeClicked();
    
    void registerClicked();

private:
    Ui::RegisterWndow *ui;
    UserModule* user_obj = nullptr;
    
};



class LinkHandler : public QObject {
    Q_OBJECT
public:
    LinkHandler(QObject* parent = nullptr);
    static void setLink(QLabel* label, const QString& link);
protected:
    bool eventFilter(QObject* obj, QEvent* event) override;
};




#endif // REGISTERWNDOW_H
