//
//  AVS_TaskCustomItem.cpp
//  UVAIGC
//
//  Created by 林溪明 on 2023/6/8.
//

#include "AVS_TaskCustomItem.hpp"
#include <QFileDialog>
#include "../AudioVocalSeparation/AVS_Task.hpp"
#include "../Basis/SRTQTime.h"

AVS_TaskCustomItem::AVS_TaskCustomItem(TaskBasis* task, int tag, QWidget *parent)
    :TaskCustomItem(task, tag)// 实例化 Ui::TaskCustomItem
{
//    ui->setupUi(this);

    // 调用基类构造函数
    // 传递task和tag参数给基类构造函数
//    TaskCustomItem(task, tag);

    // 这里可以添加其他的逻辑代码
    reloadData();
}
AVS_TaskCustomItem::~AVS_TaskCustomItem(){
    
}
void AVS_TaskCustomItem::reloadData() {

    if (MYIMAGE_ICON == nullptr) {
        MYIMAGE_ICON = new MyImage();
    }
    //对象惊喜转变
    // 使用 AVS_task 对象进行音频处理的具体实现
//    AVS_Task* avsTask = static_cast< AVS_Task*>(task);
    AVS_Task* avsTask = static_cast< AVS_Task*>(task);



    ui->name_label->setText(QString::fromStdString( avsTask->getName()));

    //时间长度
    SRTQTime dutime(avsTask->raw_duration);
    ui->durationlabel->setText(dutime.toString("hh:mm:ss"));
    ui->info_label->setText(task_QTtool.getShowInfo(avsTask) );
    //ui->durationlabel->setText(this->my_ocr->raw_duration)
    //QPixmap pixmap = this->my_ocr->getThumbnail();
    //pixmap.scaled(ui->thumbnaillabel->size(), Qt::KeepAspectRatio);
    //ui->thumbnaillabel->setScaledContents(true);

    //重新链接
    ui->Relink_pushButton->hide();
//    if (this->my_ocr->is_missing) {
//        ui->state_label->setText(tr("文件丢失"));
//        ui->state_image_label->setPixmap(MYIMAGE_ICON->red);
//        ui->Relink_pushButton->show();
//    }

    ui->thumbnaillabel->setPixmap(MYIMAGE_ICON->music_qpix);

    ui->progressBar->hide();
    //状态判断
    switch (avsTask->getState())
    {
    case AVS_Task::NotStarted:
        ui->state_label->setText(tr("未知状态"));
        //cyan
        ui->state_image_label->setPixmap(MYIMAGE_ICON->cyan);
        break;
        case AVS_Task::Waiting:
        ui->state_label->setText(tr("读取"));
        ui->state_image_label->setPixmap(MYIMAGE_ICON->gray);
        break;
        case AVS_Task::Running:
        ui->progressBar->show();
        ui->progressBar->setValue(int(avsTask->getProgress() * 100));
        ui->state_label->setText(tr("分离中"));
        ui->state_image_label->setPixmap(MYIMAGE_ICON->exoirting);//exoirting
        break;
        case AVS_Task::Finished:

        ui->state_label->setText(tr("完成"));
        ui->state_image_label->setPixmap(MYIMAGE_ICON->green);
        break;
        case AVS_Task::Error:
        ui->state_label->setText(tr("错误"));
        ui->state_image_label->setPixmap(MYIMAGE_ICON->red);
            ui->state_label->setToolTip(QString::fromStdString(avsTask->getOutInfo()));
        break;
        case AVS_Task::Cancelled:
        ui->state_label->setText(tr("取消"));
        ui->state_image_label->setPixmap(MYIMAGE_ICON->yellow);
        break;
    default:
        break;
    }
    



}


//打开编辑器
void AVS_TaskCustomItem::openEditWin(){
    
    emit editpushButt(this->tag);
    if (editW !=nullptr) {
//        delete editW;
//        return;
    }
    AVS_Task* obj = (AVS_Task*)this->task;
    //打开编辑页面
    editW = new VideoQtWidgets() ;
    
    //绑定关闭信号
    connect(editW, &VideoQtWidgets::closeSig, this, [=](){
        //刷新界面
        reloadData();
        //删除对象
        delete editW;
    });
    
    
    editW->set_mytask(obj);
    editW->show();

    



    
}

