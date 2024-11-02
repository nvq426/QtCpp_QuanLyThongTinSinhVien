#include <QApplication>
#include <QIcon>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/hinhanh/bieutuong.ico"));
    MainWindow w;
    w.show();
    return app.exec();
}
