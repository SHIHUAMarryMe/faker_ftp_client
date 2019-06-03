
#include <QDir>
#include <QList>
#include <QListView>
#include <QStringListModel>
#include <QVBoxLayout>


#include "mainwindow.h"
#include "httpworkercontroller.h"
#include "selectfileandshowprogress.h"




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      list_view_{new QListView{nullptr}},
      progress_widget_{new SelectFileAndShowProgressWidget}
{
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
