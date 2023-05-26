#include "usermemberwindow.h"
#include "ui_usermemberwindow.h"
#include <QDateTime>
#include <QTimer>
#include <QMessageBox>
#include <QDesktopServices>



UserMemberWindow::UserMemberWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserMemberWindow)
{
    ui->setupUi(this);
    connect(ui->up_pushButton, &QPushButton::clicked, this, [=](){
        ui->tableWidget->clear();
        getWeb();
    });
    
    connect(ui->tokenRefresh_pushButton, &QPushButton::clicked, this, [=](){
        tokenRefresh();
    });
    
    
    //设备管理
    connect(ui->device_pushButton, &QPushButton::clicked, this, [=](){
        deviceDialog.refreshDeviceRecords();
        deviceDialog.exec();
    });
    
    
    // 隐藏控件到期时间
    ui->time_out_widget->setVisible(false);
    
    //隐藏所有会员信息表
    ui->tableWidget->setVisible(false);
    ui->up_pushButton->setVisible(false);
    
    
    
    

}

UserMemberWindow::~UserMemberWindow()
{
    delete ui;
}

void UserMemberWindow::load_UserModule(UserModule* user_obj){
    this->user_obj = user_obj;
    
    deviceDialog.load_UserModule(user_obj);
    //绑定登入信息
    //成功
    //失败
    QObject::connect( this->user_obj, &UserModule::apppermissionCodeSuccess,this, [=](const QJsonArray& dataArray) {
        
        
        int rowCount = dataArray.size();
        int columnCount = 3;// /* 设置列数 */;

       
        // 设置水平表头的调整模式为 Stretch
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        // 设置表头
        QStringList headers = {tr("会员类型名称"),tr("距离到期天数"),tr("到期时间")};// /* 设置表头标题 */;
        ui->tableWidget->setColumnCount(columnCount);
        ui->tableWidget->setHorizontalHeaderLabels(headers);
        ui->tableWidget->setRowCount(rowCount);
        
        for (int i = 0; i < rowCount; ++i)
        {
            QJsonObject itemData = dataArray[i].toObject();
            for (int j = 0; j < columnCount; ++j)
            {
                // 获取数据并填充到单元格
                QString cellData = "";
             
                if (j==0){
                    cellData = itemData["tooltype"].toString();
                }else if (j==1){
                    
                    // 获取当前系统时间
                    QDateTime currentDateTime = QDateTime::currentDateTime();
                    QString dateString = itemData["time_out"].toString();
                    // 将字符串转换为 QDateTime 对象
                    QDateTime targetDateTime = QDateTime::fromString(dateString, Qt::ISODateWithMs);
                    // 计算日期差异
                    int daysDiff = currentDateTime.date().daysTo(targetDateTime.date());
                    if (daysDiff > 0) {
                        cellData = QString::number(daysDiff);
                    }else{
                        cellData = tr("已到期");
                    }
                    
            
                    
                }else if (j==2){
                    
                    
                    // 将字符串转换为 QDateTime 对象
                    QDateTime dateTime = QDateTime::fromString(itemData["time_out"].toString(), Qt::ISODateWithMs);

                    // 将日期时间对象格式化为指定格式的字符串
                    QString formattedDateTime = dateTime.toString("yyyy-MM-dd hh:mm:ss");
                    
                    
                    cellData = formattedDateTime;
                }
               
                QTableWidgetItem* item = new QTableWidgetItem(cellData);
                
                ui->tableWidget->setItem(i, j, item);
            }
        }
        
        
        
        });
 
    QObject::connect( this->user_obj, &UserModule::UpdataDataSig,this,&UserMemberWindow::upadtaMyUi);
    
    //token刷新成功
    QObject::connect( this->user_obj, &UserModule::tokenRefreshSuccess,this, [=](){
        
        isPro();
        
    });
    //TODO: 错误处理
    
    
    //失败绑定
    
    
};


void UserMemberWindow::upadtaMyUi(){
    
    if (this->user_obj == nullptr) {
        return;
    }
    
    
    
    if (this->user_obj->isSuperMember()){
        ui->isPro_label->setText(tr("专业版"));
        // 显示控件
        
        
        ui->time_out_widget->setVisible(true);
        auto dateTime = this->user_obj->get_time_outdata();
        // 将日期时间对象格式化为指定格式的字符串
        QString formattedDateTime = dateTime.toString("yyyy-MM-dd hh:mm:ss");
        ui->time_out_label->setText(formattedDateTime);
        
        
        
        //窗口提示一下
//        QMessageBox msgBox;
//        msgBox.setText(tr("状态变成:") +"\n\n" + tr("专业版"));
//        msgBox.exec();
        
    }else{
        ui->isPro_label->setText(tr("免费版"));
       
        // 隐藏控件
        ui->time_out_widget->setVisible(false);
        
    }
    
    
    
    
}




void UserMemberWindow::getWeb(){
    
    this->user_obj->getApppermission();
    
    
    
}
void UserMemberWindow::isPro(){
    
    this->user_obj->getWebSuperMember();
}


void UserMemberWindow::tokenRefresh(){
    
    this->user_obj->postTokenRefresh();
}


void UserMemberWindow::showEvent(QShowEvent* event)
{
    if (isFirstLoad)
       {
           qDebug()<< "第一次";
           // 第一次载入的处理逻辑
           isFirstLoad = false;
           
           //延迟一小会
//           QTimer::singleShot(1, this, &UserMemberWindow::getWeb);
           QTimer::singleShot(1, this, &UserMemberWindow::isPro);

       }


    QWidget::showEvent(event);
}

//跳转到开通会员路径
void UserMemberWindow::on_to_openPro_pushButton_clicked()
{
    
    QDesktopServices::openUrl(QUrl(QString::fromStdString(GETPRO_URL)));


}

