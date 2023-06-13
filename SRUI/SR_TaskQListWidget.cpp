#include "SR_TaskQListWidget.h"
#include "VideoQtWidgets.h"
#include "SR_TaskCustomItem.hpp"


SR_TaskQListWidget::SR_TaskQListWidget(QWidget *parent)
    : QListWidget(parent)
{

    //测试
//    reloadData();
    
    //QAbstractItemView::ExtendedSelection: 按住ctrl多选
   //QAbstractItemView::SingleSelection: 单选， 默认
   //QAbstractItemView::MultiSelection：点击多选
   //ContiguousSelection 鼠标拖拉多选
//设置可以多选
    this->setSelectionMode(QAbstractItemView::ContiguousSelection);

    // 菜单
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &SR_TaskQListWidget::customContextMenuRequested,
        this, &SR_TaskQListWidget::show_Contextmenu1);




}

SR_TaskQListWidget::~SR_TaskQListWidget()
{

}

void SR_TaskQListWidget::reloadData()
{

    mutex.lock();

    //获取列表 添加
//    QStringList _list ;
//    _list << "sss";
//    _list << "jj";
    if (avs_Tasks->size() == 0) {
        mutex.unlock();
    	return;
    }

    //清空
    clear();

    for (size_t i = 0; i < avs_Tasks->size(); i++)
    {
        
//        AVS_Task* obj =  avs_Tasks->data()[i];
       SR_Task& obj = (*avs_Tasks)[i];
        SR_TaskCustomItem * item_DIY = new  SR_TaskCustomItem(&obj,i);

        connect(item_DIY, &SR_TaskCustomItem::editpushButt,
                this, [=](int tag) {
                    emit editpushButt(tag);
                });

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
    this->setCurrentRow(select_row);

    update();

    mutex.unlock();



}




void SR_TaskQListWidget::addData(std::vector<SR_Task>* avs_Tasks)
{
    this->avs_Tasks = avs_Tasks;
}

void SR_TaskQListWidget::setSelect_row(qint64 index) {
    this->select_row = index;
}

void SR_TaskQListWidget::del()
{

    auto items = selectedItems();

    //获取序号 然后从尾巴开是删除 就不会影响去哦阿门，
    QList<int> del_list;

    // 从尾巴往前删除
    for (size_t i = 0; i < items.count(); i++)
    {

        QListWidgetItem* item = items[items.count()-1-i];//被选中或背点击的item删除
        //    QListWidgetItem* item = ui->listWidget->takeItem(2);//指定删除

        TaskCustomItem* myitem = (TaskCustomItem*)itemWidget(item);
        int tag = myitem->tag;

        this->removeItemWidget(item);
        delete item;
        delete myitem;
        
        
        avs_Tasks->erase(avs_Tasks->begin()+tag);

//        this->taskobj->del(tag);
        del_list.append(tag);

        //判断是否数量为零 就显示拖放图标
        emit delitemSig(tag);
        //标记位置
        select_row = tag;

    }

    qDebug() << del_list;
    

    
    reloadData();





}



//更新ui数据 同步显示 全部
void SR_TaskQListWidget::updataUI() {

    //all Items
    for (size_t i = 0; i < this->count(); i++)
    {

        QListWidgetItem* _item = this->item(i);//被选中或背点击的item删除
        //    QListWidgetItem* item = ui->listWidget->takeItem(2);//指定删除

        SR_TaskCustomItem* myitem = (SR_TaskCustomItem*)itemWidget(_item);

        myitem->reloadData();
    }
    

}


//显示原文件 和输出文件
void SR_TaskQListWidget::showRawPath(int type)
{

    auto items = selectedItems();
    for (size_t i = 0; i < items.count(); i++)
    {
        int index = this->row(items[i]);
        
       SR_Task& obj =  this->avs_Tasks->data()[index];

        //定义 0为原文件 1为输出
        QStringList  pathIn_list;
        if (type == 1){
            
            for(auto & i:  obj.out_path_list){
                pathIn_list << QString::fromStdString(i);
            }
           
            
            
            
        }
        else if (type == 0)
        {
            pathIn_list << QString::fromStdString(obj.getUrl());
//            qDebug() << QString::fromStdString(obj.getUrl());

        }
        AVS_Task_QTtool::showFileFromFinder(pathIn_list);



    }





    
}

//打开编辑器 发射信号
void SR_TaskQListWidget::openEdit() {

    auto items = selectedItems();
    for (size_t i = 0; i < items.count(); i++)
    {
        int index = this->row(items[i]);
        
        QListWidgetItem* _item = this->item(index);//被选中或背点击的item删除
        //    QListWidgetItem* item = ui->listWidget->takeItem(2);//指定删除

        SR_TaskCustomItem* myitem = (SR_TaskCustomItem*)itemWidget(_item);
        myitem->openEditWin();

        
       
        
        
        emit editpushButt(index);// 只打开一个
        return;
    }

}

//重置 和仅重置状态 0重置 1仅重置状态
void SR_TaskQListWidget::resetObj(int type) {

    auto items = selectedItems();
    for (size_t i = 0; i < items.count(); i++)
    {
        int index = this->row(items[i]);
        
        
//        std::vector<AVS_Task>* avs_Tasks;
        SR_Task& obj =  this->avs_Tasks->data()[index];
//        AVS_Task& obj = avs_Tasks[index];

        //定义 0重置 1仅重置状态
        if (type == 0) {
            
            obj.reset_obj();
        }
        else if (type == 1)
        {
            obj.reset_state();
        }
        else
        {
            return;
        }


        //刷新
        updataUI();



        
    }


}

// 导出 0 srt  1为txt
void SR_TaskQListWidget::ExportObj(int type) {
    //获取保存窗口 获取数据 保存 只保存第一个

//    auto items = selectedItems();
//    QString save_text = "";
//
//    for (size_t i = 0; i < items.count(); i++)
//    {
//        int index = this->row(items[i]);
//
//        auto* obj = this->taskobj->task_list[index];
//
//
//        if (type == 0) {
//            save_text = obj->getSRT_string();
//        }
//        else if (type == 1)
//        {
//            save_text = obj->getTXT_string();
//
//        }
//        else
//        {
//            return;
//        }
//        break;
//
//
//
//
//    }
//    QString fileName;
//    if (type == 0) {
//        fileName = QFileDialog::getSaveFileName(this,
//            tr("保存srt文件"),
//            "",
//            tr("Files (*.srt)"));
//    }
//    else if (type == 1)
//    {
//        fileName = QFileDialog::getSaveFileName(this,
//            tr("保存txt文件"),
//            "",
//            tr("Files (*.txt)"));
//
//    }
//
//
//
//
//    if (!fileName.isNull())
//    {
//        //fileName是文件名
//
//        QFile file;
//        file.setFileName(fileName);
//        //只写 追加写入
//        if (file.open(QIODevice::WriteOnly | QIODevice::Text ))
//
//        {
//            QTextStream in(&file);
//            in << save_text;
//        }
//        file.close();
//
//
//
//
//
//    }
//    else
//    {
//        //点的是取消
//    }




}





//右击选中，listView中显示"start、stop、delete"
void SR_TaskQListWidget::show_Contextmenu1(const QPoint& pos)
{
    QModelIndexList list = this->selectionModel()->selectedIndexes();
    if (!list.empty())
    {
        QMenu* cmenu = new QMenu(this);
        QAction* show_rawFile = new QAction(tr("显示源文件"), this);
        QAction* show_outFile = new QAction(tr("显示输出文件"), this);
        QAction* open_edit = new QAction(tr("打开编辑器"), this);
        QAction* reset = new QAction(tr("重置"), this);
        QAction* reset_only = new QAction(tr("仅重置状态"), this);
//        QAction* out_srt = new QAction(tr("导出srt字幕"), this);
//        QAction* out_txt = new QAction(tr("导出txt文本"), this);
        QAction* delete_step = new QAction(tr("删除"), this);
        cmenu->addAction(show_rawFile);
        cmenu->addAction(show_outFile);
        cmenu->addSeparator();
        cmenu->addAction(open_edit);
        cmenu->addSeparator();
        cmenu->addAction(reset);
        cmenu->addAction(reset_only);
//        cmenu->addSeparator();
//        cmenu->addAction(out_srt);
//        cmenu->addAction(out_txt);
        cmenu->addSeparator();
        cmenu->addAction(delete_step);

        //connect(delete_step, &QAction::activate, this, &SR_TaskQListWidget::del);

        QAction* selectedAction = cmenu->exec(QCursor::pos());//在当前鼠标位置显示
        //ui->listView->selectionModel()->clear();
        if (selectedAction == delete_step) {
            this->del();
        }
        else if(selectedAction == show_rawFile)
        {
            this->showRawPath(0);

        }
        else if (selectedAction == show_outFile)
        {
            this->showRawPath(1);

        }
        else if (selectedAction == open_edit)
        {
            this->openEdit();

        }
        else if (selectedAction == reset)
        {
            this->resetObj(0);

        }
        else if (selectedAction == reset_only)
        {
            this->resetObj(1);

        }
//        else if (selectedAction == out_srt)
//        {
//            this->ExportObj(0);
//
//        }
//        else if (selectedAction == out_txt)
//        {
//            this->ExportObj(1);
//
//        }

        delete cmenu;

        delete show_rawFile;
        delete show_outFile;
        delete open_edit;
        delete reset;
        delete reset_only;
//        delete out_srt;
//        delete out_txt;
        
        delete delete_step;
    }
}

