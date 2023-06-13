#include "DownloadListOBJ.h"
#include <QUrl>
#include <QFileInfo>
#include <QDir>

DownloadListOBJ::DownloadListOBJ(QObject *parent)
	: QObject(parent)
{}

DownloadListOBJ::~DownloadListOBJ()
{

	delete dT;
}

void DownloadListOBJ::init_obj(QString name, QString info, QString d_url, QString save_path)
{
	//��ȡ�ļ����� ͨ��url   �жϱ����ļ��Ƿ����

	this->name = name;
	this->info = info;
	this->d_url = d_url;
	this->save_path = save_path;


	

	this->isDownloaded = false;

	isBenDiFile();

	QTemporaryDir dir;
	if (dir.isValid()) {
		// dir.path() returns the unique directory path
		this->TemDir = dir.path();
		qDebug() << "ls dir:" << this->TemDir;
	}
	else
	{
		this->TemDir = this->save_path;

	}


	dT = new DownloadTool(d_url, this->TemDir);
	connect(dT, &DownloadTool::sigProgress, this, [=](qint64 bytesRead, qint64 totalBytes, qreal progress) {
		this->isDownloading = true;
		this->downloadProgress = progress * 100;
		emit updataSig();

		});
	// ���
	connect(dT, &DownloadTool::sigDownloadFinished, this, [=]() {
		this->isDownloading = false;

		//�ƶ��ļ�
		QString old_name = this->TemDir + "/" + this->name ;
		QString new_name = this->save_path + "/" + this->name;
		//�ж����ļ��Ƿ���� ����ɾ��
		QFileInfo f_newname(new_name);
		if (f_newname.isFile() && old_name != new_name) {
			QFile::remove(new_name);
		}
		//
		bool x = QFile::rename(old_name, new_name);
		isBenDiFile();

		emit updataSig();
		});
	//���ź�


}

//�鿴�����ļ��Ƿ����
void DownloadListOBJ::isBenDiFile() {

	const QUrl newUrl = QUrl::fromUserInput(this->d_url);

	QFileInfo fileinfo = QFileInfo(this->d_url);
	//�ļ���
	QString file_name = fileinfo.fileName();
	//�����ļ���
	QDir dir(save_path);   //QDir��·��һ��Ҫ��ȫ·�������·�����д���
	if (!dir.exists()) {

	}
	else
	{
		//ȡ�����е��ļ����ļ�����ȥ��.��..�ļ���
		dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
		dir.setSorting(QDir::DirsFirst);
		//����ת��Ϊһ��list
		QFileInfoList list = dir.entryInfoList();
		for (auto i : list) {
			if (i.fileName() == file_name) {
				this->isDownloaded = true;
				break;
			}


		}



	}
}

bool DownloadListOBJ::startDownload()
{
	
	
	//dT = new DownloadTool(url, this->model_save_path);
	//
 
	dT->startDownload();
	isDownloading = true;
	emit updataSig();
	return false;
}


bool DownloadListOBJ::stopDownload() {

	dT->cancelDownload();
	isDownloading = false;
	downloadProgress = 0;
	emit updataSig();
	return true;

}

void DownloadListOBJ::set_savePath(QString new_path)
{
	this->isDownloaded = false;
	this->save_path = new_path;
	// ��Ϊ������������ƶ���Ŀ���ļ���  ���� ����Ŀ¼ ����߾Ϳ���
    qDebug () << "保存路径" << this->save_path;

	isBenDiFile();
	emit  updataSig();


}
