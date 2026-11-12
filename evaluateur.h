#ifndef EVALUATEUR_H
#define EVALUATEUR_H

#include <QString>
#include <QList>

/*
 * Entité « Evaluateur » : opérations sur la table Oracle EVALUATEUR
 * (les colonnes sont détectées automatiquement, voir oracletable.h).
 */
class Evaluateur
{
public:
    Evaluateur() = default;
    Evaluateur(const QString &id, const QString &nom, const QString &prenom,
               const QString &statut, const QString &projet)
        : id(id), nom(nom), prenom(prenom), statut(statut), projet(projet) {}

    QString id, nom, prenom, statut, projet;

    bool ajouter(QString *err = nullptr) const;                           // INSERT
    bool modifier(const QString &ancienId, QString *err = nullptr) const; // UPDATE
    static bool supprimer(const QString &id, QString *err = nullptr, bool cascade = false);     // DELETE
    static bool lister(QList<Evaluateur> &resultat, QString *err = nullptr); // SELECT

    static bool initialiser(QString *err = nullptr);  // vérifie table + colonnes
    static QString prochainId();                       // identifiant libre suivant
    static bool idNumerique();
};

#endif // EVALUATEUR_H
