#include "TaskCustomItem.h"
//#include "ui_TaskCustomItem.h"
#include <QFileDialog>
#include "../AudioVocalSeparation/AVS_Task.hpp"
#include "../Basis/SRTQTime.h"


extern MyImage* MYIMAGE_ICON = nullptr;
//基础类

TaskCustomItem::TaskCustomItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TaskCustomItem)
{
    ui->setupUi(this);
}

TaskCustomItem::TaskCustomItem(TaskBasis* task,int tag) :
ui(new Ui::TaskCustomItem)
{
    
    ui->setupUi(this);
    this->task = task;
    this->tag = tag;
    
//    reloadData();

    //绑定编辑按钮
    connect(ui->editpushButt, &QPushButton::clicked,
            this,&TaskCustomItem::openEditWin);
    //重新链接绑定
    connect(ui->Relink_pushButton, &QPushButton::clicked, this, &TaskCustomItem::relink_file);
    
    
    
    
    
    
    
}





TaskCustomItem::~TaskCustomItem()
{
    delete ui;
}


void TaskCustomItem::relink_file() {
    QString fileName;
    //QFileDialog::getOpenFileName
    fileName = QFileDialog::getOpenFileName(this,
        tr("链接文件"),
        "",
        tr("Files (*)"));
    if (!fileName.isNull())
    {
        
        //this->my_ocr->status = unknown;
//        this->my_ocr->relinkRawFile(fileName);
        this->reloadData();
    }
    


}



//
void TaskCustomItem::reloadData(){
    qDebug()<<"继承者自己实现";
}

void TaskCustomItem::openEditWin(){
    qDebug()<<"继承者自己实现";
}






