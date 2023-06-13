#ifndef SR_SETTINGQTWIDGETSCLASS_H
#define SR_SETTINGQTWIDGETSCLASS_H

#include <QWidget>
#include "Settings_info.hpp"
#include <QSettings>
#include <QVector>
#include "Download/DownloadListOBJ.h"
//#include "../AudioVocalSeparation/AVS_MDXNet_ONNX_Strategy.hpp"
#include "../SR/SR_TaskProcessor_QT.hpp"

namespace Ui {
class SR_settingqtwidgetsclass;
}

class SR_settingqtwidgetsclass : public QWidget
{
    Q_OBJECT

public:
    explicit SR_settingqtwidgetsclass(QWidget *parent = nullptr);
    ~SR_settingqtwidgetsclass();


    //处理对象
    SR_TaskProcessor_QT * strategy = nullptr;
    void set_Strategy(  SR_TaskProcessor_QT * _strategy){
        strategy = _strategy;
    };




    void directory_model_save();

    void set_model_save_path();
    //选择不同类型模型
    void set_model_selection_comboBox();
//    void set_model_Vocals_selection_comboBox();

    //默认分离类型选择
    void set_default_Models_Type_list();
    //关联放大倍数
    void set_SR01_models_scaled();
    //设置输出类型配置
    void init_Video_image_OutType();
    //解压文件 用系统自带工具
    void unzip_process(QString zipFilePath,QString destinationDir);





    static int getFileList(const QString& dirPath, QStringList* outfileList);

    void loadModel();
    void verifyModel();

    void upDataDownloadListOBJ_list();


private:

    //œ¬‘ÿ¡–±Ì
    //    QVector<DownloadListOBJ*> downloadListOBJ_list;

    // 模型保存地址
    QString model_save_path = "";
    QSettings settings = QSettings(ORGANIZATION_Name, APPLICATION_Name);
    // 模型保存地址
    QString KEY_model_save_path = QSettings_KEY_SR_model_save_path;
    
    //超分辨率引擎1 默认选择推理文件
    QString default_SR01_selection_name;
    QString KEY_SR01_selection_name = QSettings_KEY_SR01_selection_model_name;




    //输出类型
    QString default_Image_OutType;
    QString KEY_SR_Image_OutType = QSettings_KEY_SR_Image_OutType;
    
    
    QString default_Video_OutType;
    QString KEY_SR_Video_OutType = QSettings_KEY_SR_Video_OutType;
    


    //下载对象
    QVector<DownloadListOBJ*> downloadListOBJ_list;


private:
    Ui::SR_settingqtwidgetsclass *ui;
};

#endif // SR_SETTINGQTWIDGETSCLASS_H
