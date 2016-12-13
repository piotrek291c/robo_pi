#include "mainwindow.h"
#include <QApplication>
#include <QWebView>
int main(int argc, char *argv[])
{

    Q_INIT_RESOURCE(zasoby); // za za zadanie włączenie zasobów do pliku wykonywalnego. Jako parametr, podajemy nazwę pliku zasobów, bez rozszerzenia.


    QApplication a(argc, argv);
    MainWindow w;
    w.show();
   // QWebView view;
  //      view.show();
  //      view.load(QUrl("http://google.com"));


    return a.exec();
}


