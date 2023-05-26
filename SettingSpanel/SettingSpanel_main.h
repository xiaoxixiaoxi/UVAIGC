#pragma once

#include <QObject>
#include "SettingSpanelQtWidgetsClass.h"

#include "../AudioVocalSeparation/AVS_MDXNet_ONNX_Strategy.hpp"

#include "UpdateManager.hpp"

#include "../user/UserModule/UserModule.hpp"

//����һ��ȫ�ֶ���
extern SettingSpanelQtWidgetsClass* SETTINGSpanewin_OBJ;

//����һ��ȫ�ֵ��ú���
void SHOW_SettingSpanelQtWidgetsClass_WIN(int tag=0);


void SHOW_SettingSpanelQtWidgetsClass_AVS_WIN(int tag,AVS_MDXNet_ONNX_Strategy * avs_strategy);



//检查更新
extern UpdateManager* MyUpdateManager_obj;
void UpdateManager_to_init();


//一个用户对象 是不是会员判断
extern UserModule* MyUserModule_obj;
