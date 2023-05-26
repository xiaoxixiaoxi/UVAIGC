#pragma once

#include <QTime>
// 对时间进行扩展

class SRTQTime :public QObject ,public QTime
{
    Q_OBJECT
public:
    SRTQTime(QObject* parent = nullptr);
    ~SRTQTime();
    SRTQTime(qint64 ms);
    //字符串
   // SRTQTime(text, "hh:mm:ss,zzz")
    bool fromSRTString(QString time);

    // 获取
    qint64 getAllms();

};
