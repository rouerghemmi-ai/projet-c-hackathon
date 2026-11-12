#ifndef ORACLETABLE_H
#define ORACLETABLE_H

#include <QString>
#include <QStringList>
#include <QList>
#include <QMap>
#include <QVariantMap>

/*
 * Accès générique à une table Oracle existante.
 *
 * Tu décris les champs « logiques » (id, nom, prenom...) avec des motifs
 * (expressions régulières) ; au premier accès, la classe lit les colonnes
 * réelles de la table (USER_TAB_COLUMNS / ALL_TAB_COLUMNS) et fait la
 * correspondance automatiquement. Un champ sans colonne correspondante est
 * simplement ignoré.
 */
struct ChampLogique {
    QString cle;          // ex. "nom"
    QStringList motifs;   // ex. {"^NOM"}  (insensible à la casse)
};

class OracleTable
{
public:
    OracleTable(const QString &table, const QList<ChampLogique> &champs,
                const QString &prefixeIdParDefaut = QString());

    // Résout la table et ses colonnes. Renvoie false + message si impossible.
    bool init(QString *err = nullptr);

    bool pret() const { return m_pret; }
    bool disponible(const QString &cle) const { return m_colonnes.contains(cle); }
    QString colonne(const QString &cle) const { return m_colonnes.value(cle); }
    bool idNumerique() const { return m_idNumerique; }
    QString nomComplet() const { return m_nomComplet; }

    bool lister(QList<QVariantMap> &resultat, QString *err = nullptr);
    bool ajouter(const QVariantMap &valeurs, QString *idUtilise = nullptr, QString *err = nullptr);
    bool modifier(const QString &ancienId, const QVariantMap &valeurs, QString *err = nullptr);
    // cascade = true : supprime aussi les lignes liées des autres tables (clés étrangères).
    bool supprimer(const QString &id, QString *err = nullptr, bool cascade = false);

    // Prochain identifiant libre (MAX+1, en gardant le préfixe/format existant).
    QString prochainId(QString *err = nullptr);

private:
    QVariant valeurId(const QString &id) const;
    // Exécute une requête préparée ; en cas d'échec, réessaie avec les valeurs
    // écrites directement dans le SQL (contourne les soucis de paramètres du pilote ODBC)
    // et renvoie un message détaillé (requête + code Oracle).
    bool executer(const QString &sql, const QVariantList &params, QString *err, int *affectees = nullptr);
    bool supprimerEnCascade(const QString &id, QString *err);
    static QString messageErreur(const QString &brut);

    struct InfoCol { QString nom, type; int longueur = 0; bool obligatoire = false; };
    QList<InfoCol> m_infos;               // toutes les colonnes réelles de la table
    QString descriptionTable() const;

    QString m_table;
    QList<ChampLogique> m_champs;
    QString m_prefixeDefaut;
    bool m_pret = false;
    bool m_idNumerique = false;
    QString m_owner;
    QString m_nomComplet;                 // [PROPRIETAIRE.]TABLE
    QMap<QString, QString> m_colonnes;    // cle logique -> colonne réelle
    QMap<QString, QString> m_types;       // cle logique -> type Oracle
};

#endif // ORACLETABLE_H
