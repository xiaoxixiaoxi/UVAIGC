//
//  SR_TaskCustomItem.hpp
//  UVAIGC
//
//  Created by 林溪明 on 2023/6/8.
//

#ifndef SR_TaskCustomItem_hpp
#define SR_TaskCustomItem_hpp

#include <stdio.h>

#include "../AudioVocalSeparationUI/TaskCustomItem.h"

#include "../SRUI/sr_editqtwidgets.h"



class SR_TaskCustomItem : public TaskCustomItem
{
    Q_OBJECT

    
    
public:
    explicit SR_TaskCustomItem(TaskBasis* task, int tag, QWidget* parent = nullptr);
    ~SR_TaskCustomItem();
    
    //重新写加载数据
    void reloadData() override;
    void openEditWin() override;
    
    
    
    //编辑窗口页面
    SR_editQtWidgets* editW = nullptr;

    
//protected:
   
//private:
//    Ui::TaskCustomItem *ui; // 添加 ui 作为非静态数据成员
 
    
};

#endif /* SR_TaskCustomItem_hpp */
