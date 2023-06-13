#include "general_settings_widgets.h"
#include "ui_general_settings_widgets.h"
#include <QMessageBox>

General_settings_Widgets::General_settings_Widgets(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::General_settings_Widgets)
{
    ui->setupUi(this);
}

General_settings_Widgets::~General_settings_Widgets()
{
    delete ui;
}

void General_settings_Widgets::on_restore_pushButton_clicked()
{
    //清空本地保存的配置信息
    // 获取所有设置项的键
    // 清空所有储存的内容
    settings.clear();
    
    QMessageBox msgBox;
    msgBox.setText(tr("重置成功!请重启软件"));
    msgBox.exec();
    // 
//    QStringList keys = settings.allKeys();
//
//    // 遍历所有键并输出对应的键和值
//    for (const QString& key : keys) {
//        QVariant value = settings.value(key);
//        qDebug() << "Key:" << key << "Value:" << value;
//    }


}

