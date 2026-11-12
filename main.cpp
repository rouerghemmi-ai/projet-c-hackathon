
#include "integration.h"
#include "connection.h"

#include <QApplication>
#include <QFont>
#include <QDebug>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFont font("Arial", 10);
    a.setFont(font);

    Connection& connection =
        Connection::createInstance();

    if (!connection.isConnected()) {
        qWarning().noquote()
        << "Oracle non connecté :"
        << connection.lastError();
        QMessageBox::warning(nullptr, "Oracle non connecté",
            "Connexion à Oracle impossible :\n\n" + connection.lastError() +
            "\n\nL'application démarre en mode hors ligne (données de démonstration).\n"
            "Voir GUIDE_ORACLE.md.");
    }

    integration w;
    w.showMaximized();

    return a.exec();
}
