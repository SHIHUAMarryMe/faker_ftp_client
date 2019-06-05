#ifndef HTTPWORKERCONTROLLER_H
#define HTTPWORKERCONTROLLER_H


#include <QObject>
#include <QThread>
#include <QNetworkReply>

#include <atomic>


class HttpWroker;


class HttpWorkerController final : public QObject
{
    Q_OBJECT
public:
    HttpWorkerController(QObject* const parent = nullptr);
    HttpWorkerController(const HttpWorkerController&) = delete;
    HttpWorkerController& operator=(const HttpWorkerController&) = delete;

    virtual ~HttpWorkerController();

    inline bool whetherOccuredErr()noexcept
    {
        return error_flag_.load(std::memory_order_release);
    }

signals:
    void finished();
    void fileListJson(const QByteArray& json_str);
    void error(QNetworkReply::NetworkError err);
    void uploadProgress(std::size_t sent_bytes, std::size_t total_bytes);

    void post(const QUrl& url, const QString& file_path_str);
    void get(const QUrl& url);


private slots:
    void setErrorFlag(QNetworkReply::NetworkError err);

private:
    QThread* worker_thread_;
    HttpWroker* http_worker_;

    std::atomic<bool> error_flag_{false};
};




#endif // HTTPWORKERCONTROLLER_H
