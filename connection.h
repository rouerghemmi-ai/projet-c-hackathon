#ifndef CONNECTION_H
#define CONNECTION_H

#include <QSqlDatabase>
#include <QString>

/*
 * Connexion Oracle <-> Qt (via ODBC, pilote QODBC).
 *
 * Paramètres utilisés (modifiables dans connection.cpp, ou via les
 * variables d'environnement ORACLE_DSN / ORACLE_USER / ORACLE_PASSWORD) :
 *   - DSN      : nom de la source de données ODBC  -> "source_projet_2a"
 *   - Utilisateur / mot de passe Oracle
 */
class Connection
{
public:
    static Connection& createInstance();

    QSqlDatabase database() const;
    bool isConnected() const;
    QString lastError() const;

    // Retente la connexion (utile si Oracle a été démarré après l'application).
    bool reconnect();

    Connection(const Connection&) = delete;
    Connection& operator=(const Connection&) = delete;

private:
    Connection();
    ~Connection();

    bool ouvrir();

    QSqlDatabase m_database;
    QString m_lastError;
};

#endif // CONNECTION_H
