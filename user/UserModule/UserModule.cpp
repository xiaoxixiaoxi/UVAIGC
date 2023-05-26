//
//  UserModule.cpp
//  UVAIGC
//
//  Created by 林溪明 on 2023/5/15.
//

#include "UserModule.hpp"
#include <QTimer>
#include <QSysInfo>
#include <QCoreApplication>



const QString UserModule::WEB_ROOT_URL =  "https://api.yssck.com/";
//const QString UserModule::WEB_ROOT_URL =  "http://192.168.31.6:8000/";
const QString UserModule::LOGIN_API_URL = WEB_ROOT_URL + "api/v2/user/login/";



const QString UserModule::REGISTER_GETCODE_API_URL = WEB_ROOT_URL + "api/v1/user/mailcode/";
const QString UserModule::REGISTER_API_URL = WEB_ROOT_URL + "api/v2/user/signin/";

//请求是不是会员
const QString UserModule::GET_ISPRO_API_URL = WEB_ROOT_URL + "api/v1/shop/convert/srt2pr/";
const QString UserModule::GET_U_INFO_API_URL = WEB_ROOT_URL + "api/v2/user/user/";

//所有会员信息
const QString UserModule::AppPermission_API_URL = WEB_ROOT_URL + "api/v1/shop/convert/apppermission/";

const QString UserModule::TokenRefresh_API_URL = WEB_ROOT_URL + "api/v2/user/token/refresh/";

const QString UserModule::GET_Tool_AVS_Permission_API_URL = WEB_ROOT_URL + "api/v1/shop/convert/apppermission/";
const QString UserModule::GET_UVAIGC_SuperMember_API_URL = WEB_ROOT_URL + "api/v1/shop/uvaigcispro/";

//设备管理
const QString UserModule::GET_Devicelist_API_URL = WEB_ROOT_URL + "api/v1/user/device/";



//apppermission


UserModule::UserModule(QObject* parent) : QObject(parent) {
    
    
    
    
    client_type = CLIENT_TYPE_USK;
    device_name = QSysInfo::machineHostName();
    device_uuid = create_device_uuid();
    
    // 读出储存的 需要用到uuid。一定要先加载uuid 再去获取token
    this->authToken = readAuthToken();
    connect(&networkManager, &QNetworkAccessManager::finished, this, &UserModule::handleALLResponse);
    
    getWebSuperMember();
    
}



void UserModule::login(const QString& email, const QString& password) {
    
    this->userEmail = email;
    this->userpassword = password;
    
    
    QJsonObject requestBody;
    requestBody["email"] = email;
    requestBody["password"] = password;
    requestBody["device_uuid"] = device_uuid;
    requestBody["client_type"] = client_type;
    requestBody["device_name"] = device_name;
    
    QJsonDocument jsonDocument(requestBody);
    QByteArray requestData = jsonDocument.toJson();

    QNetworkRequest request(LOGIN_API_URL);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    networkManager.post(request, requestData);
}

void UserModule::handleALLResponse(QNetworkReply* reply) {
    if (reply->error() == QNetworkReply::NoError) {
        
        
        
        QUrl requestUrl = reply->request().url();
//        qDebug() << requestUrl;
        if (requestUrl == QUrl(LOGIN_API_URL)) {
            // 处理登入请求的响应
            handleLoginResponse(reply);
            
        } else if (requestUrl == QUrl(GET_ISPRO_API_URL)) {
            // 是不会会员处理
            handleGetIsProResponse(reply);
            
        }
        else if (requestUrl == QUrl(GET_U_INFO_API_URL)) {
            // 基本信息
            handleGetUserInfoResponse(reply);
            
        }else if (requestUrl == QUrl(REGISTER_GETCODE_API_URL)){
            handlegetRegisterCodeResponse(reply);
        }
        else if (requestUrl == QUrl(REGISTER_API_URL)){
            handlegetEmailRegisterResponse(reply);
        }else if (requestUrl == QUrl(AppPermission_API_URL)){
            handlegetAppPermissionResponse(reply);
        }else if (requestUrl == QUrl(TokenRefresh_API_URL)){
            handleTokenRefreshResponse(reply);
        }else if (requestUrl == QUrl(GET_Devicelist_API_URL +"?client_type="+client_type)){
            handleGetWebDevicelistResponse(reply);
        }
        
        
        
        return;
        
        
        
        
        
        
        
    }
    else{
        //403 要展示没有权限。访问
        
        // 请求出错，检查状态代码
        int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if (statusCode == 403) {
            // 服务器拒绝了请求，执行相应的处理
            qDebug() << "Request failed with 403 Forbidden error.";
            emit net_error403Failure(reply->errorString());
        } else {
            // 其他错误状态代码，执行相应的处理
            emit net_errorFailure("\nstatusCode:" + QString::number(statusCode) + "\n" + reply->errorString());
        }
        
        
    }
    
    // 清理和释放网络响应资源
    reply->deleteLater();
    
   
}

void UserModule::handleLoginResponse(QNetworkReply* reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
        QJsonObject responseObject = jsonResponse.object();
        
        //根据不同请求路径在这边分发不同请求结果处理
        QUrl requestUrl = reply->request().url();

        int code = responseObject["code"].toInt();
        //登入成功
        if (code == 0) {
            
            QString token = parseTokenFromResponse(responseObject);
            
            //名字
            this->userName = responseObject["name"].toString();
            //保存
            saveAuthToken(token);
            //标记登入
            this->loggedIn = true;
            emit loginSuccess(token);
            
//            通过token 请求是不是会员
            this->getWebSuperMember();
            
            
        } else {
            QString errorMessage = parseErrorMessageFromResponse(responseObject);
            emit loginFailure(errorMessage);
        }
    } else {
        emit loginFailure(reply->errorString());
    }

    reply->deleteLater();
    
    //通知ui更新
    emit UpdataDataSig();
}

QString UserModule::parseTokenFromResponse(const QJsonObject& responseObject) {
    // 解析返回的 JSON 中的 token 字段
    return responseObject["token"].toString();
}

QString UserModule::parseErrorMessageFromResponse(const QJsonObject& responseObject) {
    // 解析返回的 JSON 中的 msg 字段
        return responseObject["msg"].toString();
}

void UserModule::getRegisterCode(const QString& username, const QString& email) {
    
//    this->userEmail = email;
//    this->userpassword = password;
    
    
    QJsonObject requestBody;
    requestBody["username"] = username;
    requestBody["email"] = email;
    
//    TODO: 带上本地标识。限制登入数量

    QJsonDocument jsonDocument(requestBody);
    QByteArray requestData = jsonDocument.toJson();

    QNetworkRequest request(REGISTER_GETCODE_API_URL);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    networkManager.post(request, requestData);
}

void UserModule::handlegetRegisterCodeResponse(QNetworkReply* reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
        QJsonObject responseObject = jsonResponse.object();
        
        //根据不同请求路径在这边分发不同请求结果处理
        QUrl requestUrl = reply->request().url();

        int code = responseObject["code"].toInt();
        //登入成功
        if (code == 0) {
            
            
            
            emit getRegisterCodeSuccess();
        } else {
            QString errorMessage = parseErrorMessageFromResponse(responseObject);
            emit getRegisterCodeFailure(errorMessage);
        }
    } else {
        emit getRegisterCodeFailure(reply->errorString());
    }

    reply->deleteLater();
}

void UserModule::emailRegister(const QString& username, const QString& email,const QString& mailcode,const QString& password,const QString& invite) {
    
//    this->userEmail = email;
//    this->userpassword = password;
    
    
    QJsonObject requestBody;
    requestBody["username"] = username;
    requestBody["email"] = email;
    requestBody["mailcode"] = mailcode;
    requestBody["password"] = password;
    requestBody["invite"] = invite;
    

    QJsonDocument jsonDocument(requestBody);
    QByteArray requestData = jsonDocument.toJson();

    QNetworkRequest request(REGISTER_API_URL);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    networkManager.post(request, requestData);
}
void UserModule::handlegetEmailRegisterResponse(QNetworkReply* reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
        QJsonObject responseObject = jsonResponse.object();
        
        //根据不同请求路径在这边分发不同请求结果处理
        QUrl requestUrl = reply->request().url();

        int code = responseObject["code"].toInt();
        //登入成功
        if (code == 0) {
            
            
            
            emit emailRegisterCodeSuccess();
        } else {
            QString errorMessage = parseErrorMessageFromResponse(responseObject);
            emit emailRegisterFailure(errorMessage);
        }
    } else {
        emit emailRegisterFailure(reply->errorString());
    }

    reply->deleteLater();
}

//准备不用
void UserModule::getWebIsPro(){
    

    const QString token = authToken;
    
   
    QNetworkRequest request(GET_ISPRO_API_URL);
    request.setRawHeader("Token", QString("%1").arg(token).toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    

    networkManager.get(request);
}
//会员准备不用
void UserModule::handleGetIsProResponse(QNetworkReply* reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
        QJsonObject responseObject = jsonResponse.object();
        
        //根据不同请求路径在这边分发不同请求结果处理
        QUrl requestUrl = reply->request().url();

        int code = responseObject["code"].toInt();
        //是会员
        if (code == 0) {
            


            emit getIsProSuccess();
        } else {
            //不是会员
            QString errorMessage = parseErrorMessageFromResponse(responseObject);
            emit getIsProFailure(errorMessage);
        }
    } else {
        emit getIsProFailure(reply->errorString());
    }

    reply->deleteLater();
}


//基本信息
void UserModule::getWebUserInfo() {
    
  
    
    const QString token = authToken;
    if(token == ""){
        emit  getUserInfoFailure(" ");
        return;
    }
    
   
    QNetworkRequest request(GET_U_INFO_API_URL);
    request.setRawHeader("Token", QString("%1").arg(token).toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    networkManager.get(request);
}

void UserModule::handleGetUserInfoResponse(QNetworkReply* reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
        QJsonObject responseObject = jsonResponse.object();
        
        //根据不同请求路径在这边分发不同请求结果处理
        QUrl requestUrl = reply->request().url();

        int code = responseObject["code"].toInt();
        //是会员
        if (code == 0) {
           

            //是那个基本信息
            QString username =  responseObject["data"].toObject()["username"].toString();
            QString email = responseObject["data"].toObject()["email"].toString();
            QString balance = responseObject["data"].toObject()["wallet"].toObject()["balance"].toString();

            emit getUserInfoSuccess(username,email,balance);
        } else {
            //不是会员
            QString errorMessage = parseErrorMessageFromResponse(responseObject);
            emit getUserInfoFailure(errorMessage);
        }
    } else {
        
        emit getUserInfoFailure(reply->errorString());
    }

    reply->deleteLater();
}

//所有开通的会员信息
void UserModule::getApppermission() {
    
  
    
    const QString token = authToken;
    if(token == ""){
//        emit  apppermissionFailure(" ");
        //提示去登录
        emit  getUserInfoFailure(" ");
        return;
    }
    
   
    QNetworkRequest request(AppPermission_API_URL);
    request.setRawHeader("Token", QString("%1").arg(token).toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    networkManager.get(request);
}
void UserModule::handlegetAppPermissionResponse(QNetworkReply* reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
        QJsonObject responseObject = jsonResponse.object();
        
        //根据不同请求路径在这边分发不同请求结果处理
        QUrl requestUrl = reply->request().url();

        int code = responseObject["code"].toInt();
        //是会员
        if (code == 0) {
            
            QJsonArray dataArray = responseObject["data"].toArray();

            emit apppermissionCodeSuccess(dataArray);
        } else {
            //不是会员
            QString errorMessage = parseErrorMessageFromResponse(responseObject);
            emit apppermissionFailure(errorMessage);
        }
    } else {
        
        emit apppermissionFailure(reply->errorString());
    }

    reply->deleteLater();
}



//退出登入
void UserModule::signOut(){
    //保存
    settings.setValue(Key_QSettings_authToken,"");
    
    //标记登入
    this->loggedIn = false;
    this->authToken = "";
    
    
    //会员信息
    m_isSuperMember = false;
    m_hasTool_AVS_Permission = false;
    
    
    emit signOutSuccess();

    //通知更新
    emit UpdataDataSig();
    
}



bool UserModule::isLoggedIn() const
{
    return this->loggedIn;
}





//用户权限管理
bool UserModule::isSuperMember()const{
    
    return m_isSuperMember;
    
}
bool UserModule::hasTool_AVS_Permission()const{
    return m_hasTool_AVS_Permission;
    
}
QDateTime  UserModule::get_time_outdata()const{
    return time_out_dateTime;
}




//向服务器请求权限 同步 //TODO: 获取会员同一入口
void  UserModule::getWebSuperMember(){
    
    
    //GET_UVAIGC_SuperMember_API_URL
    
    const QString token = authToken;
    if(token == ""){
        emit  getUserInfoFailure(" ");
        return;
    }
    
   
    QNetworkRequest request(GET_UVAIGC_SuperMember_API_URL);
    request.setRawHeader("Token", QString("%1").arg(token).toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // 发送同步请求
   QNetworkReply *reply =networkManager.get(request);
    
    // 设置超时时间
    int timeout = 5000; // 设置超时时间为5秒
    QTimer timer;
    timer.setSingleShot(true);
    timer.start(timeout);

    // 等待请求完成或超时
    while (!reply->isFinished()) {
        if (!timer.isActive()) {
            // 请求超时
            qDebug() << "Request timeout";
            reply->abort(); // 中断请求
            break;
        }
        QCoreApplication::processEvents();
    }

    if (reply->error() == QNetworkReply::NoError) {
        // 请求成功，处理响应数据
        //逻辑处理
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
        QJsonObject responseObject = jsonResponse.object();
        
        //根据不同请求路径在这边分发不同请求结果处理
        QUrl requestUrl = reply->request().url();

        int code = responseObject["code"].toInt();
        //是会员
        if (code == 0) {
            
            
            QJsonObject data_responseObject = responseObject["data"].toObject();
           
            
//            QString timestampString = .toString();
            double timestamp = data_responseObject["time_out"].toDouble();
            qDebug() << data_responseObject ;
            qint64 timestampMs = static_cast<qint64>(timestamp * 1000);  // 转换为毫秒

            QDateTime dateTime = QDateTime::fromMSecsSinceEpoch(timestampMs);
            
            
            //tonken 到期时间判断 自动刷新token 小于半天 就刷新
            double timestamp_exp = data_responseObject["exp"].toDouble();
            qint64 timestampS_exp = static_cast<qint64>(timestamp_exp);  // 转换为秒
            //小于12小时。60*60*12
            if (timestampS_exp != 0 && timestampS_exp < 43200) {
                //刷新token
                postTokenRefresh();
            }

            
            //到期时间
//            QDateTime dateTime = QDateTime::fromString(data_responseObject["time_out"].toString(), Qt::ISODateWithMs);
            
//            data_responseObject["time_out"].toString();
            this->time_out_dateTime = dateTime;
            qDebug() << "xiaoxi: " << this->time_out_dateTime;
            
            m_isSuperMember = true;
            
            
        } else {
            //不是会员
            QString errorMessage = parseErrorMessageFromResponse(responseObject);
            
        }
        
        
        
        
        
        
//        qDebug() << "Response data: " << responseData;
    } else {
        // 请求出错，打印错误信息
        qDebug() << "Request error: " << reply->errorString();

        emit net_errorFailure(reply->errorString());
    }
    
    //标记登入
    this->loggedIn = true;
    //通知更新
    emit UpdataDataSig();
    
 


    // 清理资源
    reply->deleteLater();
    
    
    
    
}
void  UserModule::getWebTool_AVS_Permission(){
    // 先尝试从缓存中获取Tool A的权限状态

   // 如果缓存中没有，则根据用户类型获取权限状态


       // 超级会员拥有Tool A的权限
      

       // 根据实际逻辑从服务器获取Tool A的权限状态
       // 这里为了演示，简单地假设具有Tool A权限
      


   // 更新缓存


//   return status;
    
}



//请求更新token
void UserModule::postTokenRefresh(){
    
    const QString token = authToken;
    if(token == ""){
        //去登录
        emit  getUserInfoFailure(" ");
        return;
    }
    
   

   
    
    
    

    QJsonObject requestBody;
    requestBody["device_uuid"] = device_uuid;
    requestBody["client_type"] = client_type;
    requestBody["device_name"] = device_name;
    
    QJsonDocument jsonDocument(requestBody);
    QByteArray requestData = jsonDocument.toJson();

    QNetworkRequest request(TokenRefresh_API_URL);
    request.setRawHeader("Token", QString("%1").arg(token).toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    networkManager.post(request, requestData);
    
}
void UserModule::handleTokenRefreshResponse(QNetworkReply* reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
        QJsonObject responseObject = jsonResponse.object();
        
        //根据不同请求路径在这边分发不同请求结果处理
        QUrl requestUrl = reply->request().url();

        int code = responseObject["code"].toInt();

        if (code == 0) {
            //获取token 更新自己的值
            QString token = parseTokenFromResponse(responseObject);
            //保存
            saveAuthToken(token);
            
            emit tokenRefreshSuccess();
            
            
        }else if (code == 2000){
            //提示设备已经到上限了,需要解除设备
            QString errorMessage = parseErrorMessageFromResponse(responseObject);
            emit tokenRefreshFailure(errorMessage);
            
            
        }
        else {
           
            QString errorMessage = parseErrorMessageFromResponse(responseObject);
            emit tokenRefreshFailure(errorMessage);
        }
    } else {
        
        emit tokenRefreshFailure(reply->errorString());
    }

    reply->deleteLater();
}


//设备管理信息
void UserModule::getWebDevicelist() {
    
  
    
    const QString token = authToken;
    if(token == ""){
//        emit  apppermissionFailure(" ");
        //提示去登录
        emit  getUserInfoFailure(" ");
        return;
    }
    
   
    QNetworkRequest request(GET_Devicelist_API_URL+"?client_type="+client_type);
    request.setRawHeader("Token", QString("%1").arg(token).toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    networkManager.get(request);
}
void UserModule::handleGetWebDevicelistResponse(QNetworkReply* reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
        QJsonObject responseObject = jsonResponse.object();
        
        //根据不同请求路径在这边分发不同请求结果处理
        QUrl requestUrl = reply->request().url();

        int code = responseObject["code"].toInt();
        //是会员
        if (code == 0) {
            
            QJsonArray dataArray = responseObject["data"].toArray();

            emit getWebDevicelistSuccess(dataArray);
        } else {
            //不是会员
            QString errorMessage = parseErrorMessageFromResponse(responseObject);
            emit getWebDevicelistFailure(errorMessage);
        }
    } else {
        
        emit getWebDevicelistFailure(reply->errorString());
    }

    reply->deleteLater();
}


//解除设备 同步
QNetworkReply* UserModule::delWebDevice(int id){
    
    //GET_UVAIGC_SuperMember_API_URL
    
    const QString token = authToken;
    if(token == ""){
        emit  getUserInfoFailure(" ");
        return nullptr;
    }
    
    QString url = GET_Devicelist_API_URL + QString::number(id) + "/";
    QNetworkRequest request(url);
    request.setRawHeader("Token", QString("%1").arg(token).toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    
   

    // 发送同步请求
   QNetworkReply *reply =networkManager.deleteResource(request);
    
    // 设置超时时间
    int timeout = 5000; // 设置超时时间为5秒
    QTimer timer;
    timer.setSingleShot(true);
    timer.start(timeout);

    // 等待请求完成或超时
    while (!reply->isFinished()) {
        if (!timer.isActive()) {
            // 请求超时
            qDebug() << "Request timeout";
            reply->abort(); // 中断请求
            break;
        }
        QCoreApplication::processEvents();
    }
    // 清理资源
//    reply->deleteLater();
    
    return reply;
    
}



QString UserModule::create_device_uuid(){
    //根据不同系统生成唯一id
    QString uuid = "suwhsdakjdsakjdskfoosdksk" + client_type;
    //win
    //mac
    // 获取设备唯一ID TODO: 不知道稳不稳定
    
    //系统平台判断
    QString productType = QSysInfo::productType();
    // 获取当前平台的内核类型
//    QString kernelType = QSysInfo::kernelType();
    
    QByteArray deviceId = QSysInfo::machineUniqueId();
    if(QString(deviceId.toHex())!=""){
        
        uuid = QString(deviceId.toHex()) +"_"+ productType;
        
    }
//    qDebug() << "Device ID: " << uuid << uuid.size();
    //用系统的方法??
    
    
    
    return uuid;
    
    
}

//保存token 同一管理
void UserModule::saveAuthToken(const QString& token){
    this->authToken = token;
    // 加密Token字符串并输出
    QString encryptedToken = encryptToken(token, device_uuid);
    settings.setValue(Key_QSettings_authToken,encryptedToken);
}
QString UserModule::readAuthToken(){
    QString out = settings.value(Key_QSettings_authToken).toString();
    // 解密加密后的Token字符串并输出
    QString decryptedToken = decryptToken(out, device_uuid);
    
    return decryptedToken;
    
}

//加密字符串
QString UserModule::encryptToken(const QString& token, const QString& key){
    // 将设备ID字符串转换为字节数组作为密钥
       QByteArray keyData = key.toUtf8();

       // 创建加密器
       QCryptographicHash hash(QCryptographicHash::Sha256);
       hash.addData(keyData);

       // 使用加密器生成密钥
       QByteArray derivedKey = hash.result();

       // 将Token字符串转换为字节数组
       QByteArray tokenData = token.toUtf8();

       // 使用XOR操作进行加密
       QByteArray encryptedData = tokenData;
       for (int i = 0; i < tokenData.size(); ++i) {
           encryptedData[i] = tokenData[i] ^ derivedKey[i % derivedKey.size()];
       }

       // 返回加密后的字符串
       return QString(encryptedData.toBase64());
    
}
QString UserModule::decryptToken(const QString& encryptedToken, const QString& key){
    
    // 将设备ID字符串转换为字节数组作为密钥
    QByteArray keyData = key.toUtf8();

    // 创建加密器
    QCryptographicHash hash(QCryptographicHash::Sha256);
    hash.addData(keyData);

    // 使用加密器生成密钥
    QByteArray derivedKey = hash.result();

    // 将加密后的Token字符串转换为字节数组
    QByteArray encryptedData = QByteArray::fromBase64(encryptedToken.toUtf8());

    // 使用XOR操作进行解密
    QByteArray decryptedData = encryptedData;
    for (int i = 0; i < encryptedData.size(); ++i) {
        decryptedData[i] = encryptedData[i] ^ derivedKey[i % derivedKey.size()];
    }

    // 返回解密后的字符串
    return QString(decryptedData);
    
}
