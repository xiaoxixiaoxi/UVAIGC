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
    //���ʱ�䲻�ܴ��ڳ���
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
    //�ж��Ƿ�ѡ��
    //�����ƶ�����

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

    //ѡ��
    if (selected_rect == 10) {
        //�ƶ�

        in_time_bl = (pos.x() - main__rect.x()) * 1.0 / main__rect.width();

        //�ж��Ƿ���ڳ���
        if (in_time_bl>out_time_bl) {

            in_time_bl = out_time_bl;
        }


        // �ж��Ƿ����
        if (in_time_bl < 0) {
            in_time_bl = 0;
        }
        else if (in_time_bl > 1) {
            in_time_bl = 1;
        }

        //in_time_rect.moveTo(pos.x(), 0);
        //�������ǰ�ı���λ��

    }
    else if (selected_rect == 20) {
        //�ƶ�

        out_time_bl = (pos.x() - main__rect.x()) * 1.0 / main__rect.width();

        //�ж��Ƿ���ڳ���
        if (out_time_bl < in_time_bl) {

            out_time_bl = in_time_bl;
        }


        // �ж��Ƿ����
        if (out_time_bl < 0) {
            out_time_bl = 0;
        }
        else if (out_time_bl > 1) {
            out_time_bl = 1;
        }

        //in_time_rect.moveTo(pos.x(), 0);
        //�������ǰ�ı���λ��

    }
    else if (selected_rect == 30) {
        //�ƶ�

        now_time_bl = (pos.x() - main__rect.x()) * 1.0 / main__rect.width();


        // �ж��Ƿ����
        if (now_time_bl < 0) {
            now_time_bl = 0;
        }
        else if (now_time_bl > 1) {
            now_time_bl = 1;
        }

        //in_time_rect.moveTo(pos.x(), 0);
        //�������ǰ�ı���λ��

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

    // ����ͬ��
    //������� ӳ�䵽����
    // ��������

    main__rect.setRect(10, height()-10, width()-20, 6);
   






    painter.setRenderHint(QPainter::Antialiasing, true);
   
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::red);

   


   
   





    //��������
    QBrush brush;   //��ˢ
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);

   

    //������ ����
    brush.setColor(QColor(150, 150, 150, 255));
    painter.setBrush(brush);
    painter.drawRoundedRect(main__rect, 2, 2);


    brush.setColor(QColor(52, 123, 226, 255));
    painter.setBrush(brush);
   
    //��������������ֱ�ӷ���QRect��Ϊ����
   // painter.drawRoundedRect(rect,3, 3);

    
    //���ʱ�� 
    in_time_rect.setRect(int(in_time_bl * main__rect.width()) + main__rect.x(), 6, 10, height());
    painter.drawRoundedRect(in_time_rect, 3, 3);

    //����ʱ��
    out_time_rect.setRect(
        int(out_time_bl * main__rect.width()) + main__rect.x() - out_time_rect.width(),
        6, 
        10,
        height());
    painter.drawRoundedRect(out_time_rect, 3, 3);


    //���Ʒ�Χ��
    
    QRect range_in_out = QRect(
        in_time_rect.x(), 
        main__rect.y() - 4,
        out_time_rect.x() - in_time_rect.x() + out_time_rect.width(),
        8);
    //���Ķ���
    range_in_out.moveCenter(QPoint(range_in_out.center().x(),main__rect.center().y()));
    painter.drawRoundedRect(range_in_out, 3, 3);





    //����ָ��
    int cenx = int(now_time_bl * main__rect.width()) + main__rect.x();


    now_time_rect.setRect(0, 0, 10, height());

    now_time_rect.moveCenter(QPoint(cenx, now_time_rect.height() / 2));
    QPen pen(QColor(255, 0, 0));
    painter.setPen(pen);
    painter.drawLine(cenx, 0, cenx, height());
    painter.setPen(Qt::NoPen);
   // painter.set
    //����һ��Բ
    //QRect e = QRect(0, 0, 5, 5);
    brush.setColor(QColor(255, 0, 0, 255));
    painter.setBrush(brush);
    painter.drawEllipse(cenx - 5, 0, 10, 10);


    //���� �������Ƶ� ������
    //QPainter painter(this);

 /*   QBrush b1("#ffffff");
    painter.setBrush(b1);
    painter.setPen(Qt::NoPen);
    QRect m_handelRect(0, 0, 5, 5);
    painter.drawRoundedRect(m_handelRect, 4, 4);*/

    //QPainter painter(this);
    //painter.setRenderHint(QPainter::Antialiasing, true);
    //QRect rect = QRect(4, 4, this->width() - 8, this->height() - 8);

    //QBrush brush;   //��ˢ
    //brush.setColor(QColor(52, 123, 226, 150));
    //brush.setStyle(Qt::SolidPattern);






}

//�����ʽ û����Ч��
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


