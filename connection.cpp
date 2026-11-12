#include "connection.h"

#include <QSqlError>
#include <QDebug>
#include <QStringList>

// ---- Paramètres de connexion (à adapter à ta machine) -------------------
static const char *kConnectionName = "ConnectionSingleton";
static const char *kDefaultDsn     = "source_projet_2a";
static const char *kDefaultUser    = "chahd";
static const char *kDefaultPass    = "chahd2005";
// --------------------------------------------------------------------------

Connection& Connection::createInstance()
{
    static Connection instance;
    return instance;
}

Connection::Connection()
{
    ouvrir();
}

Connection::~Connection()
{
    if (m_database.isValid() && m_database.isOpen())
        m_database.close();
}

bool Connection::ouvrir()
{
    if (!QSqlDatabase::isDriverAvailable("QODBC")) {
        m_lastError =
            "Le pilote QODBC de Qt n'est pas disponible.\n"
            "Pilotes détectés : " + QSqlDatabase::drivers().join(", ");
        qWarning().noquote() << m_lastError;
        return false;
    }

    if (QSqlDatabase::contains(kConnectionName))
        m_database = QSqlDatabase::database(kConnectionName, false);
    else
        m_database = QSqlDatabase::addDatabase("QODBC", kConnectionName);

    if (m_database.isOpen())
        m_database.close();

    m_database.setDatabaseName(qEnvironmentVariable("ORACLE_DSN",      kDefaultDsn));
    m_database.setUserName    (qEnvironmentVariable("ORACLE_USER",     kDefaultUser));
    m_database.setPassword    (qEnvironmentVariable("ORACLE_PASSWORD", kDefaultPass));

    if (!m_database.open()) {
        m_lastError = m_database.lastError().text();
        qWarning().noquote() << "Erreur Oracle :" << m_lastError;
        return false;
    }

    m_lastError.clear();
    qDebug() << "Connexion Oracle réussie (DSN =" << m_database.databaseName() << ")";
    return true;
}

bool Connection::reconnect()
{
    return ouvrir();
}

QSqlDatabase Connection::database() const
{
    return m_database;
}

bool Connection::isConnected() const
{
    return m_database.isValid() && m_database.isOpen();
}

QString Connection::lastError() const
{
    return m_lastError;
}
