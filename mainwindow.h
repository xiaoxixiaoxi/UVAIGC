#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>



//个个界面。手动添加
#include "user/UserMainWindow.h"
#include "SettingSpanel/SettingSpanel_main.h"
#include "AudioVocalSeparationUI/TaskListWidgetsClass.h"
#include <QToolButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
    
    void toUserWnd();
    void toSettingWnd();
    void toAvs_Wnd();
    void toHome_Wnd();
    
    
    

private slots:
    void on_action_triggered();

    void on_action_2_triggered();
    void on_toMoreTool_toolButton_clicked();

private:
    Ui::MainWindow *ui;
    
    // 在类中定义一个成员变量用于记录当前高亮按钮的索引或标识
    int currentButtonIndex = 0;
   
    QList<QToolButton*> buttonList;
    void setCurrentButton(int buttonIndex);
    
    UserMainWindow userWnd;
    SettingSpanelQtWidgetsClass settingWnd;
    TaskListWidgetsClass avs_Wnd;
};
#endif // MAINWINDOW_H
