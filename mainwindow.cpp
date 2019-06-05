
#include <QDir>
#include <QList>
#include <QUrl>
#include <QListView>
#include <QMessageBox>
#include <QStringListModel>
#include <QVBoxLayout>
#include <QSortFilterProxyModel>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>

#include "itemdef.h"
#include "itemdelegate.h"
#include "mainwindow.h"
#include "httpworkercontroller.h"
#include "selectfileandshowprogress.h"



MainWindow::MainWindow(const QUrl& url, QWidget *parent)
    : QMainWindow(parent),
      list_view_{new QListView},
      standard_item_model_{new QStandardItemModel},
      progress_widget_{new SelectFileAndShowProgressWidget},
      item_delegate_{ new ItemDelegate },
      sort_filter_proxy_model_{ new QSortFilterProxyModel },
      url_{url}
{
    this->setMinimumSize(600, 400);

    sort_filter_proxy_model_->setSourceModel(this->standard_item_model_);
//    sort_filter_proxy_model_->setDynamicSortFilter(true);


    list_view_->setSpacing(20);
    list_view_->setItemDelegate(item_delegate_);
    list_view_->setModel(sort_filter_proxy_model_);
    list_view_->setViewMode(QListView::IconMode);
    list_view_->setDragEnabled(false);
    list_view_->setMinimumWidth(600);
    list_view_->setEditTriggers(QListView::EditTrigger::NoEditTriggers);


    QVBoxLayout* v_box_layout{ new QVBoxLayout };
    v_box_layout->addWidget(progress_widget_);
    v_box_layout->addWidget(this->list_view_);



    QWidget* conteral_widget{ new QWidget };
    conteral_widget->setBaseSize(800, 600);
    conteral_widget->setLayout(v_box_layout);

    this->setCentralWidget(conteral_widget);
    this->getFileListInServer();

    QObject::connect(progress_widget_, &SelectFileAndShowProgressWidget::beSelecedPath, this, &MainWindow::uploadFile);

}

MainWindow::~MainWindow()
{
}

void MainWindow::putFileListInModel(const QList<QString> &files_name)noexcept
{
    for(const QString& file_name : files_name)
    {
        ItemData item_data{file_name, QString{"://img/file_ico.png"}};
        QStandardItem* item{ new QStandardItem };

        item->setData(QVariant::fromValue(item_data), Qt::UserRole+1);
        standard_item_model_->appendRow(item);
    }
}

void MainWindow::uploadFile(const QString& file_path_str)
{

    HttpWorkerController* http_controller{ new HttpWorkerController };

    QObject::connect(http_controller, &HttpWorkerController::finished,
    [this, http_controller, file_path_str]
    {
        qDebug() << http_controller->whetherOccuredErr();

        if(!http_controller->whetherOccuredErr())
        {
            QFileInfo file_info{file_path_str};
            ItemData item_data{file_info.fileName(), QString{"://img/file_ico.png"}};
            QStandardItem* item{ new QStandardItem };

            item->setData(QVariant::fromValue(item_data), Qt::UserRole+1);
            standard_item_model_->appendRow(item);
            list_view_->update();
            progress_widget_->setPercent(100);

        }else{

            QMessageBox msgBox;
            msgBox.setText(tr("failed to uploda file."));
            msgBox.exec();
        }

        http_controller->deleteLater();
    });


    QObject::connect(http_controller, &HttpWorkerController::error, this, &MainWindow::errorOccured);
    QObject::connect(http_controller, &HttpWorkerController::uploadProgress,
    [this](std::size_t sent_bytes, std::size_t total)
    {
        if(total != 0)
        {
            double percent{ double(sent_bytes)/double(total) };
            progress_widget_->setPercent(static_cast<std::size_t>(percent * 100));
        }


    });

    QString server_addr_port{url_.toString()};
    server_addr_port.push_back(QString{"?filename=%1"}.arg(QFileInfo(file_path_str).fileName()));

    emit http_controller->post(QUrl{server_addr_port}, file_path_str);
}

void MainWindow::errorOccured(QNetworkReply::NetworkError err)
{
    progress_widget_->setPercent(0);

    QString err_str{ QString::number(static_cast<int>(err)) };
    qDebug() << "error occured: " << err_str;

}

void MainWindow::onGetFileListJons(const QByteArray &file_list_json)
{
    QJsonParseError json_error{};
    QJsonDocument json_doc(QJsonDocument::fromJson(file_list_json, &json_error));

    if(json_error.error != QJsonParseError::NoError)
    {
        qDebug() << "json error!";
        return;
    }

    QJsonObject json_obj{ json_doc.object() };

    if(json_obj.contains("files_name"))
    {
        auto files_name(json_obj.value("files_name").toArray());

        qDebug() << files_name;

        for(std::size_t index = 0; index < files_name.size(); ++index)
        {
            ItemData item_data{files_name[index].toString(), QString{"://img/file_ico.png"}};
            QStandardItem* item{ new QStandardItem };

            item->setData(QVariant::fromValue(item_data), Qt::UserRole+1);
            standard_item_model_->appendRow(item);
            list_view_->update();
        }
    }

}

void MainWindow::getFileListInServer()
{
    HttpWorkerController* http_controller{ new HttpWorkerController };


    QObject::connect(http_controller, &HttpWorkerController::fileListJson, this, &MainWindow::onGetFileListJons);
    QObject::connect(http_controller, &HttpWorkerController::error, this, &MainWindow::errorOccured);
    QObject::connect(http_controller, &HttpWorkerController::finished, http_controller, &HttpWorkerController::deleteLater);
    QObject::connect(http_controller, &HttpWorkerController::fileListJson, this, &MainWindow::onGetFileListJons);

    QString server_addr_port{url_.toString()};
    server_addr_port.push_back("?query=all");

    emit http_controller->get(QUrl{server_addr_port});
}
