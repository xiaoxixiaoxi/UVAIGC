#include "UserMainWindow.h"
#include "ui_UserMainWindow.h"
#include <QMessageBox>
#include <QTimer>



UserMainWindow::UserMainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserMainWindow)
{
    ui->setupUi(this);
    
    //等待动画
    pIndicator = new QProgressIndicator(this);
    pIndicator->setColor(Qt::gray);
    pIndicator->startAnimation();
    pIndicator->setVisible(false);
    
    ui->qProg_horizontalLayout->addWidget(pIndicator);
//    ui->qProg_widget->setParent(pIndicator);
    
//    ui->qProg_widget
    
    
    init_UserModule();
    
    
    
    
    
    connect(ui->loan_pushButton, &QPushButton::clicked, this,[=](){
        //登入按钮点击
        if (user_obj!=nullptr){
            if (user_obj->isLoggedIn()){
                user_obj->signOut();

               
            }else{
                
                toLoginWindow();
//
            }
            
        }
       
        
    } );
    connect(ui->re_pushButton, &QPushButton::clicked, this, &UserMainWindow::toRegisterWndow);
    
    // 连接 currentChanged 信号
//    connect(ui->member_tab, &QTabWidget::currentChanged, this, [=](){
//        qDebug()<<"第一次加载..";
//    });





}






UserMainWindow::~UserMainWindow()
{
    delete ui;
}


//请求用户信息。 被拒绝 就弹出登入


void UserMainWindow::init_UserModule(){
//    this->user_obj = new UserModule();
    this->user_obj = MyUserModule_obj;
    
    ui->userInfoWidget->load_UserModule(this->user_obj);
    ui->memberWidget->load_UserModule(this->user_obj);
    
    loginwin.init_userModule(this->user_obj);
    registerWin.init_userModule(this->user_obj);
    
    //绑定ui等待
    QObject::connect( this->user_obj,&UserModule::end_qProgressSig,this,[=](){
        //结束
        pIndicator->setVisible(false);
        
    });
    QObject::connect( this->user_obj,&UserModule::start_qProgressSig,this,[=](){
        //开始
        pIndicator->setVisible(true);
//        pIndicator->startAnimation();
    });
    
    
    
    //窗口绑定
    connect(&loginwin, &LoginWindow::toRegisterWndow, this,[=](){
        loginwin.accept();
        QTimer::singleShot(0, this, &UserMainWindow::toRegisterWndow);

       
    });
    connect(&registerWin, &RegisterWndow::toLoginWindow, this,[=](){
        registerWin.accept();
        QTimer::singleShot(0, this, &UserMainWindow::toLoginWindow);
        
    });
    
    
    //更新通知
    QObject::connect( this->user_obj,&UserModule::UpdataDataSig,this,[=](){
        this->upadtaMyUi();
    });
    
    //退出成功按钮提示
    connect( this->user_obj,&UserModule::signOutSuccess,this,[=](){
        QMessageBox msgBox;
        msgBox.setText(tr("注销成功"));
        msgBox.exec();
        
    });
    
    
//    net_errorFailure
    QObject::connect( this->user_obj, &UserModule::net_errorFailure,this, [=](const QString& errorMessage) {
       
        //弹出提示
        QMessageBox::warning(this,
              tr("网络请求错误"),
              tr("网络请求错误") + "\n" + errorMessage,
              QMessageBox::Ok,
              QMessageBox::Ok);
        

        
       
    });
    
    QObject::connect( this->user_obj, &UserModule::net_error403Failure,this, [=](const QString& errorMessage) {
       
        //弹出提示
        QMessageBox::warning(this,
              tr("服务器拒绝请求!尝试重新登录"),
              tr("服务器拒绝请求!尝试重新登录") + "\n" + errorMessage,
              QMessageBox::Ok,
              QMessageBox::Ok);
        //加载获取信息 错误
        toLoginWindow();
        

        
       
    });
    
    //TODO: 网络错误处理
    QObject::connect( this->user_obj, &UserModule::getUserInfoFailure,this, [=](const QString& errorMessage) {
       
        //弹出提示
        QMessageBox::warning(this,
              tr("获取信息失败"),
              tr("需要重新登录才能继续,"),
              QMessageBox::Ok,
              QMessageBox::Ok);
        
        //加载获取信息 错误
        toLoginWindow();
        
       
    });
    
    upadtaMyUi();
    
}



void UserMainWindow::toLoginWindow(){
    
    
//    registerWin.init_userModule(this->user_obj);
//    registerWin.exec();
//
//    return;
    
    loginwin.setWindowModality(Qt::ApplicationModal);
    this->loginwin.exec();
    
    if (this->user_obj->isLoggedIn()){
//        ui->label->setText("登录成功");
        ui->userInfoWidget->getUserInfo();
    }else{
//        ui->label->setText("登录失败");
    }
    
}

void UserMainWindow::toRegisterWndow(){
    
    registerWin.setWindowModality(Qt::ApplicationModal);
    registerWin.exec();
    
    
    return;
    
}





void UserMainWindow::getUserInfo(){
    ui->userInfoWidget->getUserInfo();
    
}


void UserMainWindow::upadtaMyUi(){
    
    if (this->user_obj == nullptr) {
        return;
    }
    
    
    if (user_obj->isLoggedIn()){
        ui->loan_pushButton->setText(tr("注销"));
        ui->re_pushButton->setVisible(false);
        
    }else{
        ui->loan_pushButton->setText(tr("登录"));
        ui->re_pushButton->setVisible(true);
       
       
//        toLoginWindow();
    }
    
    
}
