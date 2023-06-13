//
//  UserModule.hpp
//  UVAIGC
//
//  Created by 林溪明 on 2023/5/15.
//

#ifndef UserModule_hpp
#define UserModule_hpp

#include <stdio.h>

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>

#include "../../SettingSpanel/Settings_info.hpp"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QSettings>

class UserModule : public QObject {
    Q_OBJECT

public:
    explicit UserModule(QObject* parent = nullptr);
//登入
    void login(const QString& email, const QString& password);
    //注册获取验证码
    void getRegisterCode(const QString& username, const QString& email);
    
    //注册提交
    void emailRegister(const QString& username, const QString& email,const QString& mailcode,const QString& password,const QString& invite);
    
    
    //注册
    // 获取登入状态
    bool isLoggedIn() const;

    // 获取会员状态
    bool getIsPro() const;
    
    //请求会员信息
    void getWebIsPro();
    
    //请求用户基本信息
    void getWebUserInfo();
    //退出登录
    void signOut();
    
    //请求所有会员信息
    void getApppermission();
    //刷新token
    void postTokenRefresh();
    //请求设备管理
    void getWebDevicelist();
    //删除设备 同步
    QNetworkReply* delWebDevice(int id);
    
    
    
    
    
    
    
    
    

    // 获取用户名称
    QString getUsername() const;

    // 获取用户邮箱
    QString getEmail() const;
    
    //向服务器请求权限 同步
    void getWebSuperMember();
    void getWebTool_AVS_Permission();
    
    
  
    
    //权限接口
    bool isSuperMember() const;
    bool hasTool_AVS_Permission() const;
    QDateTime get_time_outdata()const;
    
    //请求用的 控制设备在线数量
    QString device_uuid;
    QString device_name;
    QString client_type;
    
    //请求前执行函数
    void start_netSignals();


signals:
    
    //请求错误通用处理
    void net_errorFailure(const QString& errorMessage);
    void net_error403Failure(const QString& errorMessage);
    
    
    
    //登入完成
    void loginSuccess(const QString& token);
    //登入失败
    void loginFailure(const QString& errorMessage);
    
   //退出成功信号
    void signOutSuccess();
    
    //请求是不是pro成功
    void getIsProSuccess();
    void getIsProFailure(const QString& errorMessage);
    
    //请求用户基本信息
    void getUserInfoSuccess(const QString& username,const QString& email,const QString& balance);
    void getUserInfoFailure(const QString& errorMessage);
    
    //请求邮箱验证码是否完成
    void getRegisterCodeSuccess();
    void getRegisterCodeFailure(const QString& errorMessage);
    
    
    void emailRegisterCodeSuccess();
    void emailRegisterFailure(const QString& errorMessage);
    
    
    //所有开通会员信息请求成功
    void apppermissionCodeSuccess(const QJsonArray& dataArray);
    void apppermissionFailure(const QString& errorMessage);
    
    //token刷新成功
    void tokenRefreshSuccess();
    void tokenRefreshFailure(const QString& errorMessage);
    
    //请求设备管理
    void getWebDevicelistSuccess(const QJsonArray& dataArray);
    void getWebDevicelistFailure(const QString& errorMessage);

    
    
    
    // 定义一个信号。权限有变化就通知
    void UpdataDataSig();
    
    
    
    //定义一个更新ui进度用的 开始请求 和 结束请求
    void start_qProgressSig();//开启
    void end_qProgressSig();//结束
    
    
    
    //失败
    
    //注册

//    bool hasToolBPermission() const;
    

private slots:
    
    //所有响应处理。再根据不同路径去分发处理结果
    void handleALLResponse(QNetworkReply* reply);
    
    
    
    // 处理登入请求的响应
    void handleLoginResponse(QNetworkReply* reply);
    //会员请求返回的信息
    void handleGetIsProResponse(QNetworkReply* reply);
    //用户基本信息
    void handleGetUserInfoResponse(QNetworkReply* reply);
    
    //请求邮箱验证码
    void handlegetRegisterCodeResponse(QNetworkReply* reply);
    void handlegetEmailRegisterResponse(QNetworkReply* reply);
    
    //请求所有会员
    void handlegetAppPermissionResponse(QNetworkReply* reply);
    //刷新token
    void handleTokenRefreshResponse(QNetworkReply* reply);
    
    
    void handleGetWebDevicelistResponse(QNetworkReply* reply);
//    void handleDelWebDeviceResponse(QNetworkReply* reply);
    //..注册 会员.. 响应

private:
    QNetworkAccessManager networkManager;
   
//解析json
    QString parseTokenFromResponse(const QJsonObject& responseObject);
    QString parseErrorMessageFromResponse(const QJsonObject& responseObject);
    
    
    
    //理由管理
    // 请求地址常量
    static const QString WEB_ROOT_URL;
   static const QString LOGIN_API_URL;
//   static const QString REGISTER_API_URL;
    //请求会员
   static const QString GET_ISPRO_API_URL;
   static const QString GET_U_INFO_API_URL;
    
    //请求邮箱验证码
   static const QString REGISTER_GETCODE_API_URL;
   static const QString REGISTER_API_URL;
    
    
   static const QString AppPermission_API_URL;
   static const QString TokenRefresh_API_URL;
    
   static const QString GET_Tool_AVS_Permission_API_URL;
   static const QString GET_UVAIGC_SuperMember_API_URL;
    
   static const QString GET_Devicelist_API_URL;
    
    //获取本地唯一id
    QString create_device_uuid();
    //保存token 同一管理
    void saveAuthToken(const QString& token);
    QString readAuthToken();
    //加密字符串
    QString encryptToken(const QString& token, const QString& key);
    QString decryptToken(const QString& encryptedToken, const QString& key);
    //解码字符串
    
    
    //用户信息
    bool loggedIn = false;//是否登入
    QString authToken; //
    
    //用户权限控制是不是会员
    bool m_isSuperMember = false;
    //到期时间
    QDateTime time_out_dateTime;
    
    bool m_hasTool_AVS_Permission;
//    bool m_hasToolBPermission;
    //TODO: 本地缓存。提供离线临时使用
    
    
    //用户名称
    QString userName;
    QString userEmail;
    QString userpassword;
    

    
    
    //信息保存键
    //记住邮箱 没有勾选 直接清空记录
    QString Key_QSettings_authToken = "user_info/authToken";
    QSettings settings = QSettings(ORGANIZATION_Name, APPLICATION_Name);
    
    
    
};

#endif /* UserModule_hpp */
