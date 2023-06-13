#include "sr_editqtwidgets.h"
#include "ui_sr_editqtwidgets.h"


#include <QAudioOutput>
//#include "myocr.h"

//#include "tesseract/baseapi.h"

//#include "WinOCR.h"

//ϵͳ����
#include<QTranslator>
#include <QLocale>
//#include "MyOBJmain.h"
//#include "SettingSpanel_main.h"
#include "../Basis/SRTQTime.h"

#include "../SettingSpanel/sr_settingqtwidgetsclass.h"




void SR_editQtWidgets::closeEvent(QCloseEvent* event)
{
    qDebug() << "close";

    

    //event->ignore();
    
    if (m_mediaPlayer != nullptr) {

        delete m_mediaPlayer;
    }
    
    
    //delete qv;
    
    
    /*t1->deleteLater();
    delete t1;*/

    this->close();

    emit closeSig();

    //QtWidgets::closeEvent(event);

}

SR_editQtWidgets::SR_editQtWidgets(QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    //������
//    ui.pushButton_3->setVisible(0);
//    ui.pushButton->setVisible(0);
//    ui.infolabel->setVisible(0);

    //QUrl videourl = QUrl::fromLocalFile("D:/Sprite Fright.mp4");
    //videourl.toLocalFile();
    //auto my_ocr = new My_video_subORC(videourl.toString());

    //myInIt(my_ocr);



}

//��ڳ�ʼ��
bool SR_editQtWidgets::myInIt(SR_Task* mytask) {

    // ʶ�����Դ���
    
    //�ⲿ���봦������
   /* QUrl videourl = QUrl::fromLocalFile("D:/Sprite Fright.mp4");
    videourl.toLocalFile();*/

   // my_ocr = new My_video_subORC(videourl.toString());
    this->mytask = mytask;

    this->setWindowTitle(QString::fromStdString(this->mytask->getName()));
    
    
    


    

    
    

    







    m_mediaPlayer = new QMediaPlayer();
    QAudioOutput* audioOutput = new QAudioOutput;
    m_mediaPlayer->setAudioOutput(audioOutput);
    
    
    
//
    audioOutput->setVolume(50);
    
    
    
    QGraphicsScene* scene = new QGraphicsScene(this);
    m_videoItem = new QGraphicsVideoItem;
    //QGraphicsView* graphicsView = new QGraphicsView(scene);
    
    // 如果是图片 添加显示图片
    if (mytask->media_type == SR_Task::Image){
        
        QGraphicsPixmapItem* originalPixmapItem = new QGraphicsPixmapItem();
        QGraphicsPixmapItem* enlargedPixmapItem = new QGraphicsPixmapItem();

        // 将 QGraphicsPixmapItem 添加到场景中
        scene->addItem(originalPixmapItem);
        scene->addItem(enlargedPixmapItem);
        
        // 加载原始图像和放大后的图像
        QPixmap originalPixmap(QString::fromStdString(mytask->getUrl()));
        originalPixmapItem->setPixmap(originalPixmap);
        originalPixmapItem->setPos(0, 0);
        
        if (mytask->getState() == TaskBasis::Finished){
            QPixmap enlargedPixmap(QString::fromStdString(mytask->out_path_list[0]));

            // 设置原始图像和放大后的图像到 QGraphicsPixmapItem
           
            enlargedPixmapItem->setPixmap(enlargedPixmap);
            enlargedPixmapItem->setPos(originalPixmap.width() + 10, 0);
            
        }
       
        
        
        
        
//        scene->addPixmap(QPixmap(QString::fromStdString(mytask->getUrl())));
    }
    
    ui.graphicsView->setScene(scene);

    QRectF screenGeometry = this->contentsRect();

//    qDebug() << "¥Û–°£∫" << screenGeometry;

    //m_videoItem->setSize(screenGeometry.size());
    

    scene->addItem(m_videoItem);
    
    
    m_mediaPlayer->setVideoOutput(m_videoItem);



    //�� ���õ�ʱ�򴥷� hasVideoChanged
    connect(m_mediaPlayer, &QMediaPlayer::positionChanged,
        this, [=](qint64 nowTime) {

            //�ж��ǲ��ǵ�һ�μ���
            qint64 du = m_mediaPlayer->duration();

            bool tes = nowTime < du;

            if ((nowTime > 100 )&&(nowTime < du) && is_oneLoad == false) {

                // ֹͣ����  ��һ�μ������
                this->m_mediaPlayer->pause();
               // this->initCropRect();
                this->is_oneLoad = true;


                //���ó������ʾ
                double outTime_bl = (this->mytask->timeRange_out * 1.0) / m_mediaPlayer->duration();
                this->ui.horizontalSlider->setOutTime_r(outTime_bl);

                double inTime_bl = (this->mytask->timeRange_in * 1.0) / m_mediaPlayer->duration();
                this->ui.horizontalSlider->setInTime_r(inTime_bl);

                this->ui.horizontalSlider->inTimeUpdate(inTime_bl);
                this->ui.horizontalSlider->outTimeUpdate(outTime_bl);


                this->ui.horizontalSlider->update();
                
                this->m_mediaPlayer->setPosition(0);




            }

           


//            double now_bl = nowTime * 1.0 / m_mediaPlayer->duration();
//            //ui.
//            ui.horizontalSlider->setNowTime_r(now_bl);
//            ui.horizontalSlider->update();

    

//            ui.nowtimelabel->setText(srt_t.toString("hh:mm:ss,zzz"));


            // ������Ļ�༭
//            ui.listWidget->nowTime(nowTime);
            



        }
    );


    //�󶨽�����
    //positionChanged
    connect(m_mediaPlayer, &QMediaPlayer::positionChanged,
        this, [=](qint64 nowTime) {

            double now_bl = nowTime * 1.0 / m_mediaPlayer->duration();
            //ui.
            ui.horizontalSlider->setNowTime_r(now_bl);
            ui.horizontalSlider->update();

            SRTQTime srt_t(nowTime);

            ui.nowtimelabel->setText(srt_t.toString("hh:mm:ss,zzz"));



        }
    );

    //�󶨳����ʱ�����
    connect(ui.horizontalSlider, &MySlider::inTimeUpdate,
        this, [=](double inTime_bl) {

            qint64 inTime = inTime_bl * m_mediaPlayer->duration();

            m_mediaPlayer->setPosition(inTime);

            //ͬ��������
//            my_ocr->setInTimeRange(inTime);
        this->mytask->setInTimeRange(inTime);

            SRTQTime srt_t(inTime);


            ui.in_timelabel->setText(srt_t.toString("hh:mm:ss,zzz"));
            //qDebug() << inTime << srt_t.toString();



        }
    );

    //�󶨳�����ʱ�����
    connect(ui.horizontalSlider, &MySlider::outTimeUpdate,
        this, [=](double outTime_bl) {

            qint64 outTime = outTime_bl * m_mediaPlayer->duration();

            m_mediaPlayer->setPosition(outTime);

            //ͬ��������
//            my_ocr->setoutTimeRange(outTime);
        this->mytask->setOutTimeRange(outTime);

            SRTQTime srt_t(outTime);

            ui.out_timelabel->setText(srt_t.toString("hh:mm:ss,zzz"));


        }
    );

    //�󶨵�ǰʱ�����
    connect(ui.horizontalSlider, &MySlider::nowTimeUpdate,
        this, [=](double nowTime_bl) {

            qint64 nowTime = nowTime_bl * m_mediaPlayer->duration();

            m_mediaPlayer->setPosition(nowTime);
            SRTQTime srt_t(nowTime);

            ui.nowtimelabel->setText(srt_t.toString("hh:mm:ss,zzz"));

        }
    );




    //qv = new  QVideoSink();
    //m_mediaPlayer->setVideoSink(qv);

    


     // ������Ƶ���
//    m_mediaPlayer->setVideoOutput(m_videoItem);

    QUrl videourl = QUrl::fromLocalFile(QString::fromStdString(this->mytask->getUrl())    );
    m_mediaPlayer->setSource(videourl);
   // m_mediaPlayer->play();
    m_mediaPlayer->seekableChanged(true);




    connect(ui.tohomepushButton, &QPushButton::clicked,
        this, [=]() {
            this->close();




        });
    
    //加载可选择模型
    init_SR01_model_selection_name();

    
    
    
    //关联分离类型
    init_Video_image_OutType();
    
    //绑定缩放
    
    connect(ui.zoom_in_pushButton, &QPushButton::clicked,
            this, [=](){
        ui.graphicsView->scale(1.2, 1.2);
//        setSceneRect(0,0,ui.graphicsView->scene()->width()*0.5, ui.graphicsView->scene()->height()*0.5);
//        ui.graphicsView->scene()->setSceneRect(0,0,ui.graphicsView->scene()->width(), ui.graphicsView->scene()->height());
        QPointF scenePos(0,0);
        ui.graphicsView->centerOn(scenePos);
    });
    connect(ui.zoom_out_pushButton, &QPushButton::clicked,
            this, [=](){
        
        ui.graphicsView->scale(0.8, 0.8);
//        setSceneRect(0,0,ui.graphicsView->scene()->width()*0.5, ui.graphicsView->scene()->height()*0.5);
//        ui.graphicsView->scene()->setSceneRect(0,0,ui.graphicsView->scene()->width(), ui.graphicsView->scene()->height());
        QPointF scenePos(0,0);
        ui.graphicsView->centerOn(scenePos);
        
    });




   


    //obj = new MyVideoQtClass();
    //MyOCR obj = MyOCR();
    //obj->re

    //QtWidgetsClass * w2 = new QtWidgetsClass();

    //////w2.m_mediaPlayer = m_mediaPlayer;
    //w2->show();

















    //connect(ui.pushButton, &QPushButton::clicked,
       // this, &SR_editQtWidgets::openFile);

    connect(ui.pushButton_2, &QPushButton::clicked,
        this, &SR_editQtWidgets::play);




    //�����ó����
    connect(ui.setInTimetoolButton, &QPushButton::clicked,
        this, &SR_editQtWidgets::setInTime);
    // �󶨳���
    connect(ui.setOutTimetoolButton, &QPushButton::clicked,
        this, &SR_editQtWidgets::setOutTime);


    connect(ui.LtoolButton, &QPushButton::clicked,
        this, &SR_editQtWidgets::L_Play);
    connect(ui.JtoolButton, &QPushButton::clicked,
        this, &SR_editQtWidgets::J_Play);


   /* t1->start();*/

    this->setFocus();

    this->m_mediaPlayer->play();

    //initCropRect();

    return true;





}


 



 SR_editQtWidgets::~ SR_editQtWidgets()
{
     
    //�ͷ�
//     delete m_mediaPlayer;

    /* if (qv != nullptr) {
         delete qv;
     }*/

    // delete t1;
     qDebug() << "del obj";




}

bool SR_editQtWidgets::set_mytask(SR_Task* mytask){
    myInIt(mytask);
    return true;
}






void  SR_editQtWidgets::openFile()
{
    QFileDialog fileDialog(this);
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setWindowTitle(tr("ѡ����Ƶ�ļ�"));
    fileDialog.setDirectory(QStandardPaths::standardLocations(QStandardPaths::MoviesLocation).value(0, QDir::homePath()));
    if (fileDialog.exec() == QDialog::Accepted)
        load(fileDialog.selectedUrls().constFirst());
}

void  SR_editQtWidgets::load(const QUrl& url)
{
    m_mediaPlayer->setSource(url);
    //m_playButton->setEnabled(true);

    qDebug() << "loadvideo" << url;
}

bool  SR_editQtWidgets::isPlayerAvailable() const
{
    return m_mediaPlayer->isAvailable();
}

void  SR_editQtWidgets::play()
{
    qDebug() << m_mediaPlayer->playbackState() << isPlayerAvailable();

    //�ж��ٶ��ǲ������� �������ɵ�������
    auto Rate = m_mediaPlayer->playbackRate();
    if (Rate != 1.0) {
        m_mediaPlayer->setPlaybackRate(1.0);
    }
    //Rate = 1.0;


    switch (m_mediaPlayer->playbackState()) {
    case QMediaPlayer::PlayingState:
        m_mediaPlayer->pause();
        break;
    default:
        m_mediaPlayer->play();
        break;
    }

    // ���������Ƶ ��ݼ�������
    this->setFocus();
}











//���� ���
void  SR_editQtWidgets::setInTime() {

    double inTime_bl = (m_mediaPlayer->position()*1.0)/ m_mediaPlayer->duration();
    this->ui.horizontalSlider->setInTime_r(inTime_bl);
    this->ui.horizontalSlider->update();
    //ͬ������

    //this->my_ocr->setInTimeRange(m_mediaPlayer->position());
    this->mytask->setInTimeRange(m_mediaPlayer->position());

    // ��ʾ��
    SRTQTime srt_t(m_mediaPlayer->position());
    ui.in_timelabel->setText(srt_t.toString("hh:mm:ss,zzz"));

    // ����ʱ����ʾ
    
    //.horizontalSlider.setInTime_r()

}
void  SR_editQtWidgets::setOutTime() {
    double outTime_bl = (m_mediaPlayer->position() * 1.0) / m_mediaPlayer->duration();
    this->ui.horizontalSlider->setOutTime_r(outTime_bl);
    this->ui.horizontalSlider->update();

    //this->my_ocr->setoutTimeRange(m_mediaPlayer->position());
    this->mytask->setOutTimeRange(m_mediaPlayer->position());

    // ��ʾ��
    SRTQTime srt_t(m_mediaPlayer->position());
    ui.out_timelabel->setText(srt_t.toString("hh:mm:ss,zzz"));
}
void  SR_editQtWidgets::L_Play() {

    auto rate = m_mediaPlayer->playbackRate();
    if (rate < 0) {
        rate = 0;

    }
    rate += 0.5;
    m_mediaPlayer->setPlaybackRate(rate);

}
void  SR_editQtWidgets::J_Play() {

    auto rate = m_mediaPlayer->playbackRate();
    if (rate > 0) {
        rate = 0;

    }
    rate -= 0.50;
    m_mediaPlayer->setPlaybackRate(rate);

    //play();

}


void  SR_editQtWidgets::keyPressEvent(QKeyEvent* event)
{
//    qDebug() <<"jjj" << event->key();

    //QMediaPlayer::keyPressEvent(event);

    //if (event->key() == Qt::Key_Up && event->modifiers() == Qt::ControlModifier)
    //{
    //    // ;
    //    QWidget::keyPressEvent(event);
    //    return;
    //}



    switch (event->key()) {
    case Qt::Key_I:
        setInTime();
        break;
    case Qt::Key_O:
        setOutTime();
        break;
    case Qt::Key_L:
        L_Play();
        break;
    case Qt::Key_J:
        J_Play();
        break;

    case Qt::Key_Space:
        play();
        break;
    case Qt::Key_Left:
    {
        qint64 time = m_mediaPlayer->position();
        
        m_mediaPlayer->setPosition(time - 2000);

    }
        break;
    case Qt::Key_Right:
    {
        qint64 time = m_mediaPlayer->position();

        m_mediaPlayer->setPosition(time + 2000);
        
    }
        break;
    case Qt::Key_Minus://-
    {
        ui.graphicsView->scale(0.8, 0.8);
//        setSceneRect(0,0,ui.graphicsView->scene()->width()*0.5, ui.graphicsView->scene()->height()*0.5);
//        ui.graphicsView->scene()->setSceneRect(0,0,ui.graphicsView->scene()->width(), ui.graphicsView->scene()->height());
        QPointF scenePos(0,0);
        ui.graphicsView->centerOn(scenePos);
        
        
    }
        break;
    case Qt::Key_Equal://+
    {
        //计算图片中心
        ui.graphicsView->scale(1.2, 1.2);
        QPointF scenePos(0,0);
        ui.graphicsView->centerOn(scenePos);
//        ui.graphicsView->scene()->setSceneRect(0,0,ui.graphicsView->scene()->width(), ui.graphicsView->scene()->height());
        
        
    }
        break;
    
        

    default: // ��ѡ��
        QWidget::keyPressEvent(event);
        break; // ��ѡ��


    }
    //QWidget::keyPressEvent(event);
}



//分离类型选择
void SR_editQtWidgets::set_default_Models_Type_list(){
    

//    std::vector<AVS_MDXNet_Models_Type> default_out_list;
//
//    if (ui.Instrumental_checkBox->isChecked()){
//
//        default_out_list.push_back(AVS_MDXNet_Models_Type::MMType_Instrumental);
//    }
//    if (ui.Vocals_checkBox->isChecked()){
//
//        default_out_list.push_back(AVS_MDXNet_Models_Type::MMType_Vocals);
//    }
//
//   this->mytask->m_mdxnet_type_list = default_out_list;
    
    
}


// 自动缩放
void  SR_editQtWidgets::resizeEvent(QResizeEvent* event)
{
    //qDebug() << "¥∞ø⁄¥Û–°" << size();

    //QRectF screenGeometry = this->contentsRect();
    if (m_videoItem == nullptr) {
        return;
    }

    m_videoItem->setSize(ui.graphicsView->geometry().size() - QSize(2, 2));



    

}


void  SR_editQtWidgets::init_SR01_model_selection_name(){
    
    
    QString selectionName =  mytask->sr01_model_name;
    
    //获取保存路径
    QSettings settings = QSettings(ORGANIZATION_Name, APPLICATION_Name);
    QString model_save_path = settings.value(QSettings_KEY_SR_model_save_path).toString();
    
    
    QStringList outfileList;
    SR_settingqtwidgetsclass::getFileList(model_save_path, &outfileList);
    

    ui.model_Instrumental_selection_comboBox->clear();
    int currentIndex = 0;
    int index = 0;
    for (auto i : outfileList) {
        if (i == selectionName) {
            currentIndex = index;
        }
        //
        //QString showname = QFileInfo f_newname(new_name);
        QFileInfo showname(i);
        ui.model_Instrumental_selection_comboBox->addItem(showname.fileName(), QVariant(i));
        index += 1;
    }


    ui.model_Instrumental_selection_comboBox->setCurrentIndex(currentIndex);
    
   //展示可放大倍数
    set_SR01_models_scaled();
    
    
    
    //绑定选择
    connect(ui.model_Instrumental_selection_comboBox, &QComboBox::currentTextChanged, this, [=](QString showname) {
        mytask->sr01_model_name = ui.model_Instrumental_selection_comboBox->currentData().toString();

        //展示可放大倍数
        set_SR01_models_scaled();
        });

    
    
    //展示
    //设置选中
    
    
    
    
    
}


//展示可放大倍数
void  SR_editQtWidgets::set_SR01_models_scaled(){
    
    
    //关联可以支持的放大倍数
    QString Scaled_info = tr("未知");
    std::string currentName = (ui.model_Instrumental_selection_comboBox->currentText()).toStdString();
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
    
    
    
    
    ui.scaled_label->setText(Scaled_info);
    
}


void SR_editQtWidgets::init_Video_image_OutType(){
    
    //设置
//    ui.horizontalLayout_3->set;
    
    // 输出类型
    if (mytask->media_type == SR_Task::Video){
        ui.video_out_type_widget->setVisible(true);
        ui.image_out_type_widget->setVisible(false);
        
        
        
        //恢复
        std::string default_Video_OutType = mytask->outType;
        if (default_Video_OutType == "mov") {
            ui.video_radioButton_mov->setChecked(true);
        }else if (default_Video_OutType == "mp4"){
            ui.video_radioButton_mp4->setChecked(true);
        }else{
            ui.video_radioButton_default->setChecked(true);
        }
        
        //输出类型关联
        connect(ui.out_video_buttonGroup, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked),
                             [&](QAbstractButton *button) {
            std::string video_OutType = "default";
            //判断逻辑
            if (button == ui.video_radioButton_default) {
                video_OutType = "default";
            }
            else if(button == ui.video_radioButton_mov) {
                video_OutType = "mov";
            }
            else if(button == ui.video_radioButton_mp4) {
                video_OutType = "mp4";
            }
            
            
            //获取名称
            QFileInfo fileInfo = QFileInfo(QString::fromStdString( mytask->getUrl()));
            if (video_OutType == "default" || video_OutType ==""){
                video_OutType = fileInfo.suffix().toStdString();
            }
            mytask->outType = video_OutType;


                             });
        
        
    }else if(mytask->media_type == SR_Task::Image){
        ui.video_out_type_widget->setVisible(false);
        ui.image_out_type_widget->setVisible(true);
        
        //隐藏视频相关工具栏
        ui.video_toole_widget2->setVisible(false);
        ui.video_toole_widget->setVisible(false);
        
        
        std::string  default_Image_OutType = mytask->outType;
        if (default_Image_OutType == "jpg") {
           
            ui.image_radioButton_jpg->setChecked(true);
        }else{
            ui.image_radioButton_png->setChecked(true);
        }
        
        
        //输出类型关联
        connect(ui.out_image_buttonGroup, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked),
                             [&](QAbstractButton *button) {
            std::string image_OutType = "png";
            //判断逻辑
            if (button == ui.image_radioButton_jpg) {
                image_OutType = "jpg";
            }
            else{
                image_OutType = "png";
            }
            //储存起来
            mytask->outType = image_OutType;
            
                             });
        
    }

   
    


   
    
    
    
    
    
    
    
    
    
}



