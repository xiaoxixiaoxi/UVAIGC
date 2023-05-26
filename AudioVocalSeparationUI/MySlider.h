#pragma once

#include <QSlider>
#include <QLabel>
// 简单版时间轴控制  出点 入点 当前

class MySlider  : public QSlider
{
	Q_OBJECT

//三个信号通知
signals:
	// 修改位置 完成通知 
	void inTimeUpdate(double in_time_bl);
	void outTimeUpdate(double out_time_bl);
	void nowTimeUpdate(double now_time_bl);

public:
	MySlider(QWidget* parent = nullptr);
	~MySlider();

	void paintSlider(QPainter* painter);
	void paintText(QPainter* painter);
	void paintHandle(QPainter* painter);

	// 设置位置
	void setInTime_r(double in_time);
	void setOutTime_r(double out_time);
	void setNowTime_r(double now_time);


protected:
	//virtual void mousePressEvent(QMouseEvent* event);
	//virtual void mouseReleaseEvent(QMouseEvent* event);
	//virtual void mouseMoveEvent(QMouseEvent* event);
	void paintEvent(QPaintEvent* ev) override; //绘制控件
	void mousePressEvent(QMouseEvent* ev) override; //鼠标按下
	void mouseReleaseEvent(QMouseEvent* ev) override; //鼠标松开
	void mouseMoveEvent(QMouseEvent* ev) override; //鼠标移动

	//鼠标样式
	void hoverMoveEvent(QMouseEvent* ev);


private:
	QLabel* m_displayLabel;
	double in_time_bl=0;
	double out_time_bl = 1.0;

	//当前时间点
	double now_time_bl = 0.0;

	//识别历史记录 后面 再实现
	double log_time_bl = 0.0;
	QRect log_time_rect;


	//画框
	QRect in_time_rect;
	QRect out_time_rect;
	//时间指针
	QRect now_time_rect;


	//时间线 缩短一点
	QRect main__rect;

	// 选中选项
	qint8 selected_rect = 0;

};
