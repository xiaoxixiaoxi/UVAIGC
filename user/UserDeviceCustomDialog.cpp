//
//  UserDeviceCustomDialog.cpp
//  UVAIGC
//
//  Created by 林溪明 on 2023/5/21.
//

#include "UserDeviceCustomDialog.hpp"

#include <QLabel>
#include <QPushButton>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QMessageBox>

CustomDialog::CustomDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle(tr("设备管理"));
    layout = new QVBoxLayout(this);
    setLayout(layout);
    
    
}

void CustomDialog::onUnbindButtonClicked(int deviceId, QString device_uuid) {
    unbindDevice(deviceId,device_uuid);
    
    
    
}

void CustomDialog::loadDeviceRecords(const QJsonArray& dataArray) {
    // TODO: Perform your network request here and replace the following code with your implementation
    
    int rowCount = dataArray.size();
    
    
    // Dummy code to populate the device records
    for (int i = 0; i < rowCount; i++) {
        
        QJsonObject itemData = dataArray[i].toObject();
        
        QString device_uuid = itemData["device_uuid"].toString();
        QString device_name = itemData["device_name"].toString();
        // 将字符串转换为 QDateTime 对象
        QDateTime dateTime = QDateTime::fromString(itemData["created"].toString(), Qt::ISODateWithMs);
        // 将日期时间对象格式化为指定格式的字符串
        QString formattedDateTime = dateTime.toString("yyyy-MM-dd hh:mm:ss");
        
        QString name = device_name;
        int id = itemData["id"].toInt();
        
        QHBoxLayout *rowLayout = new QHBoxLayout();
        
        QLabel *deviceLabel = new QLabel(name);
        QPushButton *unbindButton = new QPushButton(tr("解绑"));
        QLabel *localLabel = new QLabel(tr("本机"));
        QLabel *data_label = new QLabel(formattedDateTime);
        
        unbindButton->setProperty("deviceId", id);
        unbindButton->setProperty("device_uuid", device_uuid);
        localLabel->setVisible(device_uuid == this->user_obj->device_uuid); // Only show the "本机" label for the first row
        
        connect(unbindButton, &QPushButton::clicked, [=]() {
            int deviceId = unbindButton->property("deviceId").toInt();
            QString device_uuid = unbindButton->property("device_uuid").toString();
            onUnbindButtonClicked(deviceId,device_uuid);
        });
        
        rowLayout->addWidget(deviceLabel);
        rowLayout->addWidget(data_label);
        rowLayout->addStretch(); // Add stretch to push the "本机" label to the right
       
        rowLayout->addWidget(localLabel);
        rowLayout->addWidget(unbindButton);
        
        layout->addLayout(rowLayout);
    }
}

void CustomDialog::unbindDevice(int deviceId, QString device_uuid) {
    // TODO: Perform your unbind device request here and replace the following code with your implementation
    auto* reply = this->user_obj->delWebDevice(deviceId);
    
    if (reply->error() == QNetworkReply::NoError) {
        // 请求成功，处理响应数据
        //逻辑处理
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
        QJsonObject responseObject = jsonResponse.object();
        
        //根据不同请求路径在这边分发不同请求结果处理
        QUrl requestUrl = reply->request().url();

        int code = responseObject["code"].toInt();
        //正确
        if (code == 0) {
            //判断是不是本机
            
            QString info=tr("解除成功");
            if(device_uuid == this->user_obj->device_uuid){
                info += "\n" + tr("注销成功");
                this->user_obj->signOut();
                
            }
            //提示成功
            QMessageBox msgBox;
            msgBox.setText(info);
            msgBox.exec();
            
            //重新刷新
            refreshDeviceRecords();

            
        } else {
//            错误
            QString errorMessage = responseObject["msg"].toString();
            //弹出窗口
            QMessageBox::warning(this,
                  tr("解绑失败"),
                                 tr("解绑失败") + "\n\n" +errorMessage,
                  QMessageBox::Ok,
                  QMessageBox::Ok);
           
            
        }
    } else {
        // 请求出错，打印错误信息
//        qDebug() << "Request error: " << reply->errorString();
        QMessageBox::warning(this,
              tr("解绑失败"),
                             tr("解绑失败") + "\n\n" + reply->errorString(),
              QMessageBox::Ok,
              QMessageBox::Ok);
        
    }
    // 清理资源
    reply->deleteLater();
    
}


void CustomDialog::load_UserModule(UserModule* user_obj){
    this->user_obj = user_obj;
    
    
   

    connect(this->user_obj, &UserModule::getWebDevicelistSuccess, this, [=](const QJsonArray& dataArray) {
        
        loadDeviceRecords(dataArray);
    });

   
    
}


// 递归清理子布局
void CTD__clearLayout(QLayout* layout) {
    QLayoutItem* item;
    while ((item = layout->takeAt(0)) != nullptr) {
        if (item->layout()) {
            CTD__clearLayout(item->layout());
        }
        delete item->widget();
        delete item;
    }
}
//刷新
void CustomDialog::refreshDeviceRecords() {
    // 清空布局中的所有控件
    // 清空布局中的所有控件
    QLayoutItem *child;
    while ((child = layout->takeAt(0)) != nullptr) {
        if (child->layout()) {
            CTD__clearLayout(child->layout()); // 递归清理子布局
        }
        delete child->widget();
        delete child;
    }
    layout->update();
    
    // 重新请求网络数据，并创建新的按钮和标签
    this->user_obj->getWebDevicelist();
}
