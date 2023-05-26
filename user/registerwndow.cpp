#include "registerwndow.h"
#include "ui_registerwndow.h"
#include <QMessageBox>
#include <QRegularExpressionValidator>

#include <QLabel>
#include <QDesktopServices>
#include <QUrl>
#include <QMouseEvent>

//添加超链接扩展————————————————————
LinkHandler::LinkHandler(QObject* parent) : QObject(parent) {}

void  LinkHandler::setLink(QLabel* label, const QString& link) {
        QString test = label->text();
        label->setText(QString("<a href=\"%1\">%2</a>").arg(link,test));
        label->setTextInteractionFlags(Qt::TextBrowserInteraction);
        label->setOpenExternalLinks(true);
        label->installEventFilter(new LinkHandler(label));
    }


bool LinkHandler::eventFilter(QObject* obj, QEvent* event) {
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->buttons() & Qt::LeftButton) {
            QLabel* label = static_cast<QLabel*>(obj);
            
//            qDebug()<<label->text();
            // BUG: 第一次打开网页,能正常打开. 再次点击链接,就没有反应
            QDesktopServices::openUrl(QUrl(label->text()));
            return true;
        }
    }
    return QObject::eventFilter(obj, event);
}
//添加超链接扩展————————————————————


RegisterWndow::RegisterWndow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterWndow)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("注册"));
    
    //验证
//    ui->email_lineEdit->setValidator(new QRegularExpressionValidator(QRegularExpression(R"(\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}\b)"), this));
    // 连接按钮的点击事件到槽函数
    connect(ui->send_email_pushButton, &QPushButton::clicked, this, &RegisterWndow::getWebEmailCodeClicked);
    connect(ui->register_pushButton, &QPushButton::clicked, this, &RegisterWndow::registerClicked);
    connect(ui->to_longWinpushButton, &QPushButton::clicked, this, [=](){
        //去登入窗口
        emit toLoginWindow();
       
        
        
    });
    
   
    LinkHandler::setLink( ui->link_tk_label, "https://www.yssck.com/%e7%94%a8%e6%88%b7%e5%8d%8f%e8%ae%ae");
    
    

    
    
}

RegisterWndow::~RegisterWndow()
{
    delete ui;
}



void RegisterWndow::init_userModule(UserModule* user_obj){
    this->user_obj = user_obj;
    
//    注册信号绑定
    QObject::connect( this->user_obj, &UserModule::getRegisterCodeSuccess,this, [=]() {
           //提示成功
        QMessageBox msgBox;
        msgBox.setText(tr("验证码发送成功"));
        msgBox.exec();
        
        });

    QObject::connect( this->user_obj, &UserModule::getRegisterCodeFailure,this, [=](const QString& errorMessage) {

        
//        修改成弹出窗口
        QMessageBox::warning(this,
              tr("验证码发送失败"),
                             tr("验证码发送失败") + "\n\n" +errorMessage,
              QMessageBox::Ok,
              QMessageBox::Ok);
    });
    
    
    
    //绑定注册
    
    QObject::connect( this->user_obj, &UserModule::emailRegisterCodeSuccess,this, [=]() {
           //提示成功
        QMessageBox msgBox;
        msgBox.setText(tr("注册成功"));
        msgBox.exec();
        
        //关闭
        emit toLoginWindow();
        
        });

    QObject::connect( this->user_obj, &UserModule::emailRegisterFailure,this, [=](const QString& errorMessage) {
        
        
//        修改成弹出窗口
        QMessageBox::warning(this,
              tr("注册失败"),
                             tr("注册失败") + "\n\n" +errorMessage,
              QMessageBox::Ok,
              QMessageBox::Ok);
    });
    

    
    
}


//验证是不是符合基本要求
bool RegisterWndow::verify_format(){
    // 获取输入框中的数据
       QString name = ui->name_lineEdit->text();
       QString email = ui->email_lineEdit->text();
    QString password = ui->password_lineEdit->text();
       QString confirmPassword = ui->password_2_lineEdit->text();
//       QString verificationCode = verificationCodeLineEdit->text();

    // 验证输入
   if (name.isEmpty() || email.isEmpty() || password.isEmpty() || confirmPassword.isEmpty()) {
       QMessageBox::critical(this,tr("错误!"), tr("错误!")+"\n\n" + tr("输入不能为空"));
       return false;
   }
    //邮箱格式验证
    
    QRegularExpression emailRegex(R"(^\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}\b$)");
    QRegularExpressionMatch match = emailRegex.match(email);
    
    if (!match.hasMatch()) {
        QMessageBox::critical(this,tr("错误!"), tr("错误!")+"\n\n" + tr("邮箱格式不对,请重新输入"));
        return false;
    }
    
    //验证密码要大于6位数
    
    
    


   if (password != confirmPassword) {
       QMessageBox::critical(this,tr("错误!"), tr("错误!")+"\n\n" + tr("2次输入密码不一致!"));
//           QMessageBox::critical(this, "Error", "Passwords do not match.");
       return false;
   }
    
    if(password.size() < 6){
        QMessageBox::critical(this,tr("错误!"), tr("错误!")+"\n\n" + tr("请输入6-20个非空白字符密码"));
 //           QMessageBox::critical(this, "Error", "Passwords do not match.");
        return false;
        
    }
    
    
    return true;
    
}

//获取邮箱验证码
void RegisterWndow::getWebEmailCodeClicked(){
    if (!verify_format()) {
        return;
    }
    
    //向服务器请求邮箱验证
    QString name = ui->name_lineEdit->text();
    QString email = ui->email_lineEdit->text();
    user_obj->getRegisterCode(name, email);
    
}

void RegisterWndow::registerClicked(){
    
    if (!verify_format()) {
        return;
    }
    QString mailcode = ui->emailCode_lineEdit->text();
    if (mailcode.isEmpty()) {
        QMessageBox::critical(this,tr("错误!"), tr("错误!")+"\n\n" + tr("邮箱验证码不能为空"));
        return;
    }
    
    // 是否勾选已经
    if (!ui->checkBox->isChecked()){
        QMessageBox::critical(this,tr("错误!"), tr("错误!")+"\n\n" + tr("需要勾选注册协议和条款"));
        return;
    }
    
    //获取参数
    
    QString name = ui->name_lineEdit->text();
    QString email = ui->email_lineEdit->text();
    QString password = ui->password_lineEdit->text();
    QString invite = ui->Invitation_code_lineEdit->text();
    
    user_obj->emailRegister(name, email, mailcode, password, invite);
    

    
    
    
}
