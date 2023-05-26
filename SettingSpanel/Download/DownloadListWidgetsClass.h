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

	//加载数据
	void addData(QVector<DownloadListOBJ*> in_list);

	//刷新数据
	void reloadData();

	void updataUI();


	//外部指针给进来  内部直接对对象进行操作
	QVector<DownloadListOBJ*> task_list;

	QMutex mutex;


};
