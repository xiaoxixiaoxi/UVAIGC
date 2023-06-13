#ifndef AVS_SETTINGQTWIDGETSCLASS_H
#define AVS_SETTINGQTWIDGETSCLASS_H

#include <QWidget>
#include "Settings_info.hpp"
#include <QSettings>
#include <QVector>
#include "Download/DownloadListOBJ.h"
#include "../AudioVocalSeparation/AVS_MDXNet_ONNX_Strategy.hpp"


namespace Ui {
class AVS_SettingQtWidgetsClass;
}

class AVS_SettingQtWidgetsClass : public QWidget
{
    Q_OBJECT

public:
    explicit AVS_SettingQtWidgetsClass(QWidget *parent = nullptr);
    ~AVS_SettingQtWidgetsClass();
    
    //处理对象
    AVS_MDXNet_ONNX_Strategy * avs_strategy = nullptr;
    void set_Strategy( AVS_MDXNet_ONNX_Strategy * _avs_strategy){
        avs_strategy = _avs_strategy;
    };
    
    
    
    
    void directory_model_save();

    void set_model_save_path();
//选择不同类型模型
    void set_model_selection_comboBox();
    void set_model_Vocals_selection_comboBox();
    
    //默认分离类型选择
    void set_default_Models_Type_list();
    
    
    
    

    int getFileList(const QString& dirPath, QStringList* outfileList);

    void loadModel();
    void verifyModel();

    void upDataDownloadListOBJ_list();
    
    
    //配置输出后缀名
    void init_OutType_suffix();


private:

    //œ¬‘ÿ¡–±Ì
//    QVector<DownloadListOBJ*> downloadListOBJ_list;

    // 模型保存地址
    QString model_save_path = "";
    QSettings settings = QSettings(ORGANIZATION_Name, APPLICATION_Name);
// 模型保存地址
    QString KEY_model_save_path = QSettings_KEY_AVS_model_save_path;


    QString model_Instrumental_selection_name;
    QString KEY_Instrumental_selection_name = QSettings_KEY_AVS_Instrumental_selection_name;
    
    QString model_Vocals_selection_name;
    QString KEY_Vocals_selection_name = QSettings_KEY_AVS_Vocals_selection_name;
    
    //下一版本再去更新
    QString model_Bass_selection_name;
    QString KEY_Bass_selection_name = QSettings_KEY_AVS_Bass_selection_name;
    
    QString model_Drums_selection_name;
    QString KEY_Drums_selection_name = QSettings_KEY_AVS_Drums_selection_name;
    
    QString model_Other_selection_name;
    QString KEY_Other_selection_name = QSettings_KEY_AVS_Other_selection_name;
    
    //输出类型
    QString default_Models_Type_list;
    QString KEY_AVS_OutType_list = QSettings_KEY_AVS_OutType_list;
    
    
    //输出后缀名
    QString default_out_suffix;
    
    //下载对象
    QVector<DownloadListOBJ*> downloadListOBJ_list;
    
    
    
    
    
//    extern const QString QSettings_KEY_AVS_Instrumental_selection_name;
//    extern const QString QSettings_KEY_AVS_Vocals_selection_name;
//    extern const QString QSettings_KEY_AVS_Bass_selection_name;
//    extern const QString QSettings_KEY_AVS_Drums_selection_name;
//    extern const QString QSettings_KEY_AVS_Other_selection_name;

//




private:
    Ui::AVS_SettingQtWidgetsClass *ui;
};

#endif // AVS_SETTINGQTWIDGETSCLASS_H
