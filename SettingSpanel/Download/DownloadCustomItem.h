#pragma once

#include <QWidget>
#include "ui_DownloadCustomItem.h"
#include "DownloadListOBJ.h"


class DownloadCustomItem : public QWidget
{
	Q_OBJECT

public:
	DownloadCustomItem(QWidget *parent = nullptr);
	~DownloadCustomItem();
	DownloadCustomItem(DownloadListOBJ* Download_obj, int tag);

	void upDataUi();


	void reloadData();
	int tag;

private:
	Ui::DownloadCustomItemClass ui;
	//主体对象
	DownloadListOBJ* Download_obj = nullptr;
};
