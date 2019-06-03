#ifndef HTTPWORKER_H
#define HTTPWORKER_H


#include <QUrl>
#include <QObject>
#include <QNetworkReply>
#include <QNetworkAccessManager>


class QNetworkReply;


class HttpWroker final : public QObject
{
    Q_OBJECT
public:
    HttpWroker(QObject* const parent = nullptr);
    HttpWroker(const HttpWroker&) = delete;
    HttpWroker& operator=(const HttpWroker) = delete;


    virtual ~HttpWroker();

public slots:
    void post(const QUrl& url, const QString& file_path_str);
    void get(const QUrl& url);


signals:
    void finished();
    void uploadProgress(std::size_t sent_bytes, std::size_t total);
    void errorOccured(QNetworkReply::NetworkError err);
    void fileListJson(const QByteArray& json);


private slots:
    void readContent();

private:
    QNetworkAccessManager* network_access_manager_;
    QNetworkReply* network_reply_;

    QByteArray file_list_json_;
};



#endif // HTTPWORKER_H
