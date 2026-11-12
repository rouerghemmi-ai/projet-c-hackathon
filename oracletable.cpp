#include "oracletable.h"
#include "connection.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QRegularExpression>
#include <QDebug>
#include <QSet>
#include <QPair>

OracleTable::OracleTable(const QString &table, const QList<ChampLogique> &champs,
                         const QString &prefixeIdParDefaut)
    : m_table(table.toUpper()), m_champs(champs), m_prefixeDefaut(prefixeIdParDefaut)
{
}

QString OracleTable::messageErreur(const QString &brut)
{
    QString m = brut;
    if (brut.contains("ORA-02292"))
        m += "\n\n→ Cet enregistrement est utilisé dans une autre table (clé étrangère). "
             "Supprimez d'abord les lignes liées.";
    else if (brut.contains("ORA-00001"))
        m += "\n\n→ Cet identifiant existe déjà.";
    else if (brut.contains("ORA-01400"))
        m += "\n\n→ Une colonne obligatoire de la table n'est pas renseignée par l'application.";
    else if (brut.contains("ORA-02291"))
        m += "\n\n→ Une valeur référence un enregistrement parent qui n'existe pas.";
    return m;
}

bool OracleTable::init(QString *err)
{
    if (m_pret) return true;

    Connection &c = Connection::createInstance();
    if (!c.isConnected()) {
        if (err) *err = "Pas de connexion Oracle : " + c.lastError();
        return false;
    }

    // Colonnes de la table : d'abord dans le schéma courant, sinon dans un autre schéma.
    QSqlQuery q(c.database());
    q.prepare("SELECT owner, column_name, data_type, data_length, nullable FROM all_tab_columns "
              "WHERE table_name = ? "
              "ORDER BY CASE WHEN owner = SYS_CONTEXT('USERENV','CURRENT_SCHEMA') THEN 0 ELSE 1 END, "
              "owner, column_id");
    q.addBindValue(m_table);
    if (!q.exec()) {
        if (err) *err = q.lastError().text();
        return false;
    }

    QString owner;
    QList<QPair<QString, QString>> cols;   // nom, type
    QList<InfoCol> infos;
    while (q.next()) {
        const QString o = q.value(0).toString();
        if (owner.isEmpty()) owner = o;
        if (o != owner) break;
        cols.append({q.value(1).toString(), q.value(2).toString()});
        InfoCol info;
        info.nom = q.value(1).toString();
        info.type = q.value(2).toString();
        info.longueur = q.value(3).toInt();
        info.obligatoire = (q.value(4).toString() == "N");
        infos.append(info);
    }
    if (cols.isEmpty()) {
        if (err) *err = QString("La table %1 est introuvable (ou accès refusé) pour l'utilisateur Oracle "
                                "de l'application. Vérifiez l'utilisateur dans connection.cpp.").arg(m_table);
        return false;
    }

    m_infos = infos;
    m_owner = owner;

    QSqlQuery cur(c.database());
    QString schemaCourant;
    if (cur.exec("SELECT SYS_CONTEXT('USERENV','CURRENT_SCHEMA') FROM dual") && cur.next())
        schemaCourant = cur.value(0).toString();
    m_nomComplet = (owner == schemaCourant || owner.isEmpty()) ? m_table : owner + "." + m_table;

    m_colonnes.clear();
    m_types.clear();
    QSet<QString> prises;
    for (const ChampLogique &champ : m_champs) {
        bool trouve = false;
        for (const QString &motif : champ.motifs) {
            QRegularExpression re(motif, QRegularExpression::CaseInsensitiveOption);
            for (const auto &col : cols) {
                if (prises.contains(col.first)) continue;
                if (re.match(col.first).hasMatch()) {
                    m_colonnes[champ.cle] = col.first;
                    m_types[champ.cle] = col.second;
                    prises.insert(col.first);
                    trouve = true;
                    break;
                }
            }
            if (trouve) break;
        }
    }
    // L'identifiant est indispensable : à défaut, première colonne de la table.
    if (!m_colonnes.contains("id")) {
        m_colonnes["id"] = cols.first().first;
        m_types["id"] = cols.first().second;
    }
    m_idNumerique = m_types.value("id").startsWith("NUMBER", Qt::CaseInsensitive)
                    || m_types.value("id").startsWith("INTEGER", Qt::CaseInsensitive)
                    || m_types.value("id").startsWith("FLOAT", Qt::CaseInsensitive);

    qDebug().noquote() << "Table" << m_nomComplet << "→" << m_colonnes;
    m_pret = true;
    return true;
}


static QString litteral(const QVariant &v)
{
    switch (v.userType()) {
    case QMetaType::Int: case QMetaType::UInt: case QMetaType::LongLong:
    case QMetaType::ULongLong: case QMetaType::Double:
        return v.toString();
    default: break;
    }
    if (v.isNull()) return "NULL";
    QString t = v.toString();
    t.replace("'", "''");
    return "'" + t + "'";
}

static QString detailErreur(const QSqlQuery &q)
{
    const QSqlError e = q.lastError();
    QString m = e.databaseText().trimmed();
    if (m.isEmpty()) m = e.driverText().trimmed();
    if (!e.nativeErrorCode().isEmpty()) m += " (code " + e.nativeErrorCode() + ")";
    return m;
}

bool OracleTable::executer(const QString &sql, const QVariantList &params, QString *err, int *affectees)
{
    QSqlDatabase db = Connection::createInstance().database();

    // 1) requête préparée
    QSqlQuery q(db);
    q.prepare(sql);
    for (const QVariant &p : params) q.addBindValue(p);
    if (q.exec()) { if (affectees) *affectees = q.numRowsAffected(); return true; }
    const QString erreurPreparee = detailErreur(q);

    // 2) même requête avec les valeurs écrites dans le SQL
    QString litterale;
    int idx = 0;
    for (QChar c : sql) {
        if (c == '?' && idx < params.size()) litterale += litteral(params[idx++]);
        else litterale += c;
    }
    QSqlQuery q2(db);
    if (q2.exec(litterale)) {
        if (affectees) *affectees = q2.numRowsAffected();
        qWarning().noquote() << "Requête préparée refusée (" << erreurPreparee
                             << ") mais la version littérale a réussi.";
        return true;
    }

    QString erreur = detailErreur(q2);
    if (erreur.isEmpty() || erreur.contains("unknown error", Qt::CaseInsensitive))
        erreur = erreurPreparee;
    if (err)
        *err = messageErreur(erreur) + "\n\nRequête envoyée :\n" + litterale
               + "\n\n(Copiez cette requête dans SQL Developer pour voir l'erreur Oracle exacte.)";
    return false;
}


QString OracleTable::descriptionTable() const
{
    QString d = "Structure de la table " + m_nomComplet + " :\n";
    for (const InfoCol &i : m_infos) {
        d += "  - " + i.nom + "  " + i.type;
        if (i.type.contains("CHAR")) d += "(" + QString::number(i.longueur) + ")";
        d += i.obligatoire ? "  NOT NULL" : "";
        d += "\n";
    }
    return d;
}

QVariant OracleTable::valeurId(const QString &id) const
{
    if (m_idNumerique) {
        QString d = id;
        d.remove(QRegularExpression("[^0-9]"));
        return QVariant::fromValue<qlonglong>(d.toLongLong());
    }
    return id;
}

bool OracleTable::lister(QList<QVariantMap> &resultat, QString *err)
{
    resultat.clear();
    if (!init(err)) return false;

    QStringList cles;
    for (const ChampLogique &c : m_champs)
        if (m_colonnes.contains(c.cle)) cles << c.cle;
    if (!cles.contains("id")) cles.prepend("id");

    QStringList cols;
    for (const QString &k : cles) cols << m_colonnes[k];

    QSqlQuery q(Connection::createInstance().database());
    if (!q.exec(QString("SELECT %1 FROM %2 ORDER BY %3")
                    .arg(cols.join(", "), m_nomComplet, m_colonnes["id"]))) {
        if (err) *err = q.lastError().text();
        return false;
    }
    while (q.next()) {
        QVariantMap ligne;
        for (int i = 0; i < cles.size(); ++i)
            ligne[cles[i]] = q.value(i).toString();
        resultat.append(ligne);
    }
    return true;
}

QString OracleTable::prochainId(QString *err)
{
    if (!init(err)) return QString();
    QSqlQuery q(Connection::createInstance().database());
    if (!q.exec(QString("SELECT %1 FROM %2").arg(m_colonnes["id"], m_nomComplet))) {
        if (err) *err = q.lastError().text();
        return QString();
    }
    QString prefixe = m_prefixeDefaut;
    int largeur = 3;
    long long max = 0;
    bool formatTrouve = false;
    QRegularExpression re("^(\\D*)(\\d+)$");
    while (q.next()) {
        const QString v = q.value(0).toString();
        const auto m = re.match(v);
        if (!m.hasMatch()) continue;
        const long long n = m.captured(2).toLongLong();
        if (n >= max) {
            max = n;
            if (!m_idNumerique) {
                prefixe = m.captured(1);
                largeur = m.captured(2).size();
                formatTrouve = true;
            }
        }
    }
    Q_UNUSED(formatTrouve);
    if (m_idNumerique)
        return QString::number(max + 1);
    return QString("%1%2").arg(prefixe).arg(max + 1, largeur, 10, QChar('0'));
}

bool OracleTable::ajouter(const QVariantMap &valeurs, QString *idUtilise, QString *err)
{
    if (!init(err)) return false;

    QVariantMap v = valeurs;
    QString id = v.value("id").toString().trimmed();
    if (id.isEmpty()) {
        id = prochainId(err);
        if (id.isEmpty()) return false;
    }
    v["id"] = id;

    QStringList cols, marques;
    QVariantList params;
    for (const ChampLogique &c : m_champs) {
        if (!m_colonnes.contains(c.cle) || !v.contains(c.cle)) continue;
        cols << m_colonnes[c.cle];
        marques << "?";
        params << (c.cle == "id" ? valeurId(id) : v.value(c.cle));
    }
    if (!cols.contains(m_colonnes["id"])) {
        cols.prepend(m_colonnes["id"]);
        marques.prepend("?");
        params.prepend(valeurId(id));
    }

    // Vérification de la longueur des textes avant l'envoi
    for (int i = 0; i < cols.size(); ++i) {
        for (const InfoCol &info : m_infos) {
            if (info.nom.compare(cols[i], Qt::CaseInsensitive) != 0) continue;
            if (info.type.contains("CHAR") && params[i].userType() == QMetaType::QString
                && params[i].toString().toUtf8().size() > info.longueur) {
                if (err) *err = QString("La valeur « %1 » est trop longue pour la colonne %2 "
                                        "(maximum %3 caractères).")
                                    .arg(params[i].toString(), info.nom).arg(info.longueur);
                return false;
            }
        }
    }

    if (!executer(QString("INSERT INTO %1 (%2) VALUES (%3)")
                      .arg(m_nomComplet, cols.join(", "), marques.join(", ")), params, err)) {
        if (err) {
            QStringList manquantes;
            for (const InfoCol &info : m_infos) {
                bool fournie = false;
                for (const QString &c : cols)
                    if (c.compare(info.nom, Qt::CaseInsensitive) == 0) fournie = true;
                if (info.obligatoire && !fournie) manquantes << info.nom;
            }
            if (!manquantes.isEmpty())
                *err += "\n\nColonnes obligatoires (NOT NULL) non renseignées par l'application : "
                        + manquantes.join(", ");
            *err += "\n\n" + descriptionTable();
        }
        return false;
    }
    if (idUtilise) *idUtilise = id;
    return true;
}

bool OracleTable::modifier(const QString &ancienId, const QVariantMap &valeurs, QString *err)
{
    if (!init(err)) return false;

    QStringList sets;
    QVariantList params;
    for (const ChampLogique &c : m_champs) {
        if (!m_colonnes.contains(c.cle) || !valeurs.contains(c.cle)) continue;
        sets << m_colonnes[c.cle] + " = ?";
        params << (c.cle == "id" ? valeurId(valeurs.value(c.cle).toString()) : valeurs.value(c.cle));
    }
    if (sets.isEmpty()) return true;

    params << valeurId(ancienId);
    return executer(QString("UPDATE %1 SET %2 WHERE %3 = ?")
                        .arg(m_nomComplet, sets.join(", "), m_colonnes["id"]), params, err);
}

// Supprime les lignes (table, col = val) après avoir supprimé récursivement les lignes
// des tables qui les référencent (clés étrangères simples).
static bool supprimerLignes(QSqlDatabase db, const QString &owner, const QString &table,
                            const QString &col, const QVariant &val, int profondeur, QString *err)
{
    if (profondeur > 5) { *err = "Dépendances trop profondes (relation circulaire ?)."; return false; }
    const QString cible = QString("\"%1\".\"%2\"").arg(owner, table);

    struct Enfant { QString owner, table, cons, pk; };
    QList<Enfant> enfants;
    {
        QSqlQuery fk(db);
        fk.prepare("SELECT f.owner, f.table_name, f.constraint_name, p.constraint_name "
                   "FROM all_constraints f JOIN all_constraints p "
                   "ON f.r_owner = p.owner AND f.r_constraint_name = p.constraint_name "
                   "WHERE f.constraint_type = 'R' AND p.owner = ? AND p.table_name = ?");
        fk.addBindValue(owner);
        fk.addBindValue(table);
        if (!fk.exec()) { *err = detailErreur(fk); return false; }
        while (fk.next())
            enfants.append({fk.value(0).toString(), fk.value(1).toString(),
                            fk.value(2).toString(), fk.value(3).toString()});
    }

    auto colonnes = [&db](const QString &o, const QString &cons) {
        QStringList l;
        QSqlQuery q(db);
        q.prepare("SELECT column_name FROM all_cons_columns "
                  "WHERE owner = ? AND constraint_name = ? ORDER BY position");
        q.addBindValue(o);
        q.addBindValue(cons);
        if (q.exec()) while (q.next()) l << q.value(0).toString();
        return l;
    };

    for (const Enfant &e : enfants) {
        const QStringList colFk = colonnes(e.owner, e.cons);
        const QStringList colRef = colonnes(owner, e.pk);
        if (colFk.size() != 1 || colRef.size() != 1) continue;   // clé composée : non gérée

        QSqlQuery sel(db);
        sel.prepare(QString("SELECT \"%1\" FROM %2 WHERE \"%3\" = ?").arg(colRef[0], cible, col));
        sel.addBindValue(val);
        if (!sel.exec()) { *err = detailErreur(sel); return false; }
        QVariantList valeurs;
        while (sel.next()) valeurs << sel.value(0);

        for (const QVariant &v : valeurs)
            if (!supprimerLignes(db, e.owner, e.table, colFk[0], v, profondeur + 1, err))
                return false;
    }

    QSqlQuery del(db);
    del.prepare(QString("DELETE FROM %1 WHERE \"%2\" = ?").arg(cible, col));
    del.addBindValue(val);
    if (!del.exec()) { *err = detailErreur(del); return false; }
    return true;
}

bool OracleTable::supprimerEnCascade(const QString &id, QString *err)
{
    QSqlDatabase db = Connection::createInstance().database();
    const bool transaction = db.transaction();
    QString e;
    if (supprimerLignes(db, m_owner, m_table, m_colonnes["id"], valeurId(id), 0, &e)) {
        if (transaction) db.commit();
        return true;
    }
    if (transaction) db.rollback();
    if (err) *err = messageErreur(e);
    return false;
}

bool OracleTable::supprimer(const QString &id, QString *err, bool cascade)
{
    if (!init(err)) return false;
    if (cascade) return supprimerEnCascade(id, err);

    int affectees = -1;
    if (!executer(QString("DELETE FROM %1 WHERE %2 = ?").arg(m_nomComplet, m_colonnes["id"]),
                  {valeurId(id)}, err, &affectees))
        return false;
    if (affectees == 0) {
        if (err) *err = QString("Aucune ligne supprimée : l'identifiant « %1 » n'existe pas dans %2.")
                            .arg(id, m_nomComplet);
        return false;
    }
    return true;
}
