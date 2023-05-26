#pragma once

#include <QSlider>
#include <QLabel>
// �򵥰�ʱ�������  ���� ��� ��ǰ

class MySlider  : public QSlider
{
	Q_OBJECT

//�����ź�֪ͨ
signals:
	// �޸�λ�� ���֪ͨ 
	void inTimeUpdate(double in_time_bl);
	void outTimeUpdate(double out_time_bl);
	void nowTimeUpdate(double now_time_bl);

public:
	MySlider(QWidget* parent = nullptr);
	~MySlider();

	void paintSlider(QPainter* painter);
	void paintText(QPainter* painter);
	void paintHandle(QPainter* painter);

	// ����λ��
	void setInTime_r(double in_time);
	void setOutTime_r(double out_time);
	void setNowTime_r(double now_time);


protected:
	//virtual void mousePressEvent(QMouseEvent* event);
	//virtual void mouseReleaseEvent(QMouseEvent* event);
	//virtual void mouseMoveEvent(QMouseEvent* event);
	void paintEvent(QPaintEvent* ev) override; //���ƿؼ�
	void mousePressEvent(QMouseEvent* ev) override; //��갴��
	void mouseReleaseEvent(QMouseEvent* ev) override; //����ɿ�
	void mouseMoveEvent(QMouseEvent* ev) override; //����ƶ�

	//�����ʽ
	void hoverMoveEvent(QMouseEvent* ev);


private:
	QLabel* m_displayLabel;
	double in_time_bl=0;
	double out_time_bl = 1.0;

	//��ǰʱ���
	double now_time_bl = 0.0;

	//ʶ����ʷ��¼ ���� ��ʵ��
	double log_time_bl = 0.0;
	QRect log_time_rect;


	//����
	QRect in_time_rect;
	QRect out_time_rect;
	//ʱ��ָ��
	QRect now_time_rect;


	//ʱ���� ����һ��
	QRect main__rect;

	// ѡ��ѡ��
	qint8 selected_rect = 0;

};
