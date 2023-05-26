#include "userinfowindow.h"
#include "ui_userinfowindow.h"
#include <QTimer>

UserInfoWindow::UserInfoWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserInfoWindow)
{
    ui->setupUi(this);
    qDebug()<<"加载";
    
    
    
    ui->SignOutpushButton->setVisible(false);
    
    ui->name_label->setText("");
    ui->email_label->setText("");
    ui->jb_label->setText("");
    
    
    
    //退出登录
    

}

UserInfoWindow::~UserInfoWindow()
{
    delete ui;
}

void UserInfoWindow::load_UserModule(UserModule* user_obj){
    this->user_obj = user_obj;
    
    
    //绑定登入信息
    //成功
    //失败
    QObject::connect( this->user_obj, &UserModule::getUserInfoSuccess,this, [=](const QString& username,const QString& email,const QString& balance) {
        
        ui->name_label->setText(username);
        ui->email_label->setText(email);
        ui->jb_label->setText(balance);
        });
    
    
    //注销
    connect(this->user_obj, &UserModule::signOutSuccess, this, [=](){
        ui->name_label->setText("");
        ui->email_label->setText("");
        ui->jb_label->setText("");
        
    });
 
    
};




void UserInfoWindow::getUserInfo(){
    
    this->user_obj->getWebUserInfo();
    
}


void UserInfoWindow::showEvent(QShowEvent* event)
{
    if (isFirstLoad)
       {
           qDebug()<< "第一次";
           // 第一次载入的处理逻辑
           isFirstLoad = false;
           
           QTimer::singleShot(1, this, &UserInfoWindow::getUserInfo);
       }


    QWidget::showEvent(event);
}

