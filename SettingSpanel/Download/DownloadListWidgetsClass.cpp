#include "DownloadListWidgetsClass.h"
#include "DownloadCustomItem.h"

DownloadListWidgetsClass::DownloadListWidgetsClass(QWidget* parent)
	: QListWidget(parent)
{}

DownloadListWidgetsClass::~DownloadListWidgetsClass()
{}

void DownloadListWidgetsClass::addData(QVector<DownloadListOBJ*> in_list)
{
	this->task_list = in_list;



}

void DownloadListWidgetsClass::reloadData()
{

	mutex.lock();

	//获取列表 添加
	auto _list = this->task_list;

	//清空
	clear();
	for (size_t i = 0; i < _list.count(); i++)
	{
		auto obj = this->task_list[i];


		DownloadCustomItem* item_DIY = new DownloadCustomItem(obj, i);

		//connect(item_DIY, &TaskCustomItem::editpushButt,
		//	this, [=](int tag) {
		//		emit editpushButt(tag);
		//	});

		//My_sub_ok oo = (*my_sub_ok_list)[i];
		//SRTCustomItem* testsrt = new SRTCustomItem(myobj, i);
		//testsrt->show();
		QListWidgetItem* item = new QListWidgetItem;
		item->setSizeHint(item_DIY->size());
		addItem(item);
		setItemWidget(item, item_DIY);

		//每个对象绑定


	}
	//选中

	update();

	mutex.unlock();


}
