#include "SettingSpanel_main.h"
#include <QMessageBox>
#include <QDesktopServices>



//����һ��ȫ�ֶ���
extern SettingSpanelQtWidgetsClass* SETTINGSpanewin_OBJ = nullptr;

void SHOW_SettingSpanelQtWidgetsClass_WIN(int tag)
{
	if (SETTINGSpanewin_OBJ == nullptr) {
		SETTINGSpanewin_OBJ = new SettingSpanelQtWidgetsClass();
		
	}

	SETTINGSpanewin_OBJ->show();
	

}



void SHOW_SettingSpanelQtWidgetsClass_AVS_WIN(int tag,AVS_MDXNet_ONNX_Strategy * avs_strategy){
    
    SHOW_SettingSpanelQtWidgetsClass_WIN(tag);
    
//    AVS_SettingQtWidgetsClass* avs_obj = SETTINGSpanewin_OBJ->ui.avs_setup_widget;
    
    SETTINGSpanewin_OBJ->ui.avs_setup_widget->set_Strategy(avs_strategy);
    
    
    
}




// 检查更新对象
extern UpdateManager* MyUpdateManager_obj = nullptr;
// 更新软件窗口
void UpdateManager_to_init() {
    //myNSAlert("有新版","");
    //

    //获取本地版本
    double val = 0;
    //QString v = VersionUpdateWin::GetVersion();
    QString v = QCoreApplication::applicationVersion();// 获取软件版本号
    //1.0.0.0
//    QStringList v_list = v.split(".");
    
//    if (v_list.count() >= 4) {
//        val = v_list[0].toInt() + v_list[1].toInt() / 10.0  + v_list[2].toInt() / 100.0 + v_list[3].toInt() / 1000.0;
//    }
    val = v.toDouble();
    
    //qDebug() << v << val;

    MyUpdateManager_obj = new UpdateManager(val);

    if (MyUpdateManager_obj != nullptr) {

        if (MyUpdateManager_obj->my_init_license()) {

            //有新版本

            QString text = QObject::tr("检测到新版本");


            std::string str_bd = MyUpdateManager_obj->doubleToString(MyUpdateManager_obj->appStoreVersion_bd);
            std::string str_zx = MyUpdateManager_obj->doubleToString(MyUpdateManager_obj->zxVersion);

            QString all_str = text +"\n\n"+ QObject::tr("本地版本") + QString(" : ") + QString::fromStdString(str_bd) + "\n" + QObject::tr("最新版本") + " : " + QString::fromStdString(str_zx) + "\n\n" + QObject::tr("更新内容") + ": " + QString::fromStdString(MyUpdateManager_obj->new_content);





            QMessageBox* myBox = new QMessageBox();

            myBox->setWindowTitle(text);

            myBox->setIcon(QMessageBox::NoIcon);
            myBox->setText(all_str);
            //myBox->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel | QMessageBox::);
            //myBox->setDefaultButton(QMessageBox::Ok);
            myBox->addButton(QMessageBox::Ok);
            myBox->addButton(QMessageBox::Save);
            myBox->addButton(QMessageBox::Cancel);
            myBox->setButtonText(QMessageBox::Ok, QObject::tr("去更新"));
            myBox->setButtonText(QMessageBox::Save, QObject::tr("跳过本版本更新"));
            myBox->setButtonText(QMessageBox::Cancel, QObject::tr("下次再说"));
            int ret = myBox->exec();
            switch (ret)
            {
            case QMessageBox::Save:
                // return nil; 跳过本版本
                   // 把版本信息储存起来
                MyUpdateManager_obj->save_skip_version();
                break;
            case QMessageBox::Ok:
            {
                std::string url = MyUpdateManager_obj->to_url_license;
                
                QDesktopServices::openUrl(QString::fromStdString(url));

//                std::wstring temp = std::wstring(url.begin(), url.end());
//
//                // Applying c_str() method on temp
//                LPCWSTR url_ = temp.c_str();
//                //LPCWSTR
//                ShellExecute(NULL, _T("open"), _T("explorer.exe"), url_, NULL, SW_SHOW);

            }
                break;
            case QMessageBox::Cancel:
                qDebug() << "cancel!";
                break;
            }

            



        }//没有新版本
    
    }




}

//一个用户对象 是不是会员判断
extern UserModule* MyUserModule_obj  = nullptr;


