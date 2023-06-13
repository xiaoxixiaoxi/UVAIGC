//
//  SR_TaskCustomItem.cpp
//  UVAIGC
//
//  Created by 林溪明 on 2023/6/8.
//
//#include "ui_TaskCustomItem.h"
//#include "ui_TaskCustomItem.h" // 包含自动生成的头文件
#include "SR_TaskCustomItem.hpp"

#include <QFileDialog>
//#include "../AudioVocalSeparation/AVS_Task.hpp"
#include "../Basis/SRTQTime.h"

SR_TaskCustomItem::SR_TaskCustomItem(TaskBasis* task, int tag, QWidget *parent)
    :TaskCustomItem(task, tag)// 实例化 Ui::TaskCustomItem
{
//    ui->setupUi(this);

    // 调用基类构造函数
    // 传递task和tag参数给基类构造函数
//    TaskCustomItem(task, tag);

    // 这里可以添加其他的逻辑代码
    reloadData();
}

SR_TaskCustomItem::~SR_TaskCustomItem(){
}

void SR_TaskCustomItem::reloadData(){
    
    if (MYIMAGE_ICON == nullptr) {
        MYIMAGE_ICON = new MyImage();
    }
    //对象惊喜转变
    // 使用 AVS_task 对象进行音频处理的具体实现
//    AVS_Task* avsTask = static_cast< AVS_Task*>(task);
    SR_Task* avsTask = static_cast< SR_Task*>(task);

//    this->ui->name_label->setText("6666");



    ui->name_label->setText(QString::fromStdString( avsTask->getName()));

    //时间长度
    SRTQTime dutime(avsTask->raw_duration);
    //大小
    QSize rawSize = avsTask->size;
    ui->durationlabel->setText(QString("%1x%2").arg(rawSize.width()).arg(rawSize.height()));
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
    
    //准备缩略图
//    m_image.scaled(112, 63)
    QPixmap thimage = QPixmap::fromImage(avsTask->ThumbnailImage);
    ui->thumbnaillabel->setPixmap(thimage);

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
        ui->state_label->setText(tr("运行中"));
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

void SR_TaskCustomItem::openEditWin(){
    
    
    emit editpushButt(this->tag);
    if (editW !=nullptr) {
//        delete editW;
//        return;
    }
    SR_Task* obj = (SR_Task*)this->task;
    //打开编辑页面
    editW = new SR_editQtWidgets() ;
    
    //绑定关闭信号
    connect(editW, &SR_editQtWidgets::closeSig, this, [=](){
        //刷新界面
        reloadData();
        //删除对象
        delete editW;
    });
    
    
    editW->set_mytask(obj);
    editW->show();
    
    

}
