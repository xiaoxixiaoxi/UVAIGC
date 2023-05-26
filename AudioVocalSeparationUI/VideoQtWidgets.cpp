#include "VideoQtWidgets.h"

#include "VideoQtWidgets.h"
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




void VideoQtWidgets::closeEvent(QCloseEvent* event)
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

VideoQtWidgets:: VideoQtWidgets(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	//������
//	ui.pushButton_3->setVisible(0);
//	ui.pushButton->setVisible(0);
//	ui.infolabel->setVisible(0);

	//QUrl videourl = QUrl::fromLocalFile("D:/Sprite Fright.mp4");
	//videourl.toLocalFile();
	//auto my_ocr = new My_video_subORC(videourl.toString());

	//myInIt(my_ocr);



}

//��ڳ�ʼ��
bool VideoQtWidgets::myInIt(AVS_Task* mytask) {

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
    ui.graphicsView->setScene(scene);

    QRectF screenGeometry = this->contentsRect();

    qDebug() << "¥Û–°£∫" << screenGeometry;

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
    
    
    
    //关联分离类型
    std::vector<AVS_MDXNet_Models_Type>& outpath_list = this->mytask->m_mdxnet_type_list;

    ui.Instrumental_checkBox->setChecked(false);
    ui.Vocals_checkBox->setChecked(false);
    for (AVS_MDXNet_Models_Type& i:outpath_list ) {
        
        if (i==AVS_MDXNet_Models_Type::MMType_Instrumental){
            ui.Instrumental_checkBox->setChecked(true);
        }
        if (i==AVS_MDXNet_Models_Type::MMType_Vocals){
            ui.Vocals_checkBox->setChecked(true);
        }
        
       
    }
    
    
    //绑定默认选择输出类型
    connect(ui.Vocals_checkBox, &QPushButton::clicked, this, &VideoQtWidgets::set_default_Models_Type_list);
    connect(ui.Instrumental_checkBox, &QPushButton::clicked, this, &VideoQtWidgets::set_default_Models_Type_list);
    



   


    //obj = new MyVideoQtClass();
    //MyOCR obj = MyOCR();
    //obj->re

    //QtWidgetsClass * w2 = new QtWidgetsClass();

    //////w2.m_mediaPlayer = m_mediaPlayer;
    //w2->show();

















    //connect(ui.pushButton, &QPushButton::clicked,
       // this, &VideoQtWidgets::openFile);

    connect(ui.pushButton_2, &QPushButton::clicked,
        this, &VideoQtWidgets::play);




    //�����ó����
    connect(ui.setInTimetoolButton, &QPushButton::clicked,
        this, &VideoQtWidgets::setInTime);
    // �󶨳���
    connect(ui.setOutTimetoolButton, &QPushButton::clicked,
        this, &VideoQtWidgets::setOutTime);


    connect(ui.LtoolButton, &QPushButton::clicked,
        this, &VideoQtWidgets::L_Play);
    connect(ui.JtoolButton, &QPushButton::clicked,
        this, &VideoQtWidgets::J_Play);


   /* t1->start();*/

    this->setFocus();

    this->m_mediaPlayer->play();

    //initCropRect();

    return true;





}


 



 VideoQtWidgets::~ VideoQtWidgets()
{
	 
	//�ͷ�
//	 delete m_mediaPlayer;

	/* if (qv != nullptr) {
		 delete qv;
	 }*/

	// delete t1;
	 qDebug() << "del obj";




}

bool VideoQtWidgets::set_mytask(AVS_Task* mytask){
    myInIt(mytask);
}






void  VideoQtWidgets::openFile()
{
	QFileDialog fileDialog(this);
	fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
	fileDialog.setWindowTitle(tr("ѡ����Ƶ�ļ�"));
	fileDialog.setDirectory(QStandardPaths::standardLocations(QStandardPaths::MoviesLocation).value(0, QDir::homePath()));
	if (fileDialog.exec() == QDialog::Accepted)
		load(fileDialog.selectedUrls().constFirst());
}

void  VideoQtWidgets::load(const QUrl& url)
{
	m_mediaPlayer->setSource(url);
	//m_playButton->setEnabled(true);

	qDebug() << "loadvideo" << url;
}

bool  VideoQtWidgets::isPlayerAvailable() const
{
	return m_mediaPlayer->isAvailable();
}

void  VideoQtWidgets::play()
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
void  VideoQtWidgets::setInTime() {

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
void  VideoQtWidgets::setOutTime() {
	double outTime_bl = (m_mediaPlayer->position() * 1.0) / m_mediaPlayer->duration();
	this->ui.horizontalSlider->setOutTime_r(outTime_bl);
	this->ui.horizontalSlider->update();

    //this->my_ocr->setoutTimeRange(m_mediaPlayer->position());
    this->mytask->setOutTimeRange(m_mediaPlayer->position());

	// ��ʾ��
	SRTQTime srt_t(m_mediaPlayer->position());
	ui.out_timelabel->setText(srt_t.toString("hh:mm:ss,zzz"));
}
void  VideoQtWidgets::L_Play() {

	auto rate = m_mediaPlayer->playbackRate();
	if (rate < 0) {
		rate = 0;

	}
	rate += 0.5;
	m_mediaPlayer->setPlaybackRate(rate);

}
void  VideoQtWidgets::J_Play() {

	auto rate = m_mediaPlayer->playbackRate();
	if (rate > 0) {
		rate = 0;

	}
	rate -= 0.50;
	m_mediaPlayer->setPlaybackRate(rate);

	//play();

}


void  VideoQtWidgets::keyPressEvent(QKeyEvent* event)
{
	//qDebug() <<"jjj" << event->key();

	//QMediaPlayer::keyPressEvent(event);

	//if (event->key() == Qt::Key_Up && event->modifiers() == Qt::ControlModifier)
	//{
	//	// ;
	//	QWidget::keyPressEvent(event);
	//	return;
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
		

	default: // ��ѡ��
		QWidget::keyPressEvent(event);
		break; // ��ѡ��


	}
	//QWidget::keyPressEvent(event);
}



//分离类型选择
void VideoQtWidgets::set_default_Models_Type_list(){
    

    std::vector<AVS_MDXNet_Models_Type> default_out_list;

    if (ui.Instrumental_checkBox->isChecked()){
        
        default_out_list.push_back(AVS_MDXNet_Models_Type::MMType_Instrumental);
    }
    if (ui.Vocals_checkBox->isChecked()){
        
        default_out_list.push_back(AVS_MDXNet_Models_Type::MMType_Vocals);
    }
    
   this->mytask->m_mdxnet_type_list = default_out_list;
    
    
}


// 自动缩放
void  VideoQtWidgets::resizeEvent(QResizeEvent* event)
{
    //qDebug() << "¥∞ø⁄¥Û–°" << size();

    //QRectF screenGeometry = this->contentsRect();
    if (m_videoItem == nullptr) {
        return;
    }

    m_videoItem->setSize(ui.graphicsView->geometry().size() - QSize(2, 2));



    

}



