#ifndef TASKQLISTWIDGET_H
#define TASKQLISTWIDGET_H

#include <QListWidget>
#include <QWidget>
#include <QMutex>
#include "TaskCustomItem.h"
#include "../AudioVocalSeparation/AVS_Task.hpp"




#include "TaskCustomItem.h"
#include <QMenu>

#include <QProcess>
#include <QFileInfo>
#include <QDir>


class TaskQListWidget  : public QListWidget
{
    Q_OBJECT
    //∂®“Â“ª∏ˆ–≈∫≈
signals:
int editpushButt(int tag);
//”–…æ≥˝¥•∑¢
void delitemSig(int tag);
    
    
public:
    TaskQListWidget(QWidget *parent);
    ~TaskQListWidget();


    QMutex mutex;
    //µ±«∞∏ﬂ¡¡—°œÓ
    int select_row = 0;
    //对象列表
    std::vector<AVS_Task>* avs_Tasks;
    
    
    //∏¯Œ“ ˝æ› Œ“’π æ≥ˆ¿¥ ”––ﬁ∏ƒÕ¨≤Ωª· ˝æ›
    void addData(std::vector<AVS_Task>* avs_Tasks);
    //…Ë÷√—°÷–±Íº«
    void setSelect_row(qint64 index);
    //…æ≥˝ ˝æ›
    void del();

    //À¢–¬ ˝æ›
    void reloadData();

    void updataUI();

    void showRawPath(int type);
    //¥Úø™±‡º≠∆˜ ∑¢…‰–≈∫≈
    void openEdit();
    //÷ÿ÷√ ∫ÕΩˆ÷ÿ÷√◊¥Ã¨ 0÷ÿ÷√ 1Ωˆ÷ÿ÷√◊¥Ã¨
    void resetObj(int type);
    // µº≥ˆ 0 srt  1Œ™txt
    void ExportObj(int type);

    void show_Contextmenu1(const QPoint& pos);
    
    //开始
    //暂停
    




};

#endif // TASKQLISTWIDGET_H
