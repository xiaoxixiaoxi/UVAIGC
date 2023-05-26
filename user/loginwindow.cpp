#include "loginwindow.h"
#include "ui_loginwindow.h"
#include <QMessageBox>

LoginWindow::LoginWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    
    this->setWindowTitle(tr("登录"));
    
    
//    QString Key_QSettings_userName = "user_info/userName";
//    QSettings settings = QSettings(ORGANIZATION_Name, APPLICATION_Name);
    
    
    QString userEmail = settings.value(Key_QSettings_userEmail).toString();
    ui->emailLineEdit->setText(userEmail);
    
    bool isSaveEmail =  settings.value(Key_QSettings_isSaveEmail).toBool();
    ui->isSaveEmail_checkBox->setChecked(isSaveEmail);
    connect(ui->isSaveEmail_checkBox,&QPushButton::clicked, this, [=](){
            //保存
            bool isSaveEmail =  ui->isSaveEmail_checkBox->isChecked();
            settings.setValue(Key_QSettings_isSaveEmail,isSaveEmail);
        
    });
    
    
    
    
    
    ui->passwordLineEdit->setEchoMode(QLineEdit::Password);
    
    connect(ui->loginButton, &QPushButton::clicked, this, &LoginWindow::login);
    
    connect(ui->showpassword_checkBox,&QPushButton::clicked, this, &LoginWindow::showpassword);
    
   
    //绑定去注册
    connect(ui->toRegisterW_pushButton ,&QPushButton::clicked, this, [=](){
//        registerWin.exec();
//        this->hide();
        emit toRegisterWndow();
        
        
    });
    
    
    //找回密码
    LinkHandler::setLink( ui->resetpassword_label, "https://tools.yssck.com/resetpassword");
    
    
    
    

    
}

LoginWindow::~LoginWindow()
{
    delete ui;
}


void LoginWindow::init_userModule(UserModule* user_obj){
    this->user_obj = user_obj;
    
    //绑定登入信息
    //成功
    //失败
    QObject::connect( this->user_obj, &UserModule::loginSuccess,this, [=](const QString& token) {
            //qDebug() << "Login successful. Token: " << token;
            // 在这里可以进行后续操作，如跳转到其他界面或执行其他逻辑
        this->close();
//
        
        
        
        
        });

    QObject::connect( this->user_obj, &UserModule::loginFailure,this, [=](const QString& errorMessage) {
       // qDebug() << "Login failed. Error message: " << errorMessage;
        // 在这里可以处理登录失败的情况，如显示错误消息或执行其他逻辑
        
//        修改成弹出窗口
        QMessageBox::warning(this,
              tr("登录失败"),
              errorMessage,
              QMessageBox::Ok,
              QMessageBox::Ok);
        
        
//        ui->out_info_label->setText(errorMessage);
    });

    
    
}

//登入
void LoginWindow::login(){
    //判断 最后请求
    QString email = ui->emailLineEdit->text();
    QString password = ui->passwordLineEdit->text();
    
   
    
    if (email=="" || password=="" ) {
        
        QMessageBox::warning(this,
              tr("登录失败"),
              tr("邮箱或密码不能为空"),
              QMessageBox::Ok,
              QMessageBox::Ok);
        return;
    }
    //是否保留邮箱
    saveEmail();
    
    this->user_obj->login(email, password);
    
    
    
}


void LoginWindow::showpassword(){
    
    
    if (ui->showpassword_checkBox->isChecked()) {
        ui->passwordLineEdit->setEchoMode(QLineEdit::Normal);
        
        
    }else{
        
        ui->passwordLineEdit->setEchoMode(QLineEdit::Password);
        
    }
    
}

void LoginWindow::saveEmail(){
    
    if (ui->isSaveEmail_checkBox->isChecked()) {
        //保存
        QString email = ui->emailLineEdit->text();
        settings.setValue(Key_QSettings_userEmail,email);
        
    }else{
        //清除
        settings.setValue(Key_QSettings_userEmail,"");
    }
    
}
