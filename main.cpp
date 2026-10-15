#include "gestion_evaluateurs.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    GestionEvaluateurs window;
    window.show();
    return app.exec();
}
