#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile style_file(":/Remover.qss");
    style_file.open(QIODevice::ReadOnly);
    a.setStyleSheet(style_file.readAll());
    style_file.close();
    MainWindow w;
    w.show();
    return a.exec();
}
