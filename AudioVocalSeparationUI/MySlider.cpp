#include "MySlider.h"
#include <qDebug>
#include <QPainter>
#include <QMouseEvent>


MySlider::MySlider(QWidget* parent)
	: QSlider(parent)
{





	

}

MySlider::~MySlider()
{}

void MySlider::setInTime_r(double in_time) {
    //入点时间不能大于出点
    double time = in_time;
    if (in_time > out_time_bl) {
        time = out_time_bl;
    }
    in_time_bl = time;
}
void MySlider::setOutTime_r(double out_time) {

    double time = out_time;
    if (out_time < in_time_bl) {
        time = in_time_bl;
    }

    out_time_bl = time;

}
void MySlider::setNowTime_r(double now_time) {
    now_time_bl = now_time;
}



void MySlider::mousePressEvent(QMouseEvent* event)
{
    //判断是否选中
    //计算移动距离

    QPoint pos = event->pos();

    selected_rect = 0;

    if (in_time_rect.contains(pos)) {

        selected_rect = 10;
    }
    else if (out_time_rect.contains(pos))
    {
        selected_rect = 20;
    }
    else if (now_time_rect.contains(pos))
    {
        selected_rect = 30;
    }

    //if (selected_rect != 0) {
    //
    //   // return;
    //}




    QSlider::mousePressEvent(event);
}

void MySlider::mouseReleaseEvent(QMouseEvent* event)
{

    if (selected_rect == 10) {
        emit inTimeUpdate(in_time_bl);

    }
    else if(selected_rect == 20)
    {
        emit outTimeUpdate(out_time_bl);
    }
    else if (selected_rect == 30)
    {
        emit nowTimeUpdate(now_time_bl);
    }

    

        





    QSlider::mouseReleaseEvent(event);
}

//
void MySlider::mouseMoveEvent(QMouseEvent* event)
{
    QPoint pos = event->pos();

    //选中
    if (selected_rect == 10) {
        //移动

        in_time_bl = (pos.x() - main__rect.x()) * 1.0 / main__rect.width();

        //判断是否大于出点
        if (in_time_bl>out_time_bl) {

            in_time_bl = out_time_bl;
        }


        // 判断是否出界
        if (in_time_bl < 0) {
            in_time_bl = 0;
        }
        else if (in_time_bl > 1) {
            in_time_bl = 1;
        }

        //in_time_rect.moveTo(pos.x(), 0);
        //计算出当前的比例位置

    }
    else if (selected_rect == 20) {
        //移动

        out_time_bl = (pos.x() - main__rect.x()) * 1.0 / main__rect.width();

        //判断是否大于出点
        if (out_time_bl < in_time_bl) {

            out_time_bl = in_time_bl;
        }


        // 判断是否出界
        if (out_time_bl < 0) {
            out_time_bl = 0;
        }
        else if (out_time_bl > 1) {
            out_time_bl = 1;
        }

        //in_time_rect.moveTo(pos.x(), 0);
        //计算出当前的比例位置

    }
    else if (selected_rect == 30) {
        //移动

        now_time_bl = (pos.x() - main__rect.x()) * 1.0 / main__rect.width();


        // 判断是否出界
        if (now_time_bl < 0) {
            now_time_bl = 0;
        }
        else if (now_time_bl > 1) {
            now_time_bl = 1;
        }

        //in_time_rect.moveTo(pos.x(), 0);
        //计算出当前的比例位置

    }

    update();



    if (selected_rect == 10) {
        emit inTimeUpdate(in_time_bl);

    }
    else if (selected_rect == 20)
    {
        emit outTimeUpdate(out_time_bl);
    }
    else if (selected_rect == 30)
    {
        emit nowTimeUpdate(now_time_bl);
    }


    QSlider::mouseMoveEvent(event);
}


void MySlider::paintEvent(QPaintEvent* ev)
{
    //

    Q_UNUSED(ev);







    QPainter painter(this);

    // 数据同步
    //计算比例 映射到坐标
    // 计算主线

    main__rect.setRect(10, height()-10, width()-20, 6);
   






    painter.setRenderHint(QPainter::Antialiasing, true);
   
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::red);

   


   
   





    //画出主线
    QBrush brush;   //画刷
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);

   

    //画主线 基础
    brush.setColor(QColor(150, 150, 150, 255));
    painter.setBrush(brush);
    painter.drawRoundedRect(main__rect, 2, 2);


    brush.setColor(QColor(52, 123, 226, 255));
    painter.setBrush(brush);
   
    //传入绘制区域或者直接放入QRect作为参数
   // painter.drawRoundedRect(rect,3, 3);

    
    //入点时间 
    in_time_rect.setRect(int(in_time_bl * main__rect.width()) + main__rect.x(), 6, 10, height());
    painter.drawRoundedRect(in_time_rect, 3, 3);

    //出点时间
    out_time_rect.setRect(
        int(out_time_bl * main__rect.width()) + main__rect.x() - out_time_rect.width(),
        6, 
        10,
        height());
    painter.drawRoundedRect(out_time_rect, 3, 3);


    //绘制范围区
    
    QRect range_in_out = QRect(
        in_time_rect.x(), 
        main__rect.y() - 4,
        out_time_rect.x() - in_time_rect.x() + out_time_rect.width(),
        8);
    //中心对齐
    range_in_out.moveCenter(QPoint(range_in_out.center().x(),main__rect.center().y()));
    painter.drawRoundedRect(range_in_out, 3, 3);





    //播放指针
    int cenx = int(now_time_bl * main__rect.width()) + main__rect.x();


    now_time_rect.setRect(0, 0, 10, height());

    now_time_rect.moveCenter(QPoint(cenx, now_time_rect.height() / 2));
    QPen pen(QColor(255, 0, 0));
    painter.setPen(pen);
    painter.drawLine(cenx, 0, cenx, height());
    painter.setPen(Qt::NoPen);
   // painter.set
    //绘制一个圆
    //QRect e = QRect(0, 0, 5, 5);
    brush.setColor(QColor(255, 0, 0, 255));
    painter.setBrush(brush);
    painter.drawEllipse(cenx - 5, 0, 10, 10);


    //绘制 三个控制点 绑定数据
    //QPainter painter(this);

 /*   QBrush b1("#ffffff");
    painter.setBrush(b1);
    painter.setPen(Qt::NoPen);
    QRect m_handelRect(0, 0, 5, 5);
    painter.drawRoundedRect(m_handelRect, 4, 4);*/

    //QPainter painter(this);
    //painter.setRenderHint(QPainter::Antialiasing, true);
    //QRect rect = QRect(4, 4, this->width() - 8, this->height() - 8);

    //QBrush brush;   //画刷
    //brush.setColor(QColor(52, 123, 226, 150));
    //brush.setStyle(Qt::SolidPattern);






}

//鼠标样式 没有起效果
void MySlider::hoverMoveEvent(QMouseEvent* ev) {
    QPoint pos = ev->pos();



    if (in_time_rect.contains(pos)) {

        setCursor(Qt::SizeHorCursor);
    }
    else if (out_time_rect.contains(pos))
    {
        setCursor(Qt::SizeHorCursor);
    }
    else if (now_time_rect.contains(pos))
    {
       
    }



}


