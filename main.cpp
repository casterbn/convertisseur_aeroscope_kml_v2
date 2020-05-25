#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    /* Création de l'application */
    QApplication a(argc, argv);
    MainWindow w;
    a.setWindowIcon(QIcon("icon.png"));
    w.showMaximized();
    w.setWindowTitle("Convertisseur Aéroscope - KML v1.0");

    /* mainwindow.cpp // mainwindow.h */
    w.show();
    return a.exec();
}
