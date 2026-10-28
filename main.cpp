
#include "integration.h"

#include <QApplication>
#include <QFont>
#include <QSqlDatabase>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFont font("Segoe UI", 11);
    a.setFont(font);

    qDebug() << "Pilotes SQL disponibles :"
             << QSqlDatabase::drivers();

    integration w;
    w.showMaximized();

    return a.exec();
}
