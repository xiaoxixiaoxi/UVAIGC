#ifndef SR_TASKLISTUI_MAIN_H
#define SR_TASKLISTUI_MAIN_H

#include <QWidget>
#include "../SR/sr_esrgan_ncnn_strategy.h"
#include "../SettingSpanel/Settings_info.hpp"
#include <QDragEnterEvent>
#include <QMimeData>
#include <QStandardPaths>
#include <QSettings>
#include <QThread>

#include <QFileInfo>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QTimer>
#include "../SR/SR_TaskProcessor_QT.hpp"
#include "../SR/sr_esrgan_ncnn_strategy.h"

namespace Ui {
class SR_TaskListUI_main;
}

class SR_TaskListUI_main : public QWidget
{

    Q_OBJECT
    
signals:
    void runSig();
    void stopSig();
    
    //设置面板
    void toSettingSpanelQtWidgetsClass_SR_WIN_Sig(SR_TaskProcessor_QT * sr_strategy);
    //to升级会员控制面板
    void toProwin_Sig();

    
    
public:
    explicit SR_TaskListUI_main(QWidget *parent = nullptr);
    ~SR_TaskListUI_main();


    
    // 任务列表对象
    std::vector<SR_Task> avs_Tasks;
    
    
    //带有信号的处理对象。入口
    SR_TaskProcessor_QT* processorQT;
//    SR_ESRGAN_ncnn_Strategy esrgan_nccnn_srtategy;
    
    //处理对象 需要配置
//    TaskProcessor processor;
    //ONNX 音频模块处理策略
//    AVS_MDXNet_ONNX_Strategy avs_mdxnet_onnx_Strategy;
    //加载策略
//    AVS_QTLoaderFile loadfilr_Strategy =  AVS_QTLoaderFile();
    
    
private:
    Ui::SR_TaskListUI_main *ui;
    


public:
    //√ª”–¡–±Ì ˝œ‘ æÕœ∑≈Õº±Í∞¥≈•
    void update_Drag_ui();

    void openFile();
    // ÃÌº”µΩ∂‘œÛ
    void add_to_obj(QStringList inputlist);
    //øÿ÷∆ƒ‹≤ªƒ‹µº»Î
    void if_load(QStringList inputlist, QStringList* ok_url, QStringList* no_url);

    //ªÒ»°À˘”–Œƒº˛ ÷∏∂®ƒø¬ºœ¬
    int getFileList(const QString& dirPath, QStringList* outfileList);

    //…æ≥˝—°÷–µƒ
    void del_obj();

    //  ‰≥ˆ¬∑æ∂
    void initOutPath();
    void setOutPath();
    void saveOutPath();

    void run();
    void stop();
    
    
    //创建对象统一处理 进行对象配置
    void init_avsTask_obj(SR_Task& obj);
    
    //配置处理对象 引擎
    bool init_AVS_Strategy();




protected:
    void dragEnterEvent(QDragEnterEvent* event);    //Õœ∂ØΩ¯»Î ¬º˛
    void dropEvent(QDropEvent* event);              //∑≈œ¬ ¬º˛

    void saveProject();

    void readProject();

    bool is_ProQMessageBox();

    bool run_is_ProQMessageBox();
    
    //提醒去登录
    bool run_is_ProQMessageBox_LoggedIn();

    void exportAllSRT();
    void exportAllTXT();

    bool isFile_exporQMessageBox(QStringList is_isFile);

    void exportOKQMessageBox(QStringList is_completed, QStringList no_completed);

//    void exportAllobj(int type);

    //øÏΩ›º¸
    virtual void keyPressEvent(QKeyEvent* event);


private:
    
    // ‰≥ˆ¬∑æ∂
    QString out_path = "";
    //保存的key值
    QString Key_QSettings_OutPath = "sr_info/sr_outpath";
    QSettings settings = QSettings(ORGANIZATION_Name, APPLICATION_Name);


    //‘À––
    //∂‡œﬂ≥Ã
    QThread* t1 = new QThread;
    bool is_run = false;
    
    //是不是取消
    bool m_is_cancelled = false;

    //π§≥Ã±£¥Ê
    QString project_out_fileName;
    
    //处理工具模块
//    AVS_Task_QTtool task_QTtool;
    
    
    //定义一个定时器。刷新进度
    QTimer timer = QTimer(nullptr);;

    
    
    
    
    
    
    
    
    
    
};

#endif // SR_TASKLISTUI_MAIN_H
