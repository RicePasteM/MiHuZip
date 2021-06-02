#include "mainwindow.h"

#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QPalette palette(w.palette());
    palette.setColor(QPalette::Window, QColor(68,69,73));
    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    w.setStyleSheet("padding:0;border:0;margin:0");
    w.setSizePolicy(sizePolicy);
    w.setPalette(palette);
    w.setFixedSize(350,360);
    w.setWindowTitle("米哈压 Version 1.0");
    w.show();
    return a.exec();
}
