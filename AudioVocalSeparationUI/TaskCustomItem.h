#ifndef TASKCUSTOMITEM_H
#define TASKCUSTOMITEM_H

#include <QWidget>
#include "../Basis/TaskBasis.hpp"
#include "../AudioVocalSeparationUI/AVS_Task_QTtool.hpp"

#include <QPainter>

#include "VideoQtWidgets.h"
#include "ui_TaskCustomItem.h"

//∂®“Â“ª∏ˆÕº∆¨∂‘œÛ
class MyImage {

public:
    MyImage() {
        red = getPix(QColor(255, 59, 48));
        green = getPix(QColor(0, 220, 84));
        gray = getPix(QColor(142, 142, 146));
        yellow = getPix(QColor(255, 142, 0));
        cyan = getPix(QColor(81, 79, 189));
        exoirting = getPix(QColor(78, 201, 176));
        
        
        QImage m_image = QImage();
        m_image.load(":/new/1_1image/Resource/image/winOCRUI/1.1/music.note.tv.fill.svg");

        music_qpix = QPixmap::fromImage(m_image.scaled(112, 63));

    }
    ~MyImage() {

    }

    QPixmap getPix(Qt::GlobalColor color) {
        int w = 10;
        int h = 10;
        QImage red = QImage(w, h, QImage::Format_RGBA8888);
        red.fill(QColor(0, 0, 0, 0));
        

        QPainter painter(&red);
        painter.setPen(Qt::NoPen);
        painter.setBrush(QBrush(color));
        painter.drawEllipse(0, 0, w, h);

        QPixmap out =  QPixmap::fromImage(red);


        return out;






    }
    //QColor &color QColor(0,225,225)

    QPixmap getPix(QColor color) {
        int w = 10;
        int h = 10;
        QImage red = QImage(w, h, QImage::Format_RGBA8888);
        red.fill(QColor(0, 0, 0, 0));


        QPainter painter(&red);
        painter.setPen(Qt::NoPen);
        painter.setBrush(QBrush(color));
        painter.drawEllipse(0, 0, w, h);

        QPixmap out = QPixmap::fromImage(red);


        return out;






    }

    QPixmap red, green, gray, yellow, cyan, exoirting;
    
    
    QPixmap music_qpix;
    
    




};


extern MyImage* MYIMAGE_ICON;







namespace Ui {
class TaskCustomItem;
}

class TaskCustomItem : public QWidget
{
    Q_OBJECT
signals:
    int editpushButt(int tag);
    
    
public:
    explicit TaskCustomItem(QWidget *parent = nullptr);
    ~TaskCustomItem();
    //自定义接口
    TaskCustomItem(TaskBasis* task,int tag);

     // 使函数成为虚函数
    virtual  void relink_file();
    //À¢–¬ ˝æ› ∞— ˝æ›œ‘ æ‘⁄ΩÁ√Ê…œ
    virtual void reloadData();
    virtual void openEditWin();

    int tag = -1;
    TaskBasis* task;
    
    
    //处理工具模块
    AVS_Task_QTtool task_QTtool;
    
    
    //编辑窗口页面
    VideoQtWidgets* editW = nullptr;
    

    
    Ui::TaskCustomItem *ui;
    

private:
//    Ui::TaskCustomItem *ui;
};

#endif // TASKCUSTOMITEM_H
