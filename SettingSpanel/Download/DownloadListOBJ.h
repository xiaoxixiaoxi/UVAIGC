#pragma once

#include <QObject>
#include "DownloadTool.h"

#include <QTemporaryDir>

 //VoiceModelType

class DownloadListOBJ  : public QObject
{
	Q_OBJECT

Q_SIGNALS:
	void updataSig();

public:
	DownloadListOBJ(QObject *parent);
	~DownloadListOBJ();
	

	void init_obj(QString name, QString info, QString d_url, QString save_path);

	void isBenDiFile();

	bool startDownload();

	bool stopDownload();

	void set_savePath(QString new_path);



	
	bool isDownloading = false;
	int downloadProgress = 0;

	QString name;
	QString info;
	bool isDownloaded;
	QString d_url;
	QString save_path;

	QString TemDir;

	//给一个临时文件路径 完成后再移动过去


	DownloadTool* dT;
//dT = new DownloadTool(url, this->model_save_path);
//dT->startDownload();



};
