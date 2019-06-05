#include "mainwindow.h"
#include "selectfileandshowprogress.h"

#include <QDir>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w{QUrl{"http://127.0.0.1:8888"}};
    w.show();

    return a.exec();
}
