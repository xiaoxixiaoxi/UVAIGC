#pragma once

#include <QTime>
// ��ʱ�������չ

class SRTQTime :public QObject ,public QTime
{
    Q_OBJECT
public:
    SRTQTime(QObject* parent = nullptr);
    ~SRTQTime();
    SRTQTime(qint64 ms);
    //�ַ���
   // SRTQTime(text, "hh:mm:ss,zzz")
    bool fromSRTString(QString time);

    // ��ȡ
    qint64 getAllms();

};
