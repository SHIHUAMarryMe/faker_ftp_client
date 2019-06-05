

#include "httpworker.h"
#include "httpworkercontroller.h"


HttpWorkerController::HttpWorkerController(QObject * const parent)
    :QObject{parent},
      worker_thread_{new QThread},
      http_worker_{new HttpWroker}
{
    this->http_worker_->moveToThread(worker_thread_);

    QObject::connect(this->http_worker_, &HttpWroker::finished, [this](){ emit this->finished(); });
    QObject::connect(this->http_worker_, &HttpWroker::errorOccured, this, &HttpWorkerController::setErrorFlag);
    QObject::connect(this->http_worker_, &HttpWroker::uploadProgress, [this](std::size_t sent, std::size_t total){ emit this->uploadProgress(sent, total); });
    QObject::connect(this->http_worker_, &HttpWroker::fileListJson, [this](const QByteArray& json_str){ emit this->fileListJson(json_str); });

    QObject::connect(this, &HttpWorkerController::post, [this](const QUrl& url, const QString& file_path_str){ http_worker_->upload(url, file_path_str); });
    QObject::connect(this, &HttpWorkerController::get, [this](const QUrl& url){ http_worker_->get(url); });

}

HttpWorkerController::~HttpWorkerController()
{
    this->http_worker_->deleteLater();

    this->worker_thread_->quit();
    this->worker_thread_->wait();
}

void HttpWorkerController::setErrorFlag(QNetworkReply::NetworkError err)
{
    if(err != QNetworkReply::NetworkError::NoError)
    {
        error_flag_.store(true, std::memory_order_consume);
    }

    emit error(err);
}
