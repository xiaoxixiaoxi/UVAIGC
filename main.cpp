#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include "AudioVocalSeparation/AudioVocalSeparation.hpp"
#include "SettingSpanel/SettingSpanel_main.h"
#include "AudioVocalSeparationUI/TaskListWidgetsClass.h"
//#include "AudioVocalSeparationUI/avs_tasklistui.h"


#include "user/UserModule/UserModule.hpp"
#include "user/UserMainWindow.h"
#include <QMenuBar>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    
    //检查更新
    QTimer::singleShot(2000, &a, [=]() {
        UpdateManager_to_init();
        });
    
    
    //用户对象
    if (MyUserModule_obj == nullptr){
        MyUserModule_obj = new UserModule();
        
//        qDebug()<< "是不是会员" << MyUserModule_obj->isSuperMember();
    }

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();

    for (const QString &locale : uiLanguages) {
        auto sys_language = locale;

        if (sys_language.startsWith("zh") )
        {
            if (translator.load(":/i18n/translations/UVAIGC_zh_CN.qm")) {
                a.installTranslator(&translator);
                break;
            }
        }
        else
        {
            if (translator.load(":/i18n/translations/UVAIGC_es_US.qm")) {
                a.installTranslator(&translator);
                break;

            }


        }

    }
//    qDebug()<<  QLocale::system().uiLanguages();
    
//    QLocale locale;

//    // 获取系统的语言设置
//    QLocale systemLocale = QLocale::system();
//    qDebug()<< systemLocale <<QLocale::Chinese  << QLocale::system().uiLanguages();
//    //…Ë÷√”Ô—‘
//    if (systemLocale.language() != QLocale::Chinese)
//    {
//        if (translator.load(":/i18n/translations/UVAIGC_zh_CN.qm")) {
//            a.installTranslator(&translator);
//        }
//    }
//    else
//    {
//        if (translator.load(":/i18n/translations/UVAIGC_es_US.qm")) {
//            a.installTranslator(&translator);

//        }


//    }
    
    //语言设置
//    QLocale locale;



    
    
    
    
    
    
    QIcon icon(":/new/1_1image/Resource/image/App Store-512@2x.png");

       // 设置应用程序的图标
       a.setWindowIcon(icon);

    
    
    
    MainWindow w;
    w.show();
    
//    Login lw;
//    lw.show();
    
    //测试
    AudioVocalSeparation* avs = new AudioVocalSeparation();
    
//    for (int i = 0;i < 5 ; i++ ) {
//        avs->test();
//    }

//
//    AVS_TaskListUI w;
//    w.show();

    //SHOW_SettingSpanelQtWidgetsClass_WIN();
   
//    delete avs;


//    LoginWindow lw =  LoginWindow(&userModule);
//    lw.show();
//

//    UserMainWindow  uW;
//    uW.show();
//    uW.toLoginWindow();
//    uW.getUserInfo();






    return a.exec();
}
