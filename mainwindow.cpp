
#include <QDir>
#include <QList>
#include <QListView>
#include <QStringListModel>
#include <QVBoxLayout>
#include <QSortFilterProxyModel>

#include "itemdef.h"
#include "itemdelegate.h"
#include "mainwindow.h"
#include "httpworkercontroller.h"
#include "selectfileandshowprogress.h"




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      list_view_{new QListView},
      standard_item_model_{new QStandardItemModel},
      progress_widget_{new SelectFileAndShowProgressWidget},
      item_delegate_{ new ItemDelegate },
      sort_filter_proxy_model_{ new QSortFilterProxyModel }
{
    this->setMinimumSize(600, 400);


    sort_filter_proxy_model_->setSourceModel(this->standard_item_model_);
//    sort_filter_proxy_model_->setDynamicSortFilter(true);

    list_view_->setSpacing(5);
    list_view_->setItemDelegate(item_delegate_);
    list_view_->setModel(sort_filter_proxy_model_);
    list_view_->setViewMode(QListView::IconMode);
    list_view_->setDragEnabled(false);

    list_view_->setMinimumWidth(600);


    QVBoxLayout* v_box_layout{ new QVBoxLayout };
    v_box_layout->addWidget(progress_widget_);
    v_box_layout->addWidget(this->list_view_);



    QWidget* conteral_widget{ new QWidget };
    conteral_widget->setBaseSize(800, 600);
    conteral_widget->setLayout(v_box_layout);

    this->setCentralWidget(conteral_widget);
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

void MainWindow::errorOccured(QNetworkReply::NetworkError err)
{

    QString err_str{ QString::number(static_cast<int>(err)) };
    qDebug() << err_str;

}

void MainWindow::onGetFileListJons(const QByteArray &file_list_json)
{
    qDebug() << file_list_json;
}

void MainWindow::getFileListInServer()
{
    HttpWorkerController* http_controller{ new HttpWorkerController };

    QObject::connect(http_controller, &HttpWorkerController::finished,
    [this, http_controller]()
    {
        http_controller->deleteLater();
        // here set progress.
    }
    );

    QObject::connect(http_controller, &HttpWorkerController::fileListJson, this, &MainWindow::onGetFileListJons);
    QObject::connect(http_controller, &HttpWorkerController::error, this, &MainWindow::errorOccured);

}
