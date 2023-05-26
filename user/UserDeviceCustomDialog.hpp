//
//  UserDeviceCustomDialog.hpp
//  UVAIGC
//
//  Created by 林溪明 on 2023/5/21.
//

#ifndef UserDeviceCustomDialog_hpp
#define UserDeviceCustomDialog_hpp

#include <stdio.h>

#include <QWidget>
#include <QDialog>
#include <QVBoxLayout>
#include "UserModule/UserModule.hpp"

class CustomDialog : public QDialog {
    Q_OBJECT
    
public:
    CustomDialog(QWidget *parent = nullptr);
    
    void load_UserModule(UserModule* user_obj);
    //刷新加载
    void refreshDeviceRecords();
    
private slots:
    void onUnbindButtonClicked(int deviceId, QString device_uuid);
    
private:
    QVBoxLayout *layout;
    
    void loadDeviceRecords(const QJsonArray& dataArray);
    void unbindDevice(int deviceId, QString device_uuid);
    
  
    
    UserModule* user_obj = nullptr;
    
};


#endif /* UserDeviceCustomDialog_hpp */
