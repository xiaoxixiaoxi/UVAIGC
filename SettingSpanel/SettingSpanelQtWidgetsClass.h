#pragma once

#include <QWidget>
#include "ui_SettingSpanelQtWidgetsClass.h"

class SettingSpanelQtWidgetsClass : public QWidget
{
	Q_OBJECT

public:
	SettingSpanelQtWidgetsClass(QWidget *parent = nullptr);
	~SettingSpanelQtWidgetsClass();
    
    Ui::SettingSpanelQtWidgetsClassClass ui;

private:
	
};
