

#include "httpworker.h"
#include "httpworkercontroller.h"


HttpWorkerController::HttpWorkerController(QObject * const parent)
    :QObject{parent},
      worker_thread_{new QThread},
      http_worker_{new HttpWroker}
{
    this->http_worker_->moveToThread(worker_thread_);

    QObject::connect(this->http_worker_, &HttpWroker::finished, [this](){ emit this->finished(); });
    QObject::connect(this->http_worker_, &HttpWroker::errorOccured, [this](QNetworkReply::NetworkError err){ emit this->error(err); });
    QObject::connect(this->http_worker_, &HttpWroker::uploadProgress, [this](std::size_t sent, std::size_t total){ emit this->uploadProgress(sent, total); });
    QObject::connect(this->http_worker_, &HttpWroker::fileListJson, [this](const QByteArray& json_str){ emit this->fileListJson(json_str); });

    QObject::connect(this, &HttpWorkerController::post, [this](const QUrl& url, const QString& file_path_str){ this->http_worker_->post(url, file_path_str); });
    QObject::connect(this, &HttpWorkerController::post, [this](const QUrl& url){ this->http_worker_->get(url); });

}

HttpWorkerController::~HttpWorkerController()
{
    this->http_worker_->deleteLater();

    this->worker_thread_->quit();
    this->worker_thread_->wait();
}
