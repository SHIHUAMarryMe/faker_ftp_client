

#include <QDir>
#include <QNetworkRequest>
#include <QString>

#include <functional>

#include "httpworker.h"



HttpWroker::HttpWroker(QObject * const parent)
    :QObject{parent},
      network_access_manager_{new QNetworkAccessManager{this}},
      network_reply_{nullptr}
{
}

HttpWroker::~HttpWroker()
{
}

void HttpWroker::post(const QUrl &url, const QString &file_path_str)
{
    if(url.isEmpty() || !url.isValid())
    {
        emit errorOccured(QNetworkReply::NetworkError::ProtocolUnknownError);
    }

    if(!QFileInfo::exists(file_path_str))
    {
        emit errorOccured(QNetworkReply::NetworkError::ProtocolUnknownError);
    }



    QFile file{file_path_str};

    if(!file.open(QIODevice::ReadOnly))
    {
        emit errorOccured(QNetworkReply::NetworkError::ProtocolUnknownError);
    }

    QNetworkRequest request{url};
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/octet-stream"));
    QByteArray byte_array{ file.readAll() };

    this->network_reply_ = this->network_access_manager_->post(request, byte_array);

    QObject::connect(this->network_access_manager_, &QNetworkAccessManager::finished,
    [this](QNetworkReply* reply)
    {
        (void)reply;

        if(this->network_reply_->error() == QNetworkReply::NoError)
        {

            this->network_reply_->deleteLater();

        }else{

            emit this->errorOccured(this->network_reply_->error());

        }

        emit this->finished();
    });

    QObject::connect(this->network_reply_, &QNetworkReply::uploadProgress,
    [this](std::size_t uploaded_bytes, std::size_t total_bytes){ emit this->uploadProgress(uploaded_bytes, total_bytes); });

    QObject::connect(this->network_reply_, static_cast<void(QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error),
    [this](QNetworkReply::NetworkError err){ emit this->errorOccured(err); });

}

void HttpWroker::get(const QUrl &url)
{
    if(url.isEmpty() || !url.isValid())
    {
        emit errorOccured(QNetworkReply::NetworkError::ProtocolUnknownError);
    }

    QNetworkRequest request{url};
    this->network_reply_ = this->network_access_manager_->get(request);


    QObject::connect(this->network_access_manager_, &QNetworkAccessManager::finished,
    [this](QNetworkReply* reply)
    {
        (void)reply;

        if(this->network_reply_->error() == QNetworkReply::NoError)
        {
            this->network_reply_->deleteLater();
            emit this->fileListJson(this->file_list_json_);

        }else{

            emit this->errorOccured(this->network_reply_->error());

        }

        emit this->finished();
    });

    QObject::connect(this->network_reply_, static_cast<void(QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error),
    [this](QNetworkReply::NetworkError err)
    {
        emit this->errorOccured(err);
    });
}

void HttpWroker::readContent()
{
    QByteArray file_list_json{ this->network_reply_->readAll() };
    this->file_list_json_.push_back(file_list_json);
}



