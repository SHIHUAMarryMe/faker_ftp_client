#ifndef HTTPWORKERCONTROLLER_H
#define HTTPWORKERCONTROLLER_H


#include <QObject>
#include <QThread>
#include <QNetworkReply>


class HttpWroker;


class HttpWorkerController final : public QObject
{
    Q_OBJECT
public:
    HttpWorkerController(QObject* const parent = nullptr);
    HttpWorkerController(const HttpWorkerController&) = delete;
    HttpWorkerController& operator=(const HttpWorkerController&) = delete;

    virtual ~HttpWorkerController();

signals:
    void finished();
    void fileListJson(const QByteArray& json_str);
    void error(QNetworkReply::NetworkError err);
    void uploadProgress(std::size_t sent_bytes, std::size_t total_bytes);

    void post(const QUrl& url, const QString& file_path_str);
    void get(const QUrl& url);

private:
    QThread* worker_thread_;
    HttpWroker* http_worker_;
};




#endif // HTTPWORKERCONTROLLER_H
