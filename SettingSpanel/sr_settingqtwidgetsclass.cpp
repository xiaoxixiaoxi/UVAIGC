#include "sr_settingqtwidgetsclass.h"
#include "ui_sr_settingqtwidgetsclass.h"

//SR_settingqtwidgetsclass::SR_settingqtwidgetsclass(QWidget *parent) :
//    QWidget(parent),
//    ui(new Ui::SR_settingqtwidgetsclass)
//{
//    ui->setupUi(this);
//}

//SR_settingqtwidgetsclass::~SR_settingqtwidgetsclass()
//{
//    delete ui;
//}


#include <QFileDialog>
#include <QStandardPaths>
#include <QVariant>
#include <QMessageBox>
#include <QProcess>
#include <QDesktopServices>
//#include <QZipReader>

//解压包
#include "../Basis/zip_file.hpp"
#include "../SR/SR_type.h"

//#ifdef Q_OS_MAC
////引入mac系统自带的内容
//
//#endif

SR_settingqtwidgetsclass::SR_settingqtwidgetsclass(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SR_settingqtwidgetsclass)
{
    ui->setupUi(this);


    this->model_save_path = settings.value(KEY_model_save_path).toString();
    //this->model_save_path = "";
    //为空时默认给一个路径地址
    if (this->model_save_path == "") {

        //获取默认的路径
        QString movies_path = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation) + "/" +APPLICATION_Name + "/SRModel/models";


        QDir dir(movies_path);
        if (!dir.exists())
        {
            //不存在 就新建
            bool ok = dir.mkpath(movies_path);//创建多级目录

        }
        this->model_save_path = movies_path;
        //保存一下。方便其他读出
        set_model_save_path();
        
    }
    ui->model_save_path_label->setText(this->model_save_path);



    //set path
    connect(ui->model_save_path_pushButton, &QPushButton::clicked,
        this, &SR_settingqtwidgetsclass::directory_model_save
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

//    //models seting
    set_model_selection_comboBox();
////    set_model_Vocals_selection_comboBox();
//
    //刷新按钮绑定
    connect(ui->model_up_pushButton, &QPushButton::clicked, this, [=]() {
        this->set_model_selection_comboBox();
//        this->set_model_Vocals_selection_comboBox();
        });


    //currentTextChanged
    connect(ui->model_Instrumental_selection_comboBox, &QComboBox::currentTextChanged, this, [=](QString showname) {

        default_SR01_selection_name = ui->model_Instrumental_selection_comboBox->currentData().toString();

        settings.setValue(KEY_SR01_selection_name, default_SR01_selection_name);
        //展示可放大倍数
        set_SR01_models_scaled();
        




        });



    //加载模型
//    connect(ui->model_load_pushButton, &QPushButton::clicked, this, &SR_settingqtwidgetsclass::loadModel);

    //验证是否可用
    connect(ui->verify_pushButton, &QPushButton::clicked, this, &SR_settingqtwidgetsclass::verifyModel);

    //https://huggingface.co/yssck/UVAIGC/resolve/main/instrumental.onnx

    QString root_url = "https://huggingface.co/yssck/UVAIGC/resolve/main/";
    QVector<QVector<QString>> MYinfo;
    QVector<QString> tinyvect;
    tinyvect << "realesrmodels.zip" << "(42.8MB)";
    MYinfo << tinyvect;
//    QVector<QString> tinyenvect;
//    tinyenvect << "vocals.onnx" << "(29.7MB)";
//    MYinfo << tinyenvect;



    QVector<DownloadListOBJ*> my_list;
    for (auto i : MYinfo) {
        DownloadListOBJ* testobj = new DownloadListOBJ(this);
        QString name = i[0] ;
        QString nameinfo = i[1];

        QString url = root_url + name;
        testobj->init_obj(name, nameinfo, url, this->model_save_path);
        my_list.append(testobj);
        
        //绑定完成信号
        connect(testobj->dT,&DownloadTool::sigDownloadFinished,this,[=](){
            //解压文件 //刷新一下
            QString path = model_save_path + "/realesrmodels.zip";
            //判断文件是否存在
            //解压
            try{
                //可能抛出异常的语句
                //捕捉错误。解压失败时候调用
//                QString testzip = "/Users/linximing/Movies/UVAIGC/SRModel/test/realesrmodels.zip";
                //测试 解开zip
//                miniz_cpp::zip_file file(path.toStdString());
//                file.extractall(model_save_path.toStdString());//解压 y
                
                unzip_process(path, model_save_path);
                
                
                                //刷新
                this->set_model_selection_comboBox();
            }catch(std::exception &e){
                //处理异常的语句
                qDebug()<< QString::fromStdString(e.what()) ;
            }
            
            
            
            
            
            
            
        });
        
    }

    downloadListOBJ_list = my_list;

    //auto list = taskOBJ->getTaskList();
    //新建数据
    ui->DownloadlistWidget->addData(my_list);
    ui->DownloadlistWidget->reloadData();


//    QStringList default_OutType_list = settings.value(KEY_AVS_OutType_list).toStringList();



    //去网页下载
    connect(ui->to_web_d_pushButton, &QPushButton::clicked, this, [=]() {

        //跳转网页下载
        QDesktopServices::openUrl(QString::fromStdString(TO_URL_SR_Download_Help01));

        });
    
    init_Video_image_OutType();
    
    
    
    
    connect(ui->model_path_help_pushButton, &QPushButton::clicked, this, [=]() {
        
        QString text = ui->model_path_help_pushButton->toolTip();
        QMessageBox::warning(this,
            tr("提醒"),
            text,
            QMessageBox::Yes, QMessageBox::Yes);
        



        });
    
    
    //测试解压
    
//    miniz_cpp::zip_file file("/Users/linximing/Movies/UVAIGC/export/models/realesrmodels2.zip");
//    file.extractall(model_save_path.toStdString());//解压
//    file.printdir();
    

//    QString path = model_save_path + "/realesrmodels.zip";
//    unzip_process(path, model_save_path);
//    
    
    

    
//    qDebug()<<"222";






}

SR_settingqtwidgetsclass::~SR_settingqtwidgetsclass()
{
    for(auto i :  downloadListOBJ_list){
        delete i;
    }


    delete ui;

}




//保存模型目录选择
void SR_settingqtwidgetsclass::directory_model_save() {
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
        //DirectoryName文件夹 QString路径
        //判断文件夹的名称是不是models,如果是就继续,如果不是就新建一个models 文件夹
        
        QString ok_path = DirectoryName;
        QString directoryName_ok = "models";
        QString path = DirectoryName;

        QDir dir(path);
        QString lastDirectoryName = dir.dirName();  // 获取路径中的最后一个文件夹名称
        if (lastDirectoryName != directoryName_ok) {
            // 文件夹不存在，创建新的文件夹
            if (dir.mkdir(directoryName_ok)) {
                // 文件夹创建成功
//                ok_path = ok_path + "/" + directoryName_ok;

            } else {
                // 文件夹创建失败
                // TODO: 处理文件夹创建失败的情况
            }
            
            ok_path = ok_path + "/" + directoryName_ok;
        }else{
            
            //ok_path = ok_path + "/" + directoryName_ok;
            
        }
        
       
        
        
        this->model_save_path = ok_path;
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
void SR_settingqtwidgetsclass::set_model_save_path() {
    //保存下来 显示
    ui->model_save_path_label->setText(this->model_save_path);
    settings.setValue(this->KEY_model_save_path, this->model_save_path);
}

//加载目录下所有文件 背景音模型选项
void SR_settingqtwidgetsclass::set_model_selection_comboBox() {
    //获取目录下所有文件名称
    //提供选择

    //获取选中

    QString selectionName = settings.value(KEY_SR01_selection_name).toString();
    this->default_SR01_selection_name = selectionName;

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
    
   //展示可放大倍数
    set_SR01_models_scaled();

    //保存
    default_SR01_selection_name = ui->model_Instrumental_selection_comboBox->currentData().toString();

    settings.setValue(KEY_SR01_selection_name, default_SR01_selection_name);
    //展示可放大倍数
//    set_SR01_models_scaled();
//    std::vector<int> Scaled =  [CurrentName];



}



//遍历当前路径下文件
int  SR_settingqtwidgetsclass::getFileList(const QString& dirPath, QStringList* outfileList)
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
    
    
    //推理文件过滤处理,只满足2个文件.bin 和 .param 才能算一组
    QStringList LS_outfileList;
    for (QString& i : *outfileList) {
        //获取所有后缀名为bin
        QFileInfo fileInfo(i);
        QString fileName = fileInfo.baseName(); // 获取文件名（无后缀名）
        QString fileExtension = fileInfo.suffix(); // 获取文件后缀名
        if(fileExtension == "bin"){
            for (auto& j : *outfileList){
                QFileInfo fileInfo2(j);
                QString fileName2 = fileInfo2.baseName(); // 获取文件名（无后缀名）
                QString fileExtension2 = fileInfo2.suffix(); // 获取文件后缀名
                
                if (fileExtension2 == "param") {
                    if (fileName == fileName2) {
                        LS_outfileList.append(fileName);
                        break;
                    }
                }
                
                
                
            }
            
            
            
        }

        
        
        
        
        
    }
    
    
    outfileList->clear();
    
    for (QString& i : LS_outfileList){
        outfileList->append(i);
    }
    
    
    

    return 0;
}









//验证模型
void SR_settingqtwidgetsclass::verifyModel(){


    QStringList all_path;
    all_path << default_SR01_selection_name;
//    all_path << this->model_Vocals_selection_name;




    QString ok_path = "";
    //准备
    SR_ESRGAN_ncnn_Strategy obj;
    
    for(QString& i : all_path){
        
      
        
        
        if (obj.verifyModel(model_save_path, i)){
            
            QMessageBox msgBox(this);
            msgBox.setText(QString(tr("可加载模型")) + "\n" + i);
            msgBox.exec();
            
        }else{
            QString text = "";
            text += tr("不可加载模型") + "\n";
            text += i + "\n";
            QMessageBox::warning(this,
                tr("不可加载模型"),
                text,
                QMessageBox::Yes, QMessageBox::Yes);
            
            
            
        }
        







    }






}


//展示可放大倍数
void SR_settingqtwidgetsclass::set_SR01_models_scaled(){
    
    
    //关联可以支持的放大倍数
    QString Scaled_info = tr("未知");
    std::string currentName = (ui->model_Instrumental_selection_comboBox->currentText()).toStdString();
    // 访问全局常量字典
    for (const auto& pair : SR_ESRGAN_NCNN_Models_Scaled_DICTIONARY)
    {
//        std::cout << "Key: " << pair.first << ", Value: ";
        
        if (pair.first == currentName) {
            //有
            Scaled_info = "";
            Scaled_info += "[";
            
            for (int value : pair.second)
            {
                Scaled_info += " x"+QString::number(value);
                //保存起来
               
                
            }
            Scaled_info += "]";
            
            break;
            
        }
        
        
    }
    
    
    
    
    ui->scaled_label->setText(Scaled_info);
    
}

void SR_settingqtwidgetsclass::init_Video_image_OutType(){
    //恢复
    default_Video_OutType = settings.value(KEY_SR_Video_OutType).toString();
    if (default_Video_OutType == "mov") {
        ui->video_radioButton_mov->setChecked(true);
    }else if (default_Video_OutType == "mp4"){
        ui->video_radioButton_mp4->setChecked(true);
    }else{
        ui->video_radioButton_default->setChecked(true);
    }

    //输出类型关联
    connect(ui->out_video_buttonGroup, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked),
                         [&](QAbstractButton *button) {
        QString video_OutType = "default";
        //判断逻辑
        if (button == ui->video_radioButton_default) {
            video_OutType = "default";
        }
        else if(button == ui->video_radioButton_mov) {
            video_OutType = "mov";
        }
        else if(button == ui->video_radioButton_mp4) {
            video_OutType = "mp4";
        }
        //储存起来
        default_Video_OutType = video_OutType;
        settings.setValue(KEY_SR_Video_OutType, default_Video_OutType);

                         });
    
    
    
    
    
    
    
    
    
    
    default_Image_OutType = settings.value(KEY_SR_Image_OutType).toString();
    if (default_Image_OutType == "jpg") {
       
        ui->image_radioButton_jpg->setChecked(true);
    }else{
        ui->image_radioButton_png->setChecked(true);
    }

    //输出类型关联
    connect(ui->out_image_buttonGroup, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked),
                         [&](QAbstractButton *button) {
        QString image_OutType = "png";
        //判断逻辑
        if (button == ui->image_radioButton_jpg) {
            image_OutType = "jpg";
        }
        else{
            image_OutType = "png";
        }
        //储存起来
        
        
        default_Image_OutType = image_OutType;
        settings.setValue(KEY_SR_Image_OutType,   default_Image_OutType);

                         });
    
    
    
    
    
    
    
    
    
}



//解压文件 用系统自带工具
void SR_settingqtwidgetsclass::unzip_process(QString zipFilePath,QString destinationDir){
    
    QProcess process;
//    QString zipFilePath = "/Users/linximing/Movies/UVAIGC/export/models/realesrmodels.zip";
//    QString destinationDir = model_save_path;
   
    QString productType = QSysInfo::productType();
    // 获取当前平台的内核类型
    QString kernelType = QSysInfo::kernelType();
    
    if (productType.contains("windows", Qt::CaseInsensitive)) {
//            qDebug() << "Windows";
            // 在 Windows 上执行特定的操作
        process.start("powershell", QStringList() << "-Command" << "Expand-Archive" << "-Path" << zipFilePath << "-DestinationPath" << destinationDir);
        process.waitForFinished(-1);

        } else if (productType.contains("mac", Qt::CaseInsensitive) && kernelType.contains("darwin", Qt::CaseInsensitive)) {
//            qDebug() << "macOS";
            
            process.start("unzip", QStringList() << "-d" << destinationDir << zipFilePath);
            process.waitForFinished(-1);
            // 在 macOS 上执行特定的操作
        } else {
            qDebug() << "Unknown";
            // 在未知操作系统上执行特定的操作
        }

    
    
    
    
}


