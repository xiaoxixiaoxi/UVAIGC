#ifndef GENERAL_SETTINGS_WIDGETS_H
#define GENERAL_SETTINGS_WIDGETS_H

#include <QWidget>

#include"Settings_info.hpp"
#include<QSettings>

namespace Ui {
class General_settings_Widgets;
}

class General_settings_Widgets : public QWidget
{
    Q_OBJECT

public:
    explicit General_settings_Widgets(QWidget *parent = nullptr);
    ~General_settings_Widgets();

private slots:
    void on_restore_pushButton_clicked();

private:
    Ui::General_settings_Widgets *ui;
     QSettings settings = QSettings(ORGANIZATION_Name, APPLICATION_Name);
};


#endif // GENERAL_SETTINGS_WIDGETS_H
