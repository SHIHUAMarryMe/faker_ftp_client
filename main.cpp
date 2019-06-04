#include "mainwindow.h"
#include "selectfileandshowprogress.h"

#include <QDir>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QDir dir{"."};
    QList<QString> files_name{ dir.entryList() };

//    for(const auto& file_name : files_name)
//    {
//        qDebug() << file_name;
//    }

    MainWindow w;
    w.putFileListInModel(files_name);
    w.show();

    return a.exec();
}
