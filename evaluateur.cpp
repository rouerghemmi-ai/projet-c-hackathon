#include "evaluateur.h"
#include "oracletable.h"

static OracleTable &table()
{
    static OracleTable t("EVALUATEUR", {
        {"id",     {"^ID"}},
        {"nom",    {"^NOM"}},
        {"prenom", {"^PRENOM"}},
        {"statut", {"^STATUT", "^ETAT", "^DISPONIB"}},
        {"projet", {"^PROJET$", "^NOM_PROJET"}},
    }, "E");
    return t;
}

static QVariantMap versMap(const Evaluateur &e)
{
    QVariantMap m;
    m["id"] = e.id;
    m["nom"] = e.nom;
    m["prenom"] = e.prenom;
    m["statut"] = e.statut;
    // Le projet n'est enregistré que si la table possède une colonne texte dédiée.
    return m;
}

bool Evaluateur::initialiser(QString *err) { return table().init(err); }
bool Evaluateur::idNumerique() { return table().pret() && table().idNumerique(); }
QString Evaluateur::prochainId() { return table().prochainId(); }

bool Evaluateur::ajouter(QString *err) const
{
    return table().ajouter(versMap(*this), nullptr, err);
}

bool Evaluateur::modifier(const QString &ancienId, QString *err) const
{
    return table().modifier(ancienId, versMap(*this), err);
}

bool Evaluateur::supprimer(const QString &id, QString *err, bool cascade)
{
    return table().supprimer(id, err, cascade);
}

bool Evaluateur::lister(QList<Evaluateur> &resultat, QString *err)
{
    resultat.clear();
    QList<QVariantMap> lignes;
    if (!table().lister(lignes, err)) return false;
    for (const QVariantMap &m : lignes) {
        QString statut = m.value("statut").toString();
        if (statut.isEmpty()) statut = "Actif";
        QString projet = m.value("projet").toString();
        if (projet.isEmpty()) projet = QStringLiteral("À affecter");
        resultat.append(Evaluateur(m.value("id").toString(), m.value("nom").toString(),
                                   m.value("prenom").toString(), statut, projet));
    }
    return true;
}
