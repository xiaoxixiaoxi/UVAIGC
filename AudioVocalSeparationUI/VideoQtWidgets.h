#pragma once

#include <QWidget>
#include "ui_VideoQtWidgets.h"

#include <QFileDialog>
#include <QStandardPaths>

#include <QtGui/qevent.h>



//#include <QtMultimediaWidgets/qtmultimediawidgetdefs.h>
//#include <QtMultimediaWidgets/qtmultimediawidgetsversion.h>
//#include <QGraphicsVideoItem>
#include <QMediaPlayer> 
#include <QVideoSink>

//#include <QMouseEven>
#include <QThread>
#include "MySlider.h"
#include<QMessageBox>

#include "../AudioVocalSeparation/AVS_Task.hpp"
#include <QGraphicsVideoItem>


class  VideoQtWidgets : public QWidget
{
	Q_OBJECT

protected:
	void closeEvent(QCloseEvent* event);

public:
	 VideoQtWidgets(QWidget* parent = nullptr);
//	 VideoQtWidgets(My_video_subORC* myocr);
	~ VideoQtWidgets();

	bool set_mytask(AVS_Task* mytask);
    bool myInIt(AVS_Task* mytask);


	//void load();
	void load(const QUrl& url);
	//void openFile();

	// �Ѳü���ͬ�������� ��ʵ��С
	void syncCropToObj();


	//����
	void setInTime();
	void setOutTime();
	void L_Play();
	void J_Play();


	bool isPlayerAvailable() const;

	void test_ocr();

	void preview_ORC_w();

	//��ݼ�
	virtual void keyPressEvent(QKeyEvent* event);

	void engineType_showUI();
    
    // 关联输出类型
    //分离类型选择
    void set_default_Models_Type_list();


public slots:
	void openFile();
	void play();

	//�ź�
signals:
	//void updatedata();
	// �ر�ǰ
	//
	void closeSig();

	void run_min();
	void setVideoPosition(qint64 time);
	void preview_ORC(qint64 time, QString* outText, QImage* outimage);

private:
	Ui::VideoQtWidgetsClass ui;

	QMediaPlayer* m_mediaPlayer = nullptr;
	QVideoSink* qv;
    QGraphicsVideoItem* m_videoItem = nullptr;
//	QGraphicsVideoItem* m_videoItem = nullptr;
//	CropItem* cropItem = nullptr;

	//MySlider* mytime_lisder = nullptr;


	//��¼��Ƶ��ǰλ�õ�
	QRect videoItemRect = QRect(0, 0, 0, 0);
	QRect cropItemRect = QRect(0, 0, 200, 200);

	//���̲߳���
	//QThread* t1 = new QThread(this);

	//��������
    AVS_Task* mytask;

	//��һ�μ���
	bool is_oneLoad = false;


	//?��д
protected:
//	//void initScene();//��ʼ������
	void resizeEvent(QResizeEvent* event);
//	void initCropRect();
	//���ڴ�С�����仯��ʱ�򣬸ú�������

	// ��д���
	//void mousePressEvent(QMouseEvent* event); ////��갴���¼� ��ʾͼ����ʼ
	//void mouseMoveEvent(QMouseEvent* event); ////����ƶ��¼� ��ϱ�����¼ͼ���Ĺ켣
	//void mouseReleaseEvent(QMouseEvent* event); ////����ͷ��¼� ��ʾ��ǰһ��ͼ������
	//virtual void paintEvent(QPaintEvent* event);





};
