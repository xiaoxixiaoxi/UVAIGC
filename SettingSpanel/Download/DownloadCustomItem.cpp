#include "DownloadCustomItem.h"

DownloadCustomItem::DownloadCustomItem(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

DownloadCustomItem::~DownloadCustomItem()
{}

DownloadCustomItem::DownloadCustomItem(DownloadListOBJ * Download_obj, int tag)
{
	ui.setupUi(this);
	this->Download_obj = Download_obj;
	connect(this->Download_obj, &DownloadListOBJ::updataSig, this, &DownloadCustomItem::upDataUi);
	upDataUi();

	//下载按钮绑定
	connect(ui.pushButton, &QPushButton::clicked, this, [=]() {
		
		if (Download_obj->isDownloading  == true) {
			this->Download_obj->stopDownload();
		}
		else
		{
			this->Download_obj->startDownload();
			ui.progressBar->show();
		}
		
		
		
		});






}


void DownloadCustomItem::upDataUi(){
	ui.name_label->setText(Download_obj->name);
	ui.infolabel->setText(Download_obj->info);

	if (Download_obj->downloadProgress > 0) {

		ui.pushButton->setIcon(QIcon(":/new/1_1image/Resource/image/winOCRUI/1.1/icloud.slash.svg"));
		ui.pushButton->setText(tr("停止"));
	}
	else
	{
		ui.pushButton->setIcon(QIcon(":/new/1_1image/Resource/image/winOCRUI/1.1/icloud.and.arrow.down.fill.svg"));
		ui.pushButton->setText(tr("下载"));

	}
	ui.progressBar->hide();
	
	

	if (Download_obj->isDownloaded) {
		ui.imagelabel->setPixmap(QPixmap(":/new/1_1image/Resource/image/winOCRUI/1.1/arrow.down.circle.fill.svg"));
		ui.pushButton->hide();

	}
	else
	{
		ui.imagelabel->setPixmap(QPixmap(":/new/1_1image/Resource/image/winOCRUI/1.1/arrow.down.circle.svg"));
		ui.pushButton->show();


		if (!Download_obj->isDownloading) {
			ui.progressBar->hide();
		}
		else
		{
			ui.progressBar->show();
			ui.progressBar->setValue(Download_obj->downloadProgress);
			
		}
		

	}
	




}
