#include "SRTQTime.h"

SRTQTime::SRTQTime(QObject* parent)
    :QTime()
{
    

}
//SRTQTime(qint64 ms)
SRTQTime::SRTQTime(qint64 ms)
    :QTime()

{
    qint64 ss = ms / 1000;

    qint8 h = ss / 3600;
    qint8 m = (ss % 3600) / 60;
    qint8 s = (ss - h * 3600 - m * 60);
    qint64 _ms = ms % 1000;

    //qDebug() << h << m<< s<< _ms;


    this->setHMS(h,m,s,_ms);


}

//返回毫秒数
qint64 SRTQTime::getAllms() {
    //先转换成秒 再x1000
   qint64 all_s = this->hour() * 3600 + this->minute() * 60 + this->second();
   return (all_s * 1000)+ this->msec();
}

bool SRTQTime::fromSRTString(QString time) {
    // 替换字符串  解析 返回
    QString time_ok = time;
    QTime newTime = this->fromString(time_ok, "hh:mm:ss,zzz");
    if (newTime.hour() == -1) {
        return false;
    }
    this->setHMS(newTime.hour(), newTime.minute(), newTime.second(), newTime.msec());

    return true;

    



}


SRTQTime::~SRTQTime()
{

}
