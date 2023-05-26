#pragma once

#include <QObject>
#include <QListWidget>
#include "DownloadListOBJ.h"
#include <QMutex>

class DownloadListWidgetsClass  : public QListWidget
{
	Q_OBJECT

public:
	DownloadListWidgetsClass(QWidget *parent);
	~DownloadListWidgetsClass();

	//��������
	void addData(QVector<DownloadListOBJ*> in_list);

	//ˢ������
	void reloadData();

	void updataUI();


	//�ⲿָ�������  �ڲ�ֱ�ӶԶ�����в���
	QVector<DownloadListOBJ*> task_list;

	QMutex mutex;


};
