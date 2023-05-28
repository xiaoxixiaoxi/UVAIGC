#include "avs_settingqtwidgetsclass.h"
#include "ui_avs_settingqtwidgetsclass.h"

#include <QFileDialog>
#include <QStandardPaths>
#include <QVariant>
#include <QMessageBox>
#include <QProcess>
#include <QDesktopServices>

//#ifdef Q_OS_MAC
////引入mac系统自带的内容
//
//#endif

AVS_SettingQtWidgetsClass::AVS_SettingQtWidgetsClass(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AVS_SettingQtWidgetsClass)
{
    ui->setupUi(this);
    
    
    this->model_save_path = settings.value(KEY_model_save_path).toString();
    //this->model_save_path = "";
    //为空时默认给一个路径地址
    if (this->model_save_path == "") {

        //获取默认的路径
        QString movies_path = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation) + "/" +APPLICATION_Name + "/AVSModel";


        QDir dir(movies_path);
        if (!dir.exists())
        {
            //不存在 就新建
            bool ok = dir.mkpath(movies_path);//创建多级目录

        }
        this->model_save_path = movies_path;
    }
    ui->model_save_path_label->setText(this->model_save_path);
    
    
    
    //set path
    connect(ui->model_save_path_pushButton, &QPushButton::clicked,
        this, &AVS_SettingQtWidgetsClass::directory_model_save
    );
    //show path
    connect(ui->show_model_path_pushButton, &QPushButton::clicked, this, [=]() {

        auto pathIn = this->model_save_path;
        //win
#ifdef Q_OS_WIN
        const QString explorer = "explorer";
        QStringList param;
        if (!QFileInfo(pathIn).isDir())
            param << QLatin1String("/select,");
        param << QDir::toNativeSeparators(pathIn);
        QProcess::startDetached(explorer, param);
#endif
        
#ifdef Q_OS_MAC
        
        qDebug()<<"mac";
        const QString explorer = "open";
        QStringList param;
        param << pathIn;
        QProcess::startDetached(explorer, param);
        
        #endif
        
        //mac

        });
    
    //models seting
    set_model_selection_comboBox();
    set_model_Vocals_selection_comboBox();

    connect(ui->model_up_pushButton, &QPushButton::clicked, this, [=]() {
        this->set_model_selection_comboBox();
        this->set_model_Vocals_selection_comboBox();
        });
    
    
    //currentTextChanged
    connect(ui->model_Instrumental_selection_comboBox, &QComboBox::currentTextChanged, this, [=](QString showname) {

        model_Instrumental_selection_name = ui->model_Instrumental_selection_comboBox->currentData().toString();

        settings.setValue(KEY_Instrumental_selection_name, model_Instrumental_selection_name);



        });
    
    connect(ui->model_Vocals_selection_comboBox, &QComboBox::currentTextChanged, this, [=](QString showname) {

        model_Vocals_selection_name = ui->model_Vocals_selection_comboBox->currentData().toString();

        settings.setValue(KEY_Vocals_selection_name, model_Vocals_selection_name);



        });

    //加载模型
    connect(ui->model_load_pushButton, &QPushButton::clicked, this, &AVS_SettingQtWidgetsClass::loadModel);

    //验证是否可用
    connect(ui->verify_pushButton, &QPushButton::clicked, this, &AVS_SettingQtWidgetsClass::verifyModel);
    
    //https://huggingface.co/yssck/UVAIGC/resolve/main/instrumental.onnx
    
    QString root_url = "https://huggingface.co/yssck/UVAIGC/resolve/main/";
    QVector<QVector<QString>> MYinfo;
    QVector<QString> tinyvect;
    tinyvect << "instrumental.onnx" << "(66.8MB)";
    MYinfo << tinyvect;
    QVector<QString> tinyenvect;
    tinyenvect << "vocals.onnx" << "(29.7MB)";
    MYinfo << tinyenvect;

  

    QVector<DownloadListOBJ*> my_list;
    for (auto i : MYinfo) {
        DownloadListOBJ* testobj = new DownloadListOBJ(this);
        QString name = i[0] ;
        QString nameinfo = i[1];

        QString url = root_url + name;
        testobj->init_obj(name, nameinfo, url, this->model_save_path);
        my_list.append(testobj);
    }

    downloadListOBJ_list = my_list;

    //auto list = taskOBJ->getTaskList();
    //新建数据
    ui->DownloadlistWidget->addData(my_list);
    ui->DownloadlistWidget->reloadData();
    
    
    QStringList default_OutType_list = settings.value(KEY_AVS_OutType_list).toStringList();
    if(default_OutType_list.isEmpty() ){

    }else {
        ui->Instrumental_checkBox->setChecked(false);
        ui->Vocals_checkBox->setChecked(false);
        for (QString i : default_OutType_list ) {

            if (i=="Instrumental"){
                ui->Instrumental_checkBox->setChecked(true);
            }
            if (i=="Vocals"){
                ui->Vocals_checkBox->setChecked(true);
            }


        }



    }

    //绑定默认选择输出类型
    connect(ui->Vocals_checkBox, &QPushButton::clicked, this, &AVS_SettingQtWidgetsClass::set_default_Models_Type_list);
    connect(ui->Instrumental_checkBox, &QPushButton::clicked, this, &AVS_SettingQtWidgetsClass::set_default_Models_Type_list);

    
    //去网页下载
    connect(ui->to_web_d_pushButton, &QPushButton::clicked, this, [=]() {
        
        //跳转网页下载
        QDesktopServices::openUrl(QString::fromStdString(TO_URL_AVS_Download_Help));

        });
    
    
    
    
    
    
}

AVS_SettingQtWidgetsClass::~AVS_SettingQtWidgetsClass()
{
    for(auto i :  downloadListOBJ_list){
        delete i;
    }
    
    
    delete ui;
    
}




//保存模型目录选择
void AVS_SettingQtWidgetsClass::directory_model_save() {
    //选中文件夹
    QString DirectoryName;
    QFileDialog fileDialog(this);
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    //可选择多个文件 //不能同时选择目录和文件
    fileDialog.setFileMode(QFileDialog::Directory);
    fileDialog.setWindowTitle(tr("选择保存目录"));
    fileDialog.setDirectory(this->model_save_path);
    if (fileDialog.exec() == QDialog::Accepted) {
        //QStringList strPathList = fileDialog.selectedFiles();
        //this->out_path = (fileDialog.selectedUrls().constFirst()).toLocalFile();
        //this->saveOutPath();
        DirectoryName = (fileDialog.selectedUrls().constFirst()).toLocalFile();
        this->model_save_path = DirectoryName;
        set_model_save_path();

        //更新下载列表数据
//        upDataDownloadListOBJ_list();

         //刷新模型选择
        this->set_model_selection_comboBox();


    }
    else
    {
        return;
    }

}

//显示路径保存
void AVS_SettingQtWidgetsClass::set_model_save_path() {
    //保存下来 显示
    ui->model_save_path_label->setText(this->model_save_path);
    settings.setValue(this->KEY_model_save_path, this->model_save_path);
}

//加载目录下所有文件 背景音模型选项
void AVS_SettingQtWidgetsClass::set_model_selection_comboBox() {
    //获取目录下所有文件名称
    //提供选择

    //获取选中

    QString selectionName = settings.value(KEY_Instrumental_selection_name).toString();
    this->model_Instrumental_selection_name = selectionName;

    QStringList outfileList;
    getFileList(this->model_save_path, &outfileList);
    ui->model_Instrumental_selection_comboBox->clear();
    int currentIndex = 0;
    int index = 0;
    for (auto i : outfileList) {
        if (i == selectionName) {
            currentIndex = index;
        }
        //
        //QString showname = QFileInfo f_newname(new_name);
        QFileInfo showname(i);
        ui->model_Instrumental_selection_comboBox->addItem(showname.fileName(), QVariant(i));
        index += 1;
    }

    
    ui->model_Instrumental_selection_comboBox->setCurrentIndex(currentIndex);



}

//人声模型选择 void set_model_Vocals_selection_comboBox();
void AVS_SettingQtWidgetsClass::set_model_Vocals_selection_comboBox() {
    //获取目录下所有文件名称
    //提供选择

    //获取选中

    QString selectionName = settings.value(KEY_Vocals_selection_name).toString();
    this->model_Vocals_selection_name = selectionName;

    QStringList outfileList;
    getFileList(this->model_save_path, &outfileList);
    ui->model_Vocals_selection_comboBox->clear();
    int currentIndex = 0;
    int index = 0;
    for (auto i : outfileList) {
        if (i == selectionName) {
            currentIndex = index;
        }
        //
        //QString showname = QFileInfo f_newname(new_name);
        QFileInfo showname(i);
        ui->model_Vocals_selection_comboBox->addItem(showname.fileName(), QVariant(i));
        index += 1;
    }

    
    ui->model_Vocals_selection_comboBox->setCurrentIndex(currentIndex);



}

//遍历当前路径下文件
int  AVS_SettingQtWidgetsClass::getFileList(const QString& dirPath, QStringList* outfileList)
{
    //int FindFile(const QString & _filePath)

    QDir dir(dirPath);   //QDir的路径一定要是全路径，相对路径会有错误

    if (!dir.exists())
        return -1;

    //取到所有的文件和文件名，去掉.和..文件夹
    dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    dir.setSorting(QDir::DirsFirst);
    //将其转化为一个list
    QFileInfoList list = dir.entryInfoList();
    if (list.size() < 1) {
        return -1;
    }

    //return -1;

    int i = 0;
    //采用递归算法
    do {
        QFileInfo fileInfo = list.at(i);
        bool bisDir = fileInfo.isDir();
        if (bisDir) {

            //getFileList(fileInfo.filePath(), outfileList);
        }
        else {
            outfileList->append(fileInfo.filePath());
            qDebug() << fileInfo.filePath() << ":" << fileInfo.fileName();
        }


        ++i;
    } while (i < list.size());

    return 0;
}


void AVS_SettingQtWidgetsClass::set_default_Models_Type_list(){
    
    QStringList default_out_list;
    
    if (ui->Instrumental_checkBox->isChecked()){
        default_out_list << "Instrumental";
    }
    if (ui->Vocals_checkBox->isChecked()){
        
        default_out_list << "Vocals";
    }
    
    settings.setValue(this->KEY_AVS_OutType_list, default_out_list);
    
    
    
    
}



//加载模型
void  AVS_SettingQtWidgetsClass::loadModel() {
    //获取全局对象
    //清空
    //delete LIBWHISPER_obj;
    //LIBWHISPER_obj = nullptr;
    
    //检查是否可用 数据是否支持 开始识别 进度回调 完成调用
    if (avs_strategy == nullptr) {
        QMessageBox::warning(this,
            tr("加载失败"),
            tr("没有关联对象.\n无需加载加载..."),
            QMessageBox::Yes, QMessageBox::Yes);
        
        return;
    }
    
    //对象进行加载,最后提示是否成功
    
    bool is_ok_Instrumental = avs_strategy->loadOxxnModel(avs_strategy->session_Instrumental, this->model_Instrumental_selection_name.toStdString());
    bool is_ok_Vocals = avs_strategy->loadOxxnModel(avs_strategy->session_Vocals, this->model_Vocals_selection_name.toStdString());
    if (is_ok_Instrumental && is_ok_Vocals) {
        // ok

        QMessageBox msgBox(this);
        msgBox.setText(QString(tr("加载成功")) + "\n" + this->model_Instrumental_selection_name + "\n" + this->model_Vocals_selection_name);
        msgBox.exec();
    }
    else
    {
        QString text = tr("加载失败");
        if (is_ok_Instrumental == false) {
            
            text += this->model_Instrumental_selection_name;
            
        }
        
        if (is_ok_Vocals == false) {
            text += this->model_Vocals_selection_name;
        }
        
        
        QMessageBox::warning(this,
            tr("加载失败"),
                             text,
            QMessageBox::Yes, QMessageBox::Yes);

    }


}


//验证模型
void AVS_SettingQtWidgetsClass::verifyModel(){
    
    
    QStringList all_path;
    all_path << this->model_Instrumental_selection_name;
    all_path << this->model_Vocals_selection_name;
   
    
    
    
    QString ok_path = "";
    
    for(QString& i : all_path){
        
        try {
            Ort::Env test_env;

             std::wstring weightFile_w(i.toStdString().begin(), i.toStdString().end());
            //加载ONNX模型


//        平台判断
#ifdef Q_OS_WIN
            Ort::Session  test_session_Instrumental(test_env,weightFile_w.c_str(), Ort::SessionOptions{ nullptr });
#endif

#ifdef Q_OS_MAC
           Ort::Session  test_session_Instrumental(test_env,i.toStdString().c_str(), Ort::SessionOptions{ nullptr });
#endif
            
            ok_path += i+"\n";
 

         } catch (const Ort::Exception& exception) {
             
             
             QString text = "";
             text += tr("不可加载模型") + "\n";
             text += QString::fromStdString(exception.what()) + "\n";
             text += i + "\n";
             
             
             QMessageBox::warning(this,
                 tr("不可加载模型"),
                 text,
                 QMessageBox::Yes, QMessageBox::Yes);
             
             
             std::cout << "错误" << exception.what() << std::endl;
             
         }
        
        
        
        
        
    }
    
    if (ok_path.size() > 0) {
        
        //可加载
        QMessageBox msgBox(this);
        msgBox.setText(QString(tr("可加载模型")) + "\n" + ok_path);
        msgBox.exec();
    }
    
    

    


}
//加模型 多个 有问题反馈出来


//验证模型
