/* ============================================================
 *  Pages EMPLOYÉS et SALLES reliées à Oracle
 *  (tables EMPLOYES et SALLE ; colonnes détectées automatiquement)
 * ============================================================ */
#include "integration.h"
#include "ui_integration.h"
#include "oracletable.h"

#include <QMessageBox>
#include <QHeaderView>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QColor>
#include <QTimer>
#include <QDebug>
#include <algorithm>
#include <functional>

namespace {

OracleTable &tblEmployes()
{
    static OracleTable t("EMPLOYES", {
        {"id",     {"^ID"}},
        {"nom",    {"^NOM"}},
        {"prenom", {"^PRENOM"}},
        {"mdp",    {"MOT_?DE_?PASSE", "PASSWORD", "^MDP", "^MOT_DE"}},
        {"role",   {"^ROLE"}},
        {"statut", {"^STATUT", "^ETAT"}},
    }, "EMP-");
    return t;
}

OracleTable &tblSalles()
{
    static OracleTable t("SALLE", {
        {"id",       {"^ID"}},
        {"nom",      {"^NOM", "^LIBELLE"}},
        {"capacite", {"^CAPACIT"}},
        {"type",     {"^TYPE"}},
        {"dispo",    {"^DISPO", "^STATUT", "^ETAT"}},
    }, "SAL-");
    return t;
}

const char *kStyleModifier = R"(
    QPushButton { background-color:#4338CA; color:#FFFFFF; border:none; border-radius:6px;
                  font-size:10px; font-weight:800; padding:6px 12px; min-width:90px; }
    QPushButton:hover { background-color:#6366F1; })";

const char *kStyleSupprimer = R"(
    QPushButton { background-color:#B91C1C; color:#FFFFFF; border:none; border-radius:6px;
                  font-size:10px; font-weight:800; padding:6px 12px; min-width:90px; }
    QPushButton:hover { background-color:#F87171; })";

QWidget *creerActions(QObject *ctx, std::function<void()> modif, std::function<void()> suppr)
{
    auto *container = new QWidget;
    auto *lay = new QHBoxLayout(container);
    lay->setContentsMargins(4, 2, 4, 2);
    lay->setSpacing(8);

    auto *btnModif = new QPushButton("MODIFIER");
    btnModif->setCursor(Qt::PointingHandCursor);
    btnModif->setStyleSheet(kStyleModifier);
    btnModif->setMinimumWidth(95);

    auto *btnSuppr = new QPushButton("SUPPRIMER");
    btnSuppr->setCursor(Qt::PointingHandCursor);
    btnSuppr->setStyleSheet(kStyleSupprimer);
    btnSuppr->setMinimumWidth(95);

    lay->addWidget(btnModif);
    lay->addWidget(btnSuppr);
    lay->addStretch();

    QObject::connect(btnModif, &QPushButton::clicked, ctx, [modif]() { modif(); });
    QObject::connect(btnSuppr, &QPushButton::clicked, ctx, [suppr]() { suppr(); });
    return container;
}

void configurerTable(QTableWidget *t)
{
    t->verticalHeader()->setVisible(false);
    t->setEditTriggers(QAbstractItemView::NoEditTriggers);
    t->setSelectionBehavior(QAbstractItemView::SelectRows);
    t->setAlternatingRowColors(true);
    const int lastCol = t->columnCount() - 1;
    for (int c = 0; c < lastCol; ++c)
        t->horizontalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);
    if (lastCol >= 0) {
        t->horizontalHeader()->setSectionResizeMode(lastCol, QHeaderView::Fixed);
        t->setColumnWidth(lastCol, 240);
    }
    t->verticalHeader()->setDefaultSectionSize(46);
}

QVariantMap ligne(const QList<QVariantMap> &liste, const QString &id)
{
    for (const QVariantMap &m : liste)
        if (m.value("id").toString() == id) return m;
    return {};
}

void filtrerTable(QTableWidget *t, const QString &texte, const QList<int> &colonnes)
{
    const QString q = texte.trimmed();
    for (int r = 0; r < t->rowCount(); ++r) {
        bool ok = q.isEmpty();
        for (int c : colonnes)
            if (t->item(r, c) && t->item(r, c)->text().contains(q, Qt::CaseInsensitive)) ok = true;
        t->setRowHidden(r, !ok);
    }
}

// Supprime dans Oracle ; propose de supprimer aussi les données liées (ORA-02292).
bool supprimerAvecLiens(QWidget *parent, OracleTable &t, const QString &id, const QString &quoi)
{
    QString err;
    bool ok = t.supprimer(id, &err);
    if (!ok && err.contains("02292")) {
        if (QMessageBox::question(parent, "Données liées",
                QString("%1 est utilisé dans d'autres tables.\n\nSupprimer aussi ces données liées ?")
                    .arg(quoi)) != QMessageBox::Yes)
            return false;
        err.clear();
        ok = t.supprimer(id, &err, true);
    }
    if (!ok) {
        QMessageBox::critical(parent, "Erreur Oracle", "Suppression impossible :\n" + err);
        return false;
    }
    return true;
}

bool exigerOracle(QWidget *parent, bool actif)
{
    if (actif) return true;
    QMessageBox::warning(parent, "Oracle non connecté",
        "Cette action nécessite la connexion à la base Oracle.\n"
        "Les données affichées sont des données de démonstration.\n\n"
        "Voir GUIDE_ORACLE.md.");
    return false;
}

} // namespace

/* ============================================================
 *  EMPLOYÉS
 * ============================================================ */
void integration::remplirTableEmployes()
{
    configurerTable(ui->tableEmployes);
    ui->lineEditEmpID->setReadOnly(true);          // identifiant généré automatiquement
    ui->lineEditEmpID->setPlaceholderText("Auto");
    m_empBtnTexte = ui->btnAjouterEmploye->text();

    connect(ui->btnAjouterEmploye,    &QPushButton::clicked, this, &integration::onAjouterEmploye);
    connect(ui->btn_Annuler,          &QPushButton::clicked, this, &integration::onAnnulerEmploye);
    connect(ui->btnRechercherEmploye, &QPushButton::clicked, this, &integration::onRechercherEmploye);
    connect(ui->lineEdit,             &QLineEdit::textChanged, this, &integration::onRechercherEmploye);
    connect(ui->btnTrierEmploye,      &QPushButton::clicked, this, &integration::onTrierEmploye);
    connect(ui->btnExporterEmploye,   &QPushButton::clicked, this, &integration::onExporterEmploye);

    chargerEmployes();
}

void integration::chargerEmployes()
{
    QString err;
    QList<QVariantMap> data;
    m_dbEmp = tblEmployes().lister(data, &err);

    if (!m_dbEmp) {
        qWarning().noquote() << "Employés : mode démonstration —" << err;
        const QStringList noms    = {"Benali", "Trabelsi", "Gharbi", "Mansouri", "Khelifi", "Jebali"};
        const QStringList prenoms = {"Ahmed", "Sarra", "Yassine", "Leila", "Mehdi", "Nour"};
        for (int i = 0; i < noms.size(); ++i) {
            QVariantMap m;
            m["id"] = QString("EMP-%1").arg(i + 1, 3, 10, QChar('0'));
            m["nom"] = noms[i];
            m["prenom"] = prenoms[i];
            m["mdp"] = "demo";
            m["role"] = (i % 2 == 0) ? "Administrateur" : "Responsable RH";
            m["statut"] = (i % 4 == 0) ? "Inactif" : "Actif";
            data.append(m);
        }
    }
    m_emp = data;
    afficherEmployes();
}

void integration::afficherEmployes()
{
    QList<QVariantMap> data = m_emp;
    if (m_empTri != 0) {
        std::sort(data.begin(), data.end(), [this](const QVariantMap &a, const QVariantMap &b) {
            const int c = QString::localeAwareCompare(a.value("nom").toString(), b.value("nom").toString());
            return m_empTri == 1 ? c < 0 : c > 0;
        });
    }

    auto *t = ui->tableEmployes;
    t->setRowCount(0);
    t->setRowCount(data.size());
    for (int i = 0; i < data.size(); ++i) {
        const QVariantMap &m = data[i];
        const QString id = m.value("id").toString();
        QString statut = m.value("statut").toString();
        if (statut.isEmpty()) statut = "Actif";

        t->setItem(i, 0, new QTableWidgetItem(id));
        t->setItem(i, 1, new QTableWidgetItem(m.value("nom").toString()));
        t->setItem(i, 2, new QTableWidgetItem(m.value("prenom").toString()));
        t->setItem(i, 3, new QTableWidgetItem("••••••••"));
        t->setItem(i, 4, new QTableWidgetItem(m.value("role").toString()));
        auto *itemStatut = new QTableWidgetItem(statut);
        itemStatut->setForeground(statut == "Actif" ? QColor("#34D399") : QColor("#F87171"));
        t->setItem(i, 5, itemStatut);
        t->setCellWidget(i, 6, creerActions(this,
                                            [this, id]() { modifierEmploye(id); },
                                            [this, id]() { supprimerEmploye(id); }));
    }
    onRechercherEmploye();
    dessinerStatistiques();
}

void integration::modifierEmploye(const QString &id)
{
    const QVariantMap m = ligne(m_emp, id);
    if (m.isEmpty()) return;
    m_empIdEdition = id;
    ui->lineEditEmpID->setText(id);
    ui->lineEditEmpNom->setText(m.value("nom").toString());
    ui->lineEditEmpPrenom->setText(m.value("prenom").toString());
    ui->lineEditEmpMdp->setText(m.value("mdp").toString());
    const int r = ui->comboBoxEmpRole->findText(m.value("role").toString());
    if (r >= 0) ui->comboBoxEmpRole->setCurrentIndex(r);
    const int s = ui->comboBoxEmpStatut->findText(m.value("statut").toString());
    if (s >= 0) ui->comboBoxEmpStatut->setCurrentIndex(s);
    ui->btnAjouterEmploye->setText("ENREGISTRER");
}

void integration::supprimerEmploye(const QString &id)
{
    if (!exigerOracle(this, m_dbEmp)) return;
    if (!supprimerAvecLiens(this, tblEmployes(), id, "Cet employé")) return;
    if (m_empIdEdition == id) onAnnulerEmploye();
    QTimer::singleShot(0, this, [this]() { chargerEmployes(); });
}

void integration::onAjouterEmploye()
{
    const QString nom = ui->lineEditEmpNom->text().trimmed();
    const QString prenom = ui->lineEditEmpPrenom->text().trimmed();
    const QString mdp = ui->lineEditEmpMdp->text();

    if (nom.isEmpty() || prenom.isEmpty()) {
        QMessageBox::warning(this, "Champs obligatoires", "Veuillez saisir le nom et le prénom.");
        return;
    }
    if (m_empIdEdition.isEmpty() && mdp.isEmpty()) {
        QMessageBox::warning(this, "Champs obligatoires", "Veuillez saisir un mot de passe.");
        return;
    }
    if (!exigerOracle(this, m_dbEmp)) return;

    QVariantMap v;
    v["nom"] = nom;
    v["prenom"] = prenom;
    if (!mdp.isEmpty()) v["mdp"] = mdp;
    v["role"] = ui->comboBoxEmpRole->currentText();
    v["statut"] = ui->comboBoxEmpStatut->currentText();

    QString err;
    const bool ok = m_empIdEdition.isEmpty()
                        ? tblEmployes().ajouter(v, nullptr, &err)
                        : tblEmployes().modifier(m_empIdEdition, v, &err);
    if (!ok) {
        QMessageBox::critical(this, "Erreur Oracle", "Enregistrement impossible :\n" + err);
        return;
    }
    onAnnulerEmploye();
    chargerEmployes();
}

void integration::onAnnulerEmploye()
{
    ui->lineEditEmpID->clear();
    ui->lineEditEmpNom->clear();
    ui->lineEditEmpPrenom->clear();
    ui->lineEditEmpMdp->clear();
    ui->comboBoxEmpRole->setCurrentIndex(0);
    ui->comboBoxEmpStatut->setCurrentIndex(0);
    ui->tableEmployes->clearSelection();
    m_empIdEdition.clear();
    ui->btnAjouterEmploye->setText(m_empBtnTexte);
}

void integration::onRechercherEmploye()
{
    filtrerTable(ui->tableEmployes, ui->lineEdit->text(), {0, 1, 2, 4, 5});
}

void integration::onTrierEmploye()
{
    m_empTri = (m_empTri == 1) ? 2 : 1;
    afficherEmployes();
}

void integration::onExporterEmploye()
{
    const QString path = QFileDialog::getSaveFileName(this, "Exporter les employés",
                                                      "employes.csv", "Fichier CSV (*.csv)");
    if (path.isEmpty()) return;
    QFile f(path);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Export impossible", "Impossible de créer le fichier.");
        return;
    }
    QTextStream out(&f);
    out << "ID;Nom;Prénom;Rôle;Statut\n";
    for (const QVariantMap &m : m_emp)
        out << m.value("id").toString() << ';' << m.value("nom").toString() << ';'
            << m.value("prenom").toString() << ';' << m.value("role").toString() << ';'
            << m.value("statut").toString() << '\n';
    QMessageBox::information(this, "Export terminé", "La liste a été exportée au format CSV.");
}

/* ============================================================
 *  SALLES
 * ============================================================ */
void integration::remplirTableSalles()
{
    configurerTable(ui->tableSalles);
    ui->lineEditSalleID->setReadOnly(true);        // identifiant généré automatiquement
    ui->lineEditSalleID->setPlaceholderText("Auto");
    m_salleBtnTexte = ui->btnAjouterSalle->text();

    connect(ui->btnAjouterSalle,    &QPushButton::clicked, this, &integration::onAjouterSalle);
    connect(ui->btnAnnuler,         &QPushButton::clicked, this, &integration::onAnnulerSalle);
    connect(ui->btnRechercherSalle, &QPushButton::clicked, this, &integration::onRechercherSalle);
    connect(ui->lineEdit_2,         &QLineEdit::textChanged, this, &integration::onRechercherSalle);
    connect(ui->btnTrierSalle,      &QPushButton::clicked, this, &integration::onTrierSalle);
    connect(ui->btnExporterSalle,   &QPushButton::clicked, this, &integration::onExporterSalle);

    chargerSalles();
}

void integration::chargerSalles()
{
    QString err;
    QList<QVariantMap> data;
    m_dbSalle = tblSalles().lister(data, &err);

    if (!m_dbSalle) {
        qWarning().noquote() << "Salles : mode démonstration —" << err;
        const QStringList types = {"Salle informatique", "Salle de réunion", "Salle conférence", "Laboratoire"};
        const QStringList dispos = {"Disponible", "Occupée", "Maintenance"};
        for (int i = 0; i < 8; ++i) {
            QVariantMap m;
            m["id"] = QString("SAL-%1").arg(i + 1, 3, 10, QChar('0'));
            m["nom"] = QString("Salle %1").arg(i + 100);
            m["capacite"] = QString::number(30 + i * 20);
            m["type"] = types[i % types.size()];
            m["dispo"] = dispos[i % dispos.size()];
            data.append(m);
        }
    }
    m_salles = data;
    afficherSalles();
}

void integration::afficherSalles()
{
    QList<QVariantMap> data = m_salles;
    if (m_salleTri != 0) {
        std::sort(data.begin(), data.end(), [this](const QVariantMap &a, const QVariantMap &b) {
            const int c = QString::localeAwareCompare(a.value("nom").toString(), b.value("nom").toString());
            return m_salleTri == 1 ? c < 0 : c > 0;
        });
    }

    auto *t = ui->tableSalles;
    t->setRowCount(0);
    t->setRowCount(data.size());
    for (int i = 0; i < data.size(); ++i) {
        const QVariantMap &m = data[i];
        const QString id = m.value("id").toString();
        QString dispo = m.value("dispo").toString();
        if (dispo.isEmpty()) dispo = "Disponible";

        t->setItem(i, 0, new QTableWidgetItem(id));
        t->setItem(i, 1, new QTableWidgetItem(m.value("nom").toString()));
        t->setItem(i, 2, new QTableWidgetItem(m.value("capacite").toString()));
        t->setItem(i, 3, new QTableWidgetItem(m.value("type").toString()));
        auto *itemDispo = new QTableWidgetItem(dispo);
        if (dispo == "Disponible")   itemDispo->setForeground(QColor("#34D399"));
        else if (dispo == "Occupée") itemDispo->setForeground(QColor("#FBBF24"));
        else                         itemDispo->setForeground(QColor("#F87171"));
        t->setItem(i, 4, itemDispo);
        t->setCellWidget(i, 5, creerActions(this,
                                            [this, id]() { modifierSalle(id); },
                                            [this, id]() { supprimerSalle(id); }));
    }
    onRechercherSalle();
    dessinerStatistiquesSalles();
}

void integration::modifierSalle(const QString &id)
{
    const QVariantMap m = ligne(m_salles, id);
    if (m.isEmpty()) return;
    m_salleIdEdition = id;
    ui->lineEditSalleID->setText(id);
    ui->lineEditSalleNom->setText(m.value("nom").toString());
    ui->spinBoxSalleCapacite->setValue(m.value("capacite").toInt());
    const int t = ui->comboBoxSalleType->findText(m.value("type").toString());
    if (t >= 0) ui->comboBoxSalleType->setCurrentIndex(t);
    const int d = ui->comboBoxSalleDispo->findText(m.value("dispo").toString());
    if (d >= 0) ui->comboBoxSalleDispo->setCurrentIndex(d);
    ui->btnAjouterSalle->setText("ENREGISTRER");
}

void integration::supprimerSalle(const QString &id)
{
    if (!exigerOracle(this, m_dbSalle)) return;
    if (!supprimerAvecLiens(this, tblSalles(), id, "Cette salle")) return;
    if (m_salleIdEdition == id) onAnnulerSalle();
    QTimer::singleShot(0, this, [this]() { chargerSalles(); });
}

void integration::onAjouterSalle()
{
    const QString nom = ui->lineEditSalleNom->text().trimmed();
    if (nom.isEmpty()) {
        QMessageBox::warning(this, "Champs obligatoires", "Veuillez saisir le nom de la salle.");
        return;
    }
    if (!exigerOracle(this, m_dbSalle)) return;

    QVariantMap v;
    v["nom"] = nom;
    v["capacite"] = ui->spinBoxSalleCapacite->value();
    v["type"] = ui->comboBoxSalleType->currentText();
    v["dispo"] = ui->comboBoxSalleDispo->currentText();

    QString err;
    const bool ok = m_salleIdEdition.isEmpty()
                        ? tblSalles().ajouter(v, nullptr, &err)
                        : tblSalles().modifier(m_salleIdEdition, v, &err);
    if (!ok) {
        QMessageBox::critical(this, "Erreur Oracle", "Enregistrement impossible :\n" + err);
        return;
    }
    onAnnulerSalle();
    chargerSalles();
}

void integration::onAnnulerSalle()
{
    ui->lineEditSalleID->clear();
    ui->lineEditSalleNom->clear();
    ui->spinBoxSalleCapacite->setValue(ui->spinBoxSalleCapacite->minimum());
    ui->comboBoxSalleType->setCurrentIndex(0);
    ui->comboBoxSalleDispo->setCurrentIndex(0);
    ui->tableSalles->clearSelection();
    m_salleIdEdition.clear();
    ui->btnAjouterSalle->setText(m_salleBtnTexte);
}

void integration::onRechercherSalle()
{
    filtrerTable(ui->tableSalles, ui->lineEdit_2->text(), {0, 1, 2, 3, 4});
}

void integration::onTrierSalle()
{
    m_salleTri = (m_salleTri == 1) ? 2 : 1;
    afficherSalles();
}

void integration::onExporterSalle()
{
    const QString path = QFileDialog::getSaveFileName(this, "Exporter les salles",
                                                      "salles.csv", "Fichier CSV (*.csv)");
    if (path.isEmpty()) return;
    QFile f(path);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Export impossible", "Impossible de créer le fichier.");
        return;
    }
    QTextStream out(&f);
    out << "ID;Nom;Capacité;Type;Disponibilité\n";
    for (const QVariantMap &m : m_salles)
        out << m.value("id").toString() << ';' << m.value("nom").toString() << ';'
            << m.value("capacite").toString() << ';' << m.value("type").toString() << ';'
            << m.value("dispo").toString() << '\n';
    QMessageBox::information(this, "Export terminé", "La liste a été exportée au format CSV.");
}
