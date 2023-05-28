#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QSvgRenderer>
#include <QPainter>
#include <QToolButton>
#include <QDesktopServices>
#include "SettingSpanel/Settings_info.hpp"
//选择状态
QString styleSheet01 = "QToolButton {"
"    border: none;"
"    border-radius: 2px;"  // 调整圆角半径
"    text-align: center;"  // 文本左对齐
"    padding-left: 4px;"  // 调整文本的左边距
//"    padding-right: 4px;"
//                          "    background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #CCCCCC, stop:1 transparent);"  // 设置渐变背景色
"}"
"QToolButton:hover {"
"    background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #0099cc, stop:0.03 transparent);"  // 高亮背景色
"}"
"QToolButton:pressed {"
"    background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #0099cc, stop:0.08 transparent);"  // 按下背景色
"}";


//非选中状态
QString styleSheet02 = "QToolButton {"
"    border: none;"
"    border-radius: 2px;"  // 调整圆角半径
"    text-align: center;"  // 文本左对齐
"    padding-left: 4px;"  // 调整文本的左边距
//"    padding-right: 4px;"
"    background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #0099cc, stop:0.02 transparent);"  // 设置渐变背景色
"}"
"QToolButton:hover {"
"    background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #0099cc, stop:0.03 transparent);"  // 高亮背景色
"}"
"QToolButton:pressed {"
"    background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #0099cc, stop:0.08 transparent);"  // 按下背景色
"}";


void setSvgIcon(QToolButton* button, const QString& filePath, const QColor& color, const QSize& size)
{
    button->setToolTip(button->text());
//    button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    button->setIconSize(size);
    button->setFixedSize(45, 30);
    button->setStyleSheet(styleSheet01);
    
    QSvgRenderer renderer(filePath);
    QSize iconSize = button->iconSize();

    QImage image(iconSize, QImage::Format_ARGB32);
    image.fill(Qt::transparent);

    QPainter painter(&image);
    renderer.render(&painter);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.fillRect(image.rect(), color);
    painter.end();



    button->setIcon(QPixmap::fromImage(image));
//
//    button->setText("音频分离");
   
    
    
    
   
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("");
    
    QColor iconColor = QColor("#0099cc");  // 设置图标颜色
    QSize iconSize(25, 25);  // 设置图标大小
    //设置图标
    setSvgIcon(ui->avs_toolButton, ":/new/1_1image/Resource/image/winOCRUI/1.1/waveform.svg", iconColor, iconSize);
    setSvgIcon(ui->home_toolButton, ":/new/1_1image/Resource/image/UVAIGC/house.svg", iconColor, iconSize);
    setSvgIcon(ui->setting_toolButton, ":/new/1_1image/Resource/image/UVAIGC/gearshape.svg", iconColor, iconSize);
    setSvgIcon(ui->user_toolButton, ":/new/1_1image/Resource/image/UVAIGC/person.crop.circle.fill.svg", iconColor, iconSize);
    setSvgIcon(ui->user_toolButton, ":/new/1_1image/Resource/image/UVAIGC/person.crop.circle.fill.svg", iconColor, iconSize);
    setSvgIcon(ui->toMoreTool_toolButton, ":/new/prefix1/Resource/image/winOCRUI/link.svg", iconColor, iconSize*0.7);
    
    ui->home_toolButton->setProperty("current", 0);
    ui->avs_toolButton->setProperty("current", 1);
    ui->setting_toolButton->setProperty("current", 2);
    ui->user_toolButton->setProperty("current", 3);
    
    ui->toMoreTool_toolButton->setProperty("current", 100);
    
    buttonList = { ui->home_toolButton,ui->avs_toolButton,ui->setting_toolButton,ui->user_toolButton};
    
    
    
    ui->stackedWidget->addWidget(&avs_Wnd);
    ui->stackedWidget->addWidget(&settingWnd);
    ui->stackedWidget->addWidget(&userWnd);
    
   
    
    //绑定按钮
    connect(ui->user_toolButton, &QToolButton::clicked, this, &MainWindow::toUserWnd);
    //
    connect(ui->setting_toolButton, &QToolButton::clicked, this, &MainWindow::toSettingWnd);
    connect(ui->avs_toolButton, &QToolButton::clicked, this, &MainWindow::toAvs_Wnd);
    
    connect(ui->home_toolButton, &QToolButton::clicked, this, &MainWindow::toHome_Wnd);
    
    
    //绑定avs设置信号 &avs_mdxnet_onnx_Strategy
    connect(&avs_Wnd,&TaskListWidgetsClass::toSettingSpanelQtWidgetsClass_AVS_WIN_Sig,this,[=](AVS_MDXNet_ONNX_Strategy * avs_strategy){
        
//        TODO: 添加指定音频分离面板
        settingWnd.ui.avs_setup_widget->set_Strategy(avs_strategy);
        toSettingWnd();
    });
    
    //绑定去用户面板
    connect(&avs_Wnd,&TaskListWidgetsClass::toProwin_Sig,this,[=](){
        toUserWnd();
    });


    
    
    
    
    
    // 获取软件版本号
    QString version = QCoreApplication::applicationVersion();
//    qDebug() << "version :" << version ;
    ui->version_label->setText(version);
    
//    QMenuBar* menuBar = new QMenuBar(this);
//    setMenuBar(menuBar);
//
//    QMenu* appMenu = menuBar->addMenu(tr("Application"));
//    appMenu->addAction(tr("QuitAA"), this, &MainWindow::close);
//不能删除
    QMenuBar *menuBar = new QMenuBar(0);


    //广告语样式调整
    QPalette palette =   qApp->palette();

    if(palette.color(QPalette::Text).lightnessF()<0.5){
        //深色
        qDebug()<<"深色";

         ui->label_4->setStyleSheet("#label_4 {background-image: url(:/new/1_1image/Resource/image/bj.png); background-repeat: no-repeat; background-position: center;border-radius: 10px;box-shadow: 0px 2px 5px rgba(0, 0, 0, 0.4);letter-spacing: 5px;color:#ffffff;}");

    }else {
        //浅色
         qDebug()<<"浅色";
        ui->label_4->setStyleSheet("#label_4 {background-image: url(:/new/1_1image/Resource/image/bj.png); background-repeat: no-repeat; background-position: center;border-radius: 10px;box-shadow: 0px 2px 5px rgba(0, 0, 0, 0.4);letter-spacing: 5px;}");
    }



    
    
    
    
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::setCurrentButton(int buttonIndex){
    // 更新当前高亮按钮的索引或标识
       currentButtonIndex = buttonIndex;

       // 遍历所有按钮，根据索引或标识设置样式
       for (int i = 0; i < buttonList.size(); ++i) {
           QToolButton* button = buttonList[i];
           int current = button->property("current").toInt();
           if (current == currentButtonIndex) {
               // 设置高亮样式
               button->setStyleSheet(styleSheet02);
               
               this->setWindowTitle(button->text());
               
               
           } else {
               // 设置普通样式
               button->setStyleSheet(styleSheet01);
           }
       }

    
}



void MainWindow::toUserWnd(){
    ui->stackedWidget->setCurrentWidget(&userWnd);
    setCurrentButton(3);
    
}
void MainWindow::toSettingWnd(){
    ui->stackedWidget->setCurrentWidget(&settingWnd);
    setCurrentButton(2);
    
    
}
void MainWindow::toAvs_Wnd(){
    ui->stackedWidget->setCurrentWidget(&avs_Wnd);
    setCurrentButton(1);
    
    
}
void MainWindow::toHome_Wnd(){
    
    ui->stackedWidget->setCurrentWidget(ui->home_Whm);
    setCurrentButton(0);
    
    
}


void MainWindow::on_action_triggered()
{
    qDebug()<<"点击";
    QDesktopServices::openUrl(QUrl(QString::fromStdString(TO_URL_ABOUTUS)));
}


void MainWindow::on_action_2_triggered()
{
    qDebug()<<"点击";
    QDesktopServices::openUrl(QUrl(QString::fromStdString(TO_URL_license)));
}


void MainWindow::on_toMoreTool_toolButton_clicked()
{
    QDesktopServices::openUrl(QUrl(QString::fromStdString(TO_URL_MoreAPP)));
}

