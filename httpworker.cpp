

#include <QDir>
#include <QNetworkRequest>
#include <QString>
#include <QFile>
#include <QHttpPart>
#include <functional>

#include "httpworker.h"



HttpWroker::HttpWroker(QObject * const parent)
    :QObject{parent},
      network_access_manager_{new QNetworkAccessManager},
      network_reply_{nullptr}
{
}

HttpWroker::~HttpWroker()
{
}

void HttpWroker::upload(const QUrl &url, const QString &file_path_str)
{
    if(url.isEmpty() || !url.isValid())
    {
        emit errorOccured(QNetworkReply::NetworkError::ProtocolUnknownError);
        return;
    }

    if(!QFileInfo::exists(file_path_str))
    {
        emit errorOccured(QNetworkReply::NetworkError::ProtocolUnknownError);
        return;
    }



    QNetworkRequest request{url};
    QFileInfo file_info{ file_path_str };
    QHttpPart file_part{};
    QHttpMultiPart *multi_part{ new QHttpMultiPart{QHttpMultiPart::FormDataType} };

    QString disposition_header{"form-data; name=\"%1\""};
    disposition_header = disposition_header.arg(file_info.baseName());

    file_part.setHeader(QNetworkRequest::ContentTypeHeader, QVariant{"application/octet-stream"});
    file_part.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant{disposition_header});
    file_part.setHeader(QNetworkRequest::ContentLengthHeader, QVariant{file_info.size()});

    QFile *file_ptr{ new QFile{file_path_str}};
    file_ptr->open(QIODevice::ReadOnly);
    file_part.setBodyDevice(file_ptr);


    multi_part->append(file_part);

    network_reply_ = network_access_manager_->post(request, multi_part);

    multi_part->setParent(network_reply_);

    QObject::connect(this->network_access_manager_, &QNetworkAccessManager::finished,
    [this](QNetworkReply* reply)
    {
        (void)reply;

        if(this->network_reply_->error() == QNetworkReply::NoError)
        {

            this->network_reply_->deleteLater();

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
    network_reply_ = network_access_manager_->get(request);


    QObject::connect(network_access_manager_, &QNetworkAccessManager::finished,
    [this](QNetworkReply* reply)
    {
        (void)reply;

        if(this->network_reply_->error() == QNetworkReply::NoError)
        {
            this->network_reply_->deleteLater();

            qDebug() << __FUNCTION__ << " " << file_list_json_;

            emit fileListJson(file_list_json_);

        }

        emit this->finished();

    });

    QObject::connect(this->network_reply_, static_cast<void(QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error),
    [this](QNetworkReply::NetworkError err)
    {
        emit this->errorOccured(err);
    });

    QObject::connect(network_reply_, &QNetworkReply::readyRead,
                     [this](){
                               auto data{network_reply_->readAll()};
                               file_list_json_ += data;
                              });
}



