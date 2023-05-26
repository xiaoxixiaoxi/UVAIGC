#pragma once

#include <QObject>        // QObject����Qt����ģ�͵ĺ���
#include <QUrl>           // QUrl���ṩ��ʹ��URL�ı�ݽӿ�
#include <QFile>          // QFile�����ڶ��ļ����ж�д����
#include <QDir>           // QDir�����ڲ���·�������ײ��ļ�ϵͳ
#include <QPointer>       // QPointerָ�����õĶ�������ʱ��,���Զ�ָ��NULL,���ָ����������
#include <QApplication>   // �˴����ڻ�ȡ��ǰ�������·��

#include <QNetworkReply>  // QNetworkReply���װ��ʹ��QNetworkAccessManager������������صĻظ���Ϣ��
#include <QNetworkAccessManager>  // QNetworkAccessManager��ΪӦ���ṩ������������ͽ��մ𸴵�API�ӿ�
#include <memory>         // ʹ��std::unique_ptr��Ҫ������ͷ�ļ�

//#define DOWNLOAD_DEBUG    // �Ƿ��ӡ���

class DownloadTool : public QObject  // �̳�QObject
{
    Q_OBJECT              // ����˺꣬����ʹ��QT�е�signal��slot����

public:
    // ���캯������:  1)http�ļ�������url  2)�����·��
    explicit DownloadTool(const QString& downloadUrl, const QString& savePath, QObject* parent = nullptr);
    ~DownloadTool();

    void setSavePath(QString newpath);

    void startDownload();  // ��ʼ�����ļ�
    void cancelDownload(); // ȡ�������ļ�

Q_SIGNALS:
    void sigProgress(qint64 bytesRead, qint64 totalBytes, qreal progress);  // ���ؽ����ź�
    void sigDownloadFinished();  // ��������ź�

private Q_SLOTS:
    void httpFinished();    // QNetworkReply::finished��Ӧ�Ĳۺ���
    void httpReadyRead();   // QIODevice::readyRead��Ӧ�Ĳۺ���

    void networkReplyProgress(qint64 bytesRead, qint64 totalBytes);  // QNetworkReply::downloadProgress��Ӧ�Ĳۺ���

private:
    void startRequest(const QUrl& requestedUrl);
    std::unique_ptr<QFile> openFileForWrite(const QString& fileName);

private:
    QString m_downloadUrl;  // ���湹��ʱ���������url
    QString m_savePath;     // ���湹��ʱ����ı���·��

    const QString defaultFileName = "tmp";  // Ĭ�����ص�tmp�ļ���

    QUrl url;
    QNetworkAccessManager qnam;
    QPointer<QNetworkReply> reply;
    std::unique_ptr<QFile> file;
    bool httpRequestAborted;
};