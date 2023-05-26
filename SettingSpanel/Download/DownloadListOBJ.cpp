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
	//获取文件名称 通过url   判断本地文件是否存在

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
	// 完成
	connect(dT, &DownloadTool::sigDownloadFinished, this, [=]() {
		this->isDownloading = false;

		//移动文件
		QString old_name = this->TemDir + "/" + this->name ;
		QString new_name = this->save_path + "/" + this->name;
		//判断新文件是否存在 存在删掉
		QFileInfo f_newname(new_name);
		if (f_newname.isFile() && old_name != new_name) {
			QFile::remove(new_name);
		}
		//
		bool x = QFile::rename(old_name, new_name);
		isBenDiFile();

		emit updataSig();
		});
	//绑定信号


}

//查看本地文件是否存在
void DownloadListOBJ::isBenDiFile() {

	const QUrl newUrl = QUrl::fromUserInput(this->d_url);

	QFileInfo fileinfo = QFileInfo(this->d_url);
	//文件名
	QString file_name = fileinfo.fileName();
	//遍历文件夹
	QDir dir(save_path);   //QDir的路径一定要是全路径，相对路径会有错误
	if (!dir.exists()) {

	}
	else
	{
		//取到所有的文件和文件名，去掉.和..文件夹
		dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
		dir.setSorting(QDir::DirsFirst);
		//将其转化为一个list
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
	// 因为采用下载完成移动到目标文件夹  所有 更新目录 再这边就可以

	isBenDiFile();
	emit  updataSig();


}
