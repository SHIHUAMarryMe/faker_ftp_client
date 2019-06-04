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
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void putFileListInModel(const QList<QString>& files_name)noexcept;


public slots:
    void errorOccured(QNetworkReply::NetworkError err);


private slots:
    void onGetFileListJons(const QByteArray& file_list_json);


private:
    void getFileListInServer();


    QListView* list_view_;
    QStandardItemModel* standard_item_model_;
    QSortFilterProxyModel* sort_filter_proxy_model_;
    ItemDelegate* item_delegate_;


    SelectFileAndShowProgressWidget* progress_widget_;
};

#endif // MAINWINDOW_H
