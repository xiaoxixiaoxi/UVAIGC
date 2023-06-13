#include "TaskListWidgetsClass.h"
#include "ui_TaskListWidgetsClass.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>
#include "../SettingSpanel/SettingSpanel_main.h"

TaskListWidgetsClass::TaskListWidgetsClass(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TaskListWidgetsClass)
{
    ui->setupUi(this);
    
    
    
    
    //ui->pushButton->setText("按钮");
    //输出路径
    initOutPath();

//    avs_Tasks
    //auto list = taskOBJ->getTaskList();
    //新建数据
    ui->listWidget->addData(&avs_Tasks);
    ui->listWidget->reloadData();
//编辑按钮
    connect(ui->listWidget, &TaskQListWidget::editpushButt,
        this, [=](int tag) {

//            auto obj = avs_Tasks[tag];
//
//            VideoQtWidgets* player = new VideoQtWidgets();
//            player->set_myocr(obj);
//            player->show();
//
//            //绑定
//            connect(player, &VideoQtWidgets::closeSig, this, [=]() {
//                //delete player;
//                ui->listWidget->updataUI();
//
//                });

      
            


    


            qDebug() << tag;
            //emit editpushButt(tag);
        });


    connect(ui->addpushButton, &QPushButton::clicked,
        this, &TaskListWidgetsClass::openFile
    );

    connect(ui->delpushButton, &QPushButton::clicked,
        this, &TaskListWidgetsClass::del_obj
    );
    ////输出路径显示按钮
    //connect(ui->commandLinkButton, &QPushButton::clicked,
    //    this, &TaskListWidgetsClass::setOutPath
    //);
    //// 选择按钮
    connect(ui->out_path_pushButton, &QPushButton::clicked,
        this, &TaskListWidgetsClass::setOutPath
    );
    //显示输出按钮
    connect(ui->outpath_label_pushButton, &QPushButton::clicked,
        this, [=]() {
        QStringList param;
        param << this->out_path;
        
        task_QTtool.showFileFromFinder(param);
        



        }
    );

//    connect(ui->toallsrt_pushButton, &QPushButton::clicked,
//        this, &TaskListWidgetsClass::exportAllSRT
//    );
//    connect(ui->toalltxt_pushButton, &QPushButton::clicked,
//        this, &TaskListWidgetsClass::exportAllTXT
//    );
    
    //设置菜单按钮
    connect(ui->avs_setuppushButton, &QPushButton::clicked,
            this, [=](){
//        SHOW_SettingSpanelQtWidgetsClass_WIN(1);
//        SHOW_SettingSpanelQtWidgetsClass_AVS_WIN(1, &avs_mdxnet_onnx_Strategy);
        //改成信号
        emit toSettingSpanelQtWidgetsClass_AVS_WIN_Sig(&avs_mdxnet_onnx_Strategy);
        
    }
    );
    




    connect(ui->runpushButton, &QPushButton::clicked,
        this, &TaskListWidgetsClass::run
    );

    connect(ui->stoppushButton, &QPushButton::clicked,
        this, &TaskListWidgetsClass::stop
    );

//    connect(ui->saveproject_pushButton , &QPushButton::clicked,
//        this, &TaskListWidgetsClass::saveProject
//    );
//
//    connect(ui->openproject_pushButton, &QPushButton::clicked,
//        this, &TaskListWidgetsClass::readProject
//    );



    //this->
// 开辟一个子线程 把对象丢进去 开始
    processorQT = new AVS_TaskProcessor_QT(nullptr);

    processorQT->moveToThread(t1);
    t1->start();
    //信号通知 对象开始
    connect(this, &TaskListWidgetsClass::runSig, processorQT, &AVS_TaskProcessor_QT::run);

    //信号通知 停止
    connect(this, &TaskListWidgetsClass::stopSig, processorQT, &AVS_TaskProcessor_QT::stop);

    //绑定更新 progress_updataSig
    connect(processorQT,  &AVS_TaskProcessor_QT::progress_updataSig, this, [=](int tag) {
        //通过列表更新

        //reloadData()
        ui->listWidget->updataUI();
        });
    
     connect(&timer,&QTimer::timeout , this, [=](){
         ui->listWidget->updataUI();
//            qDebug()<< "uuu";
        });

    // 完成更新 completedSig
    connect(processorQT, &AVS_TaskProcessor_QT::completedSig, this, [=]() {
        ui->listWidget->updataUI();
        this->is_run = false;

        //判断是不是停止
        if (!this->ui->Cancel_pIndicator->isHidden()) {
            this->ui->Cancel_pIndicator->hide();
        }
        
        timer.stop();

        });

    //删除 信号 更新拖放ui用 delitemSig
    connect(ui->listWidget, &TaskQListWidget::delitemSig, this, &TaskListWidgetsClass::update_Drag_ui);



    // 取消等待控件
    ui->Cancel_pIndicator->setColor(Qt::gray);
    ui->Cancel_pIndicator->startAnimation();
    ui->Cancel_pIndicator->hide();




    // 文件拖放支持
    this->setAcceptDrops(true);//启用放下操作
    this->ui->listWidget->setAcceptDrops(true);

    //拖放图标按钮
    //
    //
    // drag_pushButton
    connect(ui->drag_pushButton, &QPushButton::clicked,
        this, &TaskListWidgetsClass::openFile
    );
    //run

    //ui->Drag_ui_widget->hide();
    //ui->Drag_gridLayout->hide();

    update_Drag_ui();



    //[=]() {


//    auto ll = this->taskOBJ->getTaskList();
//    for (size_t i = 0; i < ll.count(); i++)
//    {
//        qDebug() << ll[i]->raw_path << ll[i]->raw_duration;
//    }
//
//    //taskOBJ;
//
//    ui->listWidget->reloadData();
//}

    //del_obj

    //connect(ui->, &QPushButton::clicked,
    //    this, &TaskListWidgetsClass::openFile
    //);


}


//void TaskListWidgetsClass::init_TaskProcessor(TaskProcessor &processor){
//    // 希望运行时再去检查是否可以用
//
//
//
//
//}


//配置处理
bool TaskListWidgetsClass::init_AVS_Strategy(){
    
    bool isok = true;
    
    
    bool isload_instrumental = false;
    bool isload_vocals = false;
    
    
    //判断是不是加载
    if (avs_mdxnet_onnx_Strategy.m_fileLoader == nullptr){
        
        //由于用了QT解码 想分开基本库 所以在这边加载
        avs_mdxnet_onnx_Strategy.setLoaderFile(&loadfilr_Strategy);
        
        //设置取消关联指针
        avs_mdxnet_onnx_Strategy.setCancelledP(&m_is_cancelled);
        
        //配置处理引擎
        processor.setProcessingStrategy(&avs_mdxnet_onnx_Strategy);
        
        
        //获取路径
        //配置默认输出类型 TODO: 分开处理,并且引导去设置页面进行配置,并且不要继续
        QString instrumental_path = settings.value(QSettings_KEY_AVS_Instrumental_selection_name).toString();
        QString vocals_path = settings.value(QSettings_KEY_AVS_Vocals_selection_name).toString();
        
        isload_instrumental = avs_mdxnet_onnx_Strategy.loadOxxnModel_Instrumental(instrumental_path.toStdString());
        isload_vocals = avs_mdxnet_onnx_Strategy.loadOxxnModel_Vocals(vocals_path.toStdString());
        
        
    }
    
    //判断模型
    if (avs_mdxnet_onnx_Strategy.isLoadModel(MMType_Instrumental) == false) {
        //还没加载
//        int ret = QMessageBox::warning(this, tr("模型加载失败"),
//                                       tr("instrumental模型加载失败.\n"
//                                          ""),
//                                       QMessageBox::Ok,
//                                       QMessageBox::Ok);
        
        //设置按钮
        
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText( tr("instrumental模型加载失败.\n") + tr("背景音乐分离模型加载失败"));
        QPushButton *connectButton = msgBox.addButton(tr("去设置"), QMessageBox::ActionRole);
        msgBox.exec();
        
        if (msgBox.clickedButton() == connectButton) {
            // connect
//            去设置。把处理对象指针传过去
            
//            SHOW_SettingSpanelQtWidgetsClass_AVS_WIN(1, &avs_mdxnet_onnx_Strategy);
            emit toSettingSpanelQtWidgetsClass_AVS_WIN_Sig(&avs_mdxnet_onnx_Strategy);
            
            
            
        }

        
        
        
        isok = false;
        
    }
   
    if (avs_mdxnet_onnx_Strategy.isLoadModel(MMType_Vocals) == false){

        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText( tr("vocals模型加载失败.\n"
                           ) + tr("人声分离模型加载失败"));
        QPushButton *connectButton = msgBox.addButton(tr("去设置"), QMessageBox::ActionRole);
        msgBox.exec();
        
        if (msgBox.clickedButton() == connectButton) {
            // connect
//            去设置。把处理对象指针传过去
            
//            SHOW_SettingSpanelQtWidgetsClass_AVS_WIN(1, &avs_mdxnet_onnx_Strategy);
            emit toSettingSpanelQtWidgetsClass_AVS_WIN_Sig(&avs_mdxnet_onnx_Strategy);
            
            
            
        }
        
        

        
        isok = false;
    }
    

    
    return isok;
    
}





//没有列表数显示拖放图标按钮
void TaskListWidgetsClass::update_Drag_ui() {

//    if (avs_Tasks == nullptr) {
//        return;
//    }


    if (avs_Tasks.size() > 0) {
        ui->listWidget->show();
        ui->Drag_ui_widget->hide();
    }
    else
    {
        ui->listWidget->hide();
        ui->Drag_ui_widget->show();

    }

}

//文件添加模块 判断文件是否符合内容 添加到对象 更新界面
void TaskListWidgetsClass::openFile()
{
    QFileDialog fileDialog(this);
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    //可选择多个文件 //不能同时选择目录和文件
    fileDialog.setFileMode(QFileDialog::ExistingFiles);
    fileDialog.setWindowTitle(tr("选择视频文件"));
    fileDialog.setDirectory(QStandardPaths::standardLocations(QStandardPaths::MoviesLocation).value(0, QDir::homePath()));
    if (fileDialog.exec() == QDialog::Accepted) {

        

        QStringList strPathList = fileDialog.selectedFiles();
        add_to_obj(strPathList);
        

        



    




        
        


        //this->taskOBJ->addFormUrl(fileDialog.selectedUrls().constFirst());
        //ui->listWidget->reloadData();
    }
        //load();
}


//添加文件入口 选择完给列表 过滤是否可以用 最总添加
void TaskListWidgetsClass::add_to_obj(QStringList inputlist) {

    QStringList ok_path;
    QStringList on_path;

    //判断是否可以导入
    if_load(inputlist, &ok_path, &on_path);
    qDebug() << "ok" << ok_path;
    qDebug() << "no" << on_path;

    //提示是不是会员
//    if (this->is_ProQMessageBox()) {
//
//
//
//    }



    // 文件导入 f放在判断的地方导入
//    for (size_t i = 0; i < ok_path.count(); i++)
//    {
//        QUrl url = QUrl::fromLocalFile(ok_path[i]);
//        AVS_Task a;
//        a.setUrl(url.path().toStdString());
//        this->avs_Tasks.push_back(a);
//        //this->taskOBJ->addFormUrl(url);
//    }
    //刷新
    ui->listWidget->setSelect_row(this->avs_Tasks.size() - 1);//标记位置
    ui->listWidget->reloadData();

    //延迟1秒 刷新一下数据
    QTimer::singleShot(2000, this, [=]() {
        ui->listWidget->updataUI();

        });

    if (on_path.count() > 0) {
        //提示不能导入
        QString no_info = "";
        for (size_t j = 0; j < on_path.count(); j++)
        {
            no_info += on_path[j] + "\n";

        }
        

        QMessageBox::warning(this,
            tr("以下文件不能导入"),
            no_info,
            QMessageBox::Ok,
            QMessageBox::Ok);



    }



    //更新拖放ui
    update_Drag_ui();





}

//判断能不能创建对象入口
void TaskListWidgetsClass::if_load(QStringList inputlist, QStringList* ok_url, QStringList* no_url) {

    QStringList all_apth;


    for (size_t i = 0; i < inputlist.count(); i++)
    {
        QString Path = inputlist[i];
        QFileInfo fileInfo(Path);
        //qDebug() << Path;
        if (fileInfo.isDir()) {
            this->getFileList(Path, &all_apth);
        }
        else if (fileInfo.isFile())
        {
            all_apth.append(Path);

        }





    }

    //过滤能不能导入
    for (size_t j = 0; j < all_apth.count(); j++)
    {
        QString _Path = all_apth[j];
        QUrl url = QUrl::fromLocalFile(_Path);

// TODO: 判断是否能导入
        AVS_Task task_obj;
        if (task_QTtool.if_hasVideoV2(task_obj,url)) {
            
//            QUrl url = QUrl::fromLocalFile(ok_path[i]);
//            AVS_Task a;
//            a.setUrl(url.path().toStdString());
            
            //定义一个新建前入口处理
            init_avsTask_obj(task_obj);
            
            this->avs_Tasks.push_back(task_obj);
            //this->taskOBJ->addFormUrl(url);
            
            
            
            
            // 能导入
            ok_url->append(_Path);
        }
        else
        {
            //不能导入
            no_url->append(_Path);

        }

    }



}


//第一次创建任务对象配置
void TaskListWidgetsClass::init_avsTask_obj(AVS_Task& obj){
    
    obj.Engine = AVS_type::AVS_MDXNet;
    
    //配置默认输出类型
    QStringList default_OutType_list = settings.value(QSettings_KEY_AVS_OutType_list).toStringList();
    obj.m_mdxnet_type_list.clear();

    if(default_OutType_list.isEmpty()){
        //没有存储值 默认给选项
        obj.m_mdxnet_type_list.push_back(AVS_MDXNet_Models_Type::MMType_Instrumental);
        obj.m_mdxnet_type_list.push_back(AVS_MDXNet_Models_Type::MMType_Vocals);

    }else {

        for (QString i : default_OutType_list ) {

            if (i=="Instrumental"){
                obj.m_mdxnet_type_list.push_back(AVS_MDXNet_Models_Type::MMType_Instrumental);
            }
            if (i=="Vocals"){
                obj.m_mdxnet_type_list.push_back(AVS_MDXNet_Models_Type::MMType_Vocals);
            }


        }

    }


    

    
    
    
}

//遍历当前路径下文件
int  TaskListWidgetsClass::getFileList(const QString& dirPath, QStringList* outfileList)
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

    }
        return -1;

    int i = 0;
    //采用递归算法
    do {
        QFileInfo fileInfo = list.at(i);
        bool bisDir = fileInfo.isDir();
        if (bisDir) {

            getFileList(fileInfo.filePath(), outfileList);
        }
        else {
            outfileList->append(fileInfo.filePath());
            qDebug() << fileInfo.filePath() << ":" << fileInfo.fileName();
        }
            

        ++i;
    } while (i < list.size());

    return 0;
}


// 删除接口 //删除选中的
void TaskListWidgetsClass::del_obj() {
    // 判断数据是不是在运行
    //获取选中 删除对象 刷新数据
    ui->listWidget->del();

}

//输出文件目录
void TaskListWidgetsClass::initOutPath()
{
    //获取存储文件的目录  判断是否可以写入 不可以写入 就设定指定目录

    QString save_path = settings.value(Key_QSettings_OutPath).toString();
    

    QFileInfo info(save_path);
    //qDebug() << info.absoluteFilePath();
    //() << info.isWritable();
    bool path_isWritable = info.isWritable();

    if (path_isWritable) {

        this->out_path = save_path;
    }
    else
    {
        //获取默认的路径
        QString movies_path = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation) + "/UVAIGC/export";
        
        
        QDir dir(movies_path);
        if (!dir.exists())
        {
            //不存在 就新建
            bool ok = dir.mkpath(movies_path);//创建多级目录
            
        }
        this->out_path = movies_path;
    }

    //设置输出按钮
//    ui->svs_outpath_label->setText(this->out_path);
    
    QFontMetrics elideFont(ui->outpath_label_pushButton->font());
    QString text = elideFont.elidedText(this->out_path,
                                        Qt::ElideMiddle, this->width()*0.4);
    ui->outpath_label_pushButton->setText(text);

    //QFontMetrics elideFont(ui->commandLinkButton->font());
    //QString text = elideFont.elidedText(this->out_path, Qt::ElideRight, ui->commandLinkButton->width());
    //ui->commandLinkButton->setText(text);

    

}

void TaskListWidgetsClass::setOutPath()
{
     //选中文件夹
    QFileDialog fileDialog(this);
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    //可选择多个文件 //不能同时选择目录和文件
    fileDialog.setFileMode(QFileDialog::Directory);
    fileDialog.setWindowTitle(tr("选择默认保存目录"));
    fileDialog.setDirectory(QStandardPaths::standardLocations(QStandardPaths::MoviesLocation).value(0, QDir::homePath()));
    if (fileDialog.exec() == QDialog::Accepted) {
        //QStringList strPathList = fileDialog.selectedFiles();
        this->out_path = (fileDialog.selectedUrls().constFirst()).toLocalFile();
        this->saveOutPath();
        initOutPath();
       
        

       
        //

        //ui->commandLinkButton->setText(text);
    }


}

void TaskListWidgetsClass::saveOutPath()
{
    settings.setValue(Key_QSettings_OutPath,this->out_path);
}

void TaskListWidgetsClass::run()
{
    //加载模型 如果未假 就不运行
    
    if(init_AVS_Strategy() == false ){
        return;
    }
    
    
    
    //会员判断 非会员对对象进行处理
    if (!run_is_ProQMessageBox()) {

        return;
    }


    if (is_run) {
        return;
    }

    is_run = true;
    m_is_cancelled = false;
    
    //更新一下输出目录
    for (auto& i : this->avs_Tasks){
        i.out_path_dir = this->out_path.toStdString();
    }
    
    

    //推送去处理
    this->processorQT->setObj(&avs_Tasks,&processor);
//定时更新界面。完成结束
    timer.start(500);
    
    
    
    
    
    emit runSig();
    
    qDebug() <<"222";

}

void TaskListWidgetsClass::stop()
{
    if (!is_run) {
        return;
    }
    
    m_is_cancelled = true;

    emit stopSig();
    ui->Cancel_pIndicator->show();
}

// 支持文件拖放
//当用户拖动文件到窗口部件上时候，就会触发dragEnterEvent事件
void TaskListWidgetsClass::dragEnterEvent(QDragEnterEvent* event)
{
    //如果为文件，则支持拖放
     // 如果拖动的事件中有url，则允许drop
    if (event->mimeData()->hasUrls())
    {
        // 如果未调用，则无法drop
        event->acceptProposedAction();
    }
        
}

//当用户放下这个文件后，就会触发dropEvent事件
void TaskListWidgetsClass::dropEvent(QDropEvent* event)
{
    //注意：这里如果有多文件存在，意思是用户一下子拖动了多个文件，而不是拖动一个目录
    //如果想读取整个目录，则在不同的操作平台下，自己编写函数实现读取整个目录文件名
    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty())
        return;

    //往文本框中追加文件名
    QStringList strPathList;
    foreach(QUrl url, urls) {
        QString file_name = url.toLocalFile();
        //qDebug() << file_name;
        //
        //textEdit->append(file_name);
        strPathList.append(file_name);
    }

    if (strPathList.count() > 0) {

        add_to_obj(strPathList);
    }

    
}


//工程保存
void TaskListWidgetsClass::saveProject() {
    //保存文件选择窗口 保存

    //列表为空不能保存
    if (avs_Tasks.size() == 0) {
        //会覆盖

        auto m_box = QMessageBox::warning(this,
            tr("警告"),
            tr("列表为空,是否继续保存"),
            QMessageBox::Yes | QMessageBox::No, QMessageBox::No);


        if (m_box == QMessageBox::No) {

            return;
        }
        else
        {

        }




    }




    QString fileName;

    if (!this->project_out_fileName.isNull())
    {
        fileName = this->project_out_fileName;
    }
    else
    {

        fileName = QFileDialog::getSaveFileName(this,
            tr("保存工程"),
            "",
            tr("Files (*.wuvsocr)"));
    }
    


    

    
    



    if (!fileName.isNull())
    {
        //fileName是文件名

        //this->taskOBJ->saveProject(fileName);

        QFile file(fileName);           //定义文件路径
        file.open(QIODevice::WriteOnly); //以只写模式打开
        QDataStream out(&file);          //定义数据流
       // out << *(this->taskOBJ);
        file.close();


        this->project_out_fileName = fileName;


        //提示保存成功
        QMessageBox msgBox(this);
        msgBox.setText(QString(tr("保存成功")) + "\n" + QString(fileName));
        QTimer::singleShot(2000, &msgBox, SLOT(accept()));
        msgBox.exec();




    }
    else
    {
        //点的是取消
    }




}

void TaskListWidgetsClass::readProject() {
    //TODO：有项目提示会覆盖

    if (avs_Tasks.size() > 0) {
        //会覆盖

        auto m_box  = QMessageBox::warning(this,
            tr("警告"),
            tr("打开新项目，会覆盖当前项目。是否打开"),
            QMessageBox::Yes | QMessageBox::No, QMessageBox::No);


        if (m_box == QMessageBox::No) {

            return;
        }
        else
        {

        }




    }

//
//
//
//
//
//    QString fileName;
//    //QFileDialog::getOpenFileName
//    fileName = QFileDialog::getOpenFileName(this,
//        tr("打开工程"),
//        "",
//        tr("Files (*.wuvsocr)"));
//
//
//
//
//    if (!fileName.isNull())
//    {
//
//
//        QFile file_read(fileName);         //定义文件路径
//        file_read.open(QIODevice::ReadOnly); //以只读模式打开
//        QDataStream in(&file_read);          //定义数据流
//        auto taskOBJ_ls = TaskListOBJ(this);                    //创建字符串用于接收
//        in >> taskOBJ_ls;
//
//        //对比版本号  不一样就提示
//        if (taskOBJ_ls.versiom != this->taskOBJ->versiom) {
//
//            auto m_box = QMessageBox::warning(this,
//                tr("警告"),
//                tr("工程版本和本软件版本不一致，可能打不开") +
//                "\n" +
//                tr("工程版本号:") + QString::number(taskOBJ_ls.versiom),
//                QMessageBox::Yes, QMessageBox::Yes);
//
//        }
//
//
//        //删除-----旧的
//        for (size_t j = 0; j < taskOBJ->task_list.count(); j++)
//        {
//            auto obj = taskOBJ->task_list[j];
//            delete obj;
//
//        }
//        taskOBJ->task_list.clear();
//        ui->listWidget->reloadData();
//        // -----
//
//        for (size_t i = 0; i < taskOBJ_ls.task_list_count; i++)
//        {
//            auto obj = taskOBJ_ls.task_list[i];
//            qDebug() << obj->raw_path;
//
//
//
//            //验证文件是否存在 没有标记不存在
//            QFile file(obj->raw_path);
//            if (!file.exists())
//            {
//                qDebug() << "文件不存在";
//                obj->is_missing = true;
//            }
//
//
//            this->taskOBJ->addFormObj(obj);
//        }
//
//
//
//
//        //ui->listWidget->addData(taskOBJ);
//
//        ui->listWidget->reloadData();
//        update_Drag_ui();
//        //延迟1秒 刷新一下数据
//        QTimer::singleShot(2000, this, [=]() {
//            ui->listWidget->updataUI();
//
//            });
//        //QTimer::singleShot(2000, this, [=]() {
//        //
//
//        //    });
//
//        file_read.close();
//
//
//
//        this->project_out_fileName = fileName;


//    }


}


//是不是专业版提示   准备不用
bool TaskListWidgetsClass::is_ProQMessageBox() {

    if (MyUserModule_obj != nullptr) {
        if (MyUserModule_obj->isSuperMember()) {
            //是pro
            return true;
        }

    }



    QString Title = tr("当前不是专业版");
    QString text = tr("1,升级到专业版，功能没有限制，没有广告，拥有全部功能。") +"\n" +
        tr("2, 免费版本只能处理低于40秒的文件，不能批量转换。") +"\n";
    text += tr("是否去升级");
    QMessageBox* myBox = new QMessageBox(this);

    myBox->setWindowTitle(Title);
    myBox->setText(text);
    //myBox->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel | QMessageBox::);
    //myBox->setDefaultButton(QMessageBox::Ok);
    myBox->addButton(QMessageBox::Ok);
    myBox->addButton(QMessageBox::Cancel);
    myBox->setButtonText(QMessageBox::Ok, QObject::tr("去升级到专业版"));
    myBox->setButtonText(QMessageBox::Cancel, QObject::tr("继续使用"));
    int ret = myBox->exec();
    switch (ret)
    {
    case QMessageBox::Ok:
        emit toProwin_Sig();
        break;

    case QMessageBox::Cancel:
        qDebug() << "cancel!";
        break;
    }


    return false;
}

//开始运行 判断是不是会员
bool TaskListWidgetsClass::run_is_ProQMessageBox() {
    //判断是不是会员
    
    if (MyUserModule_obj != nullptr) {
        if (MyUserModule_obj->isSuperMember()) {
            //是pro
            return true;
        }

    }



    QString Title = tr("当前不是专业版");
    QString text = Title + "\n" + tr("1, 免费版本只能处理单个文件") +"\n" +
        tr("2,专业版本，功能没有限制、没有广告，拥有全部功能.") +"\n";
    text += tr("是否去升级");
    QMessageBox* myBox = new QMessageBox(this);

    myBox->setWindowTitle(Title);
    myBox->setText(text);
    //myBox->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel | QMessageBox::);
    //myBox->setDefaultButton(QMessageBox::Ok);
    myBox->addButton(QMessageBox::Ok);
    myBox->addButton(QMessageBox::Cancel);
    myBox->setButtonText(QMessageBox::Ok, QObject::tr("去升级到专业版"));
    myBox->setButtonText(QMessageBox::Cancel, QObject::tr("继续使用"));
    int ret = myBox->exec();
    switch (ret)
    {
    case QMessageBox::Ok:
        emit toProwin_Sig();
            //不在继续
        return false;
        break;

    case QMessageBox::Cancel:
        qDebug() << "cancel!";
        break;
    }

//    非会员
//    判断数量
    if (avs_Tasks.size() >= 1) {
        //只留下第一个
        auto obj = avs_Tasks[0];

//        for (size_t i = 1; i < avs_Tasks.size(); i++)
//        {
//            auto delobj = avs_Tasks[i];
//            delete delobj;
//        }
        avs_Tasks.clear();

        avs_Tasks.push_back(obj);
        //刷新
        this->ui->listWidget->reloadData();

//        if (obj->raw_duration > 40000) {
//            //不允许识别
//            return false;
//        }
    }
    
    







    return true;

}


//导出所有Export 导出完成 告诉一共导出成功几个失败几个 显示输出文件夹
void TaskListWidgetsClass::exportAllSRT() {
//    this->exportAllobj(0);

}
void TaskListWidgetsClass::exportAllTXT() {
//    this->exportAllobj(1);
}


//封装已经存在是否覆盖弹窗
bool TaskListWidgetsClass::isFile_exporQMessageBox(QStringList is_isFile) {

    QString Title = tr("文件已经存在 是否覆盖");
    QString text;
    foreach(QString s, is_isFile)
    {
        text += s + "\n";
    }



    QMessageBox* myBox = new QMessageBox(this);

    myBox->setWindowTitle(Title);
    myBox->setText(text);
    //myBox->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel | QMessageBox::);
    myBox->setDefaultButton(QMessageBox::Cancel);
    myBox->addButton(QMessageBox::Ok);
    myBox->addButton(QMessageBox::Cancel);
    myBox->setButtonText(QMessageBox::Ok, QObject::tr("覆盖"));
    myBox->setButtonText(QMessageBox::Cancel, QObject::tr("取消"));
    int ret = myBox->exec();
    switch (ret)
    {
    case QMessageBox::Ok:
        return true;
        break;

    case QMessageBox::Cancel:
        return false;;
        break;
    }



}

//封装 最后成功提示
void TaskListWidgetsClass::exportOKQMessageBox(QStringList is_completed,QStringList no_completed) {

    QString Title = tr("导出统计");
    QString text="";
    int allco = is_completed.count() + no_completed.count();
    text += tr("成功：") + QString(" %1 / %2").arg(QString::number(is_completed.count())).arg(QString::number(allco));
    text += "\n\n";
    text += tr("失败:") + QString(" %1 / %2").arg(QString::number(no_completed.count())).arg(QString::number(allco)) +"\n";

    foreach(QString s, no_completed)
    {
        text += s + "\n";
    }



    QMessageBox* myBox = new QMessageBox(this);

    myBox->setWindowTitle(Title);
    myBox->setText(text);
    //myBox->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel | QMessageBox::);
    myBox->addButton(QMessageBox::Ok);
    myBox->addButton(QMessageBox::Cancel);
    myBox->setButtonText(QMessageBox::Ok, QObject::tr("显示输出文件夹"));
    myBox->setButtonText(QMessageBox::Cancel, QObject::tr("关闭"));
    int ret = myBox->exec();
    switch (ret)
    {
    case QMessageBox::Ok:
    {
        auto pathIn = this->out_path;
        const QString explorer = "explorer";
        QStringList param;
        if (!QFileInfo(pathIn).isDir())
            param << QLatin1String("/select,");
        param << QDir::toNativeSeparators(pathIn);
        QProcess::startDetached(explorer, param);
    }
        break;
    }




}



//快捷键
void  TaskListWidgetsClass::keyPressEvent(QKeyEvent* event)
{
    qDebug() <<"jjj" << event->key();

    //QMediaPlayer::keyPressEvent(event);
    //保存  一个删除
//    if (event->key() == Qt::Key_S && event->modifiers() == Qt::ControlModifier)
//    {
//        // ;
//        
//        saveProject();
//
//        //QWidget::keyPressEvent(event);
//        return;
//    }



    switch (event->key()) {
    case Qt::Key_Delete:
        del_obj();
        break; // 可选的
    
    default: // 可选的
        QWidget::keyPressEvent(event);
        break; // 可选的


    }
    //QWidget::keyPressEvent(event);
}

   
    


TaskListWidgetsClass::~TaskListWidgetsClass()
{
    
   
    
    
    delete ui;
    
}
