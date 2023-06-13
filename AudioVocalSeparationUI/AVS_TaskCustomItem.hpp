//
//  AVS_TaskCustomItem.hpp
//  UVAIGC
//
//  Created by 林溪明 on 2023/6/8.
//

#ifndef AVS_TaskCustomItem_hpp
#define AVS_TaskCustomItem_hpp

#include <stdio.h>
#include "../AudioVocalSeparationUI/TaskCustomItem.h"



class AVS_TaskCustomItem : public TaskCustomItem
{
    Q_OBJECT

    
    
public:
    explicit AVS_TaskCustomItem(TaskBasis* task, int tag, QWidget* parent = nullptr);
    ~AVS_TaskCustomItem();
    
    //重新写加载数据
    void reloadData() override;
    void openEditWin() override;

    
//protected:
   
//private:
//    Ui::TaskCustomItem *ui; // 添加 ui 作为非静态数据成员
 
    
};

#endif /* AVS_TaskCustomItem_hpp */
