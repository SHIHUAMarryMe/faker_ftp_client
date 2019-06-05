#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkReply>

class QListView;
class ItemDelegate;
class QStandardItemModel;
class QSortFilterProxyModel;
class SelectFileAndShowProgressWidget;


class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(const QUrl &url, QWidget *parent = nullptr);
    virtual ~MainWindow();


    void putFileListInModel(const QList<QString>& files_name)noexcept;
    void uploadFile(const QString& file_path_str);


public slots:
    void errorOccured(QNetworkReply::NetworkError err);


private slots:
    void onGetFileListJons(const QByteArray& file_list_json);


private:
    void getFileListInServer();


    QListView* list_view_;
    QStandardItemModel* standard_item_model_;
    QSortFilterProxyModel* sort_filter_proxy_model_;
    SelectFileAndShowProgressWidget* progress_widget_;
    ItemDelegate* item_delegate_;
    QUrl url_;
};

#endif // MAINWINDOW_H
