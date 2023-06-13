#include "DownloadCustomItem.h"
#include <QMessageBox>

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
        
        //判断文件夹是否存在 不存在就提示并返回
        QString save_path = this->Download_obj->save_path;
        // 判断文件夹是否存在 没有就提示
        QDir dir(save_path);
        if (dir.exists()) {
            // 文件夹存在
            // TODO: 继续处理逻辑
        } else {
            
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText( tr("错误!输出目录不存在")+"\n"+ save_path +"\n" + tr("请重新设置保存路径"));
            QPushButton *connectButton = msgBox.addButton(tr("OK"), QMessageBox::ActionRole);
            msgBox.exec();
            
            
            
            return;
        }
        
		
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
