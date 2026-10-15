#include "integration.h"

#include <QApplication>
#include <QFont>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFont font("Arial", 10);
    a.setFont(font);

    integration w;
    w.showMaximized();
    return QApplication::exec();
}