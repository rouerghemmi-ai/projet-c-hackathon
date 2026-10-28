#include "integration.h"
#include "ui_integration.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>
#include <QSqlRecord>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QDateTime>

#include <QPushButton>
#include <QStackedWidget>
#include <QMessageBox>
#include <QInputDialog>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTextBrowser>
#include <QScrollArea>
#include <QScrollBar>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QFrame>
#include <QColor>
#include <QPainter>
#include <QPainterPath>
#include <QMap>

/* ============================================================
 *  CONSTRUCTEUR / DESTRUCTEUR
 * ============================================================ */
integration::integration(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::integration)
{
    ui->setupUi(this);

    // ---------------------------------------------------------
    // Connexion Oracle via ODBC (DSN HACKNOVA)
    // ---------------------------------------------------------
    QSqlDatabase db;
    if (QSqlDatabase::contains("hacknova_connection")) {
        db = QSqlDatabase::database("hacknova_connection");
    } else {
        db = QSqlDatabase::addDatabase("QODBC", "hacknova_connection");
        db.setDatabaseName("HACKNOVA");
        db.setUserName("rayen");
        db.setPassword("rayen");
    }

    if (!db.isOpen()) {
        if (db.open()) {
            qDebug() << "Connexion Oracle réussie depuis Qt.";
        } else {
            qDebug() << "Erreur de connexion Oracle :"
                     << db.lastError().text();
            QMessageBox::critical(
                this,
                "Connexion Oracle",
                "Impossible de se connecter à la base Oracle :\n" +
                    db.lastError().text() +
                    "\n\nLes tableaux resteront vides."
                );
        }
    }

    ui->stackedWidget->setCurrentIndex(0);

    appliquerTheme();
    connecterLogin();
    connecterNavigation();
    connecterChatbot();
    connecterActions();

    remplirTableSalles();
    dessinerStatistiquesSalles();
    remplirTableEmployes();
    dessinerStatistiques();
}

integration::~integration()
{
    if (QSqlDatabase::contains("hacknova_connection")) {
        {
            QSqlDatabase db = QSqlDatabase::database("hacknova_connection", false);
            if (db.isOpen()) db.close();
        }
        QSqlDatabase::removeDatabase("hacknova_connection");
    }
    delete ui;
}

/* ============================================================
 *  HELPERS
 * ============================================================ */
namespace {
QSqlDatabase dbHacknova()
{
    QSqlDatabase db = QSqlDatabase::database("hacknova_connection", false);
    if (db.isValid() && !db.isOpen()) {
        if (!db.open())
            qDebug() << "Réouverture Oracle échouée :" << db.lastError().text();
        else
            qDebug() << "Connexion Oracle réouverte.";
    }
    return db;
}

QString sqlStr(const QVariant &v)
{
    return v.isNull() ? QString() : v.toString();
}

QString normaliserDisponibilite(const QString &brute)
{
    const QString v = brute.trimmed();
    if (v.compare("Disponible", Qt::CaseInsensitive) == 0)
        return "Disponible";
    if (v.compare("Indisponible", Qt::CaseInsensitive) == 0)
        return "Indisponible";
    return "Indisponible";
}
} // namespace

/* ============================================================
 *  REMPLIR LE TABLEAU DES EMPLOYÉS
 * ============================================================ */
void integration::remplirTableEmployes()
{
    auto *t = ui->tableEmployes;
    t->clearContents();
    t->setRowCount(0);

    t->verticalHeader()->setVisible(false);
    t->setEditTriggers(QAbstractItemView::NoEditTriggers);
    t->setSelectionBehavior(QAbstractItemView::SelectRows);
    t->setAlternatingRowColors(true);

    int lastCol = t->columnCount() - 1;
    for (int c = 0; c < lastCol; ++c)
        t->horizontalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);
    if (lastCol >= 0) {
        t->horizontalHeader()->setSectionResizeMode(lastCol, QHeaderView::Fixed);
        t->setColumnWidth(lastCol, 190);
    }
    t->verticalHeader()->setDefaultSectionSize(42);
}

/* ============================================================
 *  DESSINER LES STATISTIQUES (Employés)
 * ============================================================ */
void integration::dessinerStatistiques()
{
    {
        QPixmap pix(210, 210);
        pix.fill(Qt::transparent);
        QPainter p(&pix);
        p.setRenderHint(QPainter::Antialiasing);

        p.setBrush(QColor("#334155"));
        p.setPen(Qt::NoPen);
        p.drawPie(QRectF(10, 20, 130, 130), 0, 360 * 16);

        QFont f = p.font();
        f.setFamily("Segoe UI");
        f.setPointSize(9);
        f.setBold(true);
        p.setFont(f);

        p.setPen(QColor("#94A3B8"));
        p.drawText(155, 48, "Actif (0)");
        p.drawText(155, 83, "Inactif (0)");

        f.setPointSize(8);
        f.setBold(false);
        p.setFont(f);
        p.setPen(QColor("#64748B"));
        p.drawText(10, 195, "Répartition par statut");
        p.end();
        ui->labelStatutPie->setPixmap(pix);
    }

    {
        QPixmap pix(250, 210);
        pix.fill(Qt::transparent);
        QPainter p(&pix);
        p.setRenderHint(QPainter::Antialiasing);

        QFont f = p.font();
        f.setFamily("Segoe UI");
        f.setPointSize(9);
        f.setBold(true);
        p.setFont(f);
        p.setPen(QColor("#64748B"));
        p.drawText(10, 100, "Aucun employé enregistré");

        f.setPointSize(8);
        f.setBold(false);
        p.setFont(f);
        p.drawText(10, 195, "Nombre d'employés par rôle");
        p.end();
        ui->labelRoleBar->setPixmap(pix);
    }
}

/* ============================================================
 *  CONNEXIONS DES ACTIONS
 * ============================================================ */
void integration::connecterActions()
{
    if (ui->btnSuggerer_2)
        QObject::connect(ui->btnSuggerer_2, &QPushButton::clicked,
                         this, &integration::onSuggererSalle);

    if (ui->btnValiderQR_4)
        QObject::connect(ui->btnValiderQR_4, &QPushButton::clicked,
                         this, &integration::onValiderQR);

    if (ui->btnAjouterSalle)
        QObject::connect(ui->btnAjouterSalle, &QPushButton::clicked,
                         this, &integration::onAjouterSalle);
    if (ui->btnAnnuler)
        QObject::connect(ui->btnAnnuler, &QPushButton::clicked,
                         this, &integration::onAnnulerSalle);
    if (ui->btnRechercherSalle)
        QObject::connect(ui->btnRechercherSalle, &QPushButton::clicked,
                         this, &integration::onRechercherSalle);
    if (ui->btnTrierSalle)
        QObject::connect(ui->btnTrierSalle, &QPushButton::clicked,
                         this, &integration::onTrierSalle);
    if (ui->btnExporterSalle)
        QObject::connect(ui->btnExporterSalle, &QPushButton::clicked,
                         this, &integration::onExporterSalle);
}

/* ============================================================
 *  RAFRAÎCHISSEMENT SALLES + STATS
 * ============================================================ */
void integration::rafraichirSallesEtStats()
{
    remplirTableSalles();
    dessinerStatistiquesSalles();
}

/* ============================================================
 *  VIDER LA TABLE DES SALLES PROPREMENT
 * ============================================================ */
void integration::viderTableSalles()
{
    auto *t = ui->tableSalles;
    t->setSortingEnabled(false);
    for (int i = t->rowCount() - 1; i >= 0; --i) {
        QWidget *w = t->cellWidget(i, 5);
        if (w) {
            t->removeCellWidget(i, 5);
            w->deleteLater();
        }
    }
    t->clearContents();
    t->setRowCount(0);
}

/* ============================================================
 *  CRÉATION D'UN WIDGET D'ACTIONS (MODIFIER / SUPPRIMER)
 * ============================================================ */
QWidget *integration::creerWidgetActions(qint64 idSalle, const QString &nomSalle)
{
    static const QString styleModifier = R"(
        QPushButton {
            background-color: #4338CA; color: #FFFFFF;
            border: 1px solid #6366F1; border-radius: 6px;
            font-size: 11px; font-weight: 700;
            padding: 4px 8px; min-width: 65px; min-height: 26px;
        }
        QPushButton:hover { background-color: #6366F1; border-color: #818CF8; }
        QPushButton:pressed { background-color: #3730A3; }
    )";
    static const QString styleSupprimer = R"(
        QPushButton {
            background-color: #DC2626; color: #FFFFFF;
            border: 1px solid #EF4444; border-radius: 6px;
            font-size: 11px; font-weight: 700;
            padding: 4px 8px; min-width: 65px; min-height: 26px;
        }
        QPushButton:hover { background-color: #EF4444; border-color: #F87171; }
        QPushButton:pressed { background-color: #B91C1C; }
    )";

    auto *container = new QWidget;
    auto *lay = new QHBoxLayout(container);
    lay->setContentsMargins(4, 2, 4, 2);
    lay->setSpacing(6);

    auto *btnModif = new QPushButton("MODIFIER", container);
    btnModif->setCursor(Qt::PointingHandCursor);
    btnModif->setStyleSheet(styleModifier);

    auto *btnSuppr = new QPushButton("SUPPRIMER", container);
    btnSuppr->setCursor(Qt::PointingHandCursor);
    btnSuppr->setStyleSheet(styleSupprimer);

    lay->addWidget(btnModif);
    lay->addWidget(btnSuppr);
    lay->addStretch();

    QObject::connect(btnModif, &QPushButton::clicked, this,
                     [this, idSalle]() { ouvrirModificationSalle(idSalle); });
    QObject::connect(btnSuppr, &QPushButton::clicked, this,
                     [this, idSalle, nomSalle]() {
                         ouvrirSuppressionSalle(idSalle, nomSalle);
                     });

    return container;
}

/* ============================================================
 *  REMPLIR LE TABLEAU DES SALLES
 * ============================================================ */
void integration::remplirTableSalles()
{
    auto *t = ui->tableSalles;
    viderTableSalles();

    QSqlDatabase db = dbHacknova();
    if (!db.isValid() || !db.isOpen()) {
        qDebug() << "remplirTableSalles : connexion Oracle indisponible.";
        return;
    }

    QSqlQuery q(db);
    if (!q.exec("SELECT ID_SALLE, NOM_SALLE, CAPACITE, TYPE_SALLE, DISPONIBILITE "
                "FROM SALLE ORDER BY ID_SALLE")) {
        qDebug() << "remplirTableSalles : erreur SQL :"
                 << q.lastError().text();
        QMessageBox::warning(this, "Salles",
                             "Impossible de charger les salles depuis Oracle :\n" +
                                 q.lastError().text());
        return;
    }

    int row = 0;
    while (q.next()) {
        const qint64 id     = q.value(0).toLongLong();
        const QString nom   = sqlStr(q.value(1));
        const QString cap   = sqlStr(q.value(2));
        const QString type  = sqlStr(q.value(3));
        const QString dispo = sqlStr(q.value(4));

        t->insertRow(row);
        t->setItem(row, 0, new QTableWidgetItem(QString::number(id)));
        t->setItem(row, 1, new QTableWidgetItem(nom));
        t->setItem(row, 2, new QTableWidgetItem(cap));
        t->setItem(row, 3, new QTableWidgetItem(type));

        auto *itemDispo = new QTableWidgetItem(dispo);
        if (dispo.compare("Disponible", Qt::CaseInsensitive) == 0)
            itemDispo->setForeground(QColor("#10B981"));
        else
            itemDispo->setForeground(QColor("#EF4444"));
        t->setItem(row, 4, itemDispo);

        t->setCellWidget(row, 5, creerWidgetActions(id, nom));
        ++row;
    }

    t->verticalHeader()->setVisible(false);
    t->setEditTriggers(QAbstractItemView::NoEditTriggers);
    t->setSelectionBehavior(QAbstractItemView::SelectRows);
    t->setAlternatingRowColors(true);

    int lastCol = t->columnCount() - 1;
    for (int c = 0; c < lastCol; ++c)
        t->horizontalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);
    if (lastCol >= 0) {
        t->horizontalHeader()->setSectionResizeMode(lastCol, QHeaderView::Fixed);
        t->setColumnWidth(lastCol, 190);
    }
    t->verticalHeader()->setDefaultSectionSize(42);

    t->setSortingEnabled(true);
}

/* ============================================================
 *  STATISTIQUES SALLES
 * ============================================================ */
void integration::dessinerStatistiquesSalles()
{
    {
        QPixmap pix(190, 190);
        pix.fill(Qt::transparent);
        QPainter p(&pix);
        p.setRenderHint(QPainter::Antialiasing);

        int dispo = 0, indispo = 0;
        for (int i = 0; i < ui->tableSalles->rowCount(); ++i) {
            QTableWidgetItem *it = ui->tableSalles->item(i, 4);
            if (!it) continue;
            if (it->text().compare("Disponible", Qt::CaseInsensitive) == 0) dispo++;
            else indispo++;
        }
        int total = dispo + indispo;

        QRectF rect(10, 15, 110, 110);

        if (total == 0) {
            p.setBrush(QColor("#334155"));
            p.setPen(Qt::NoPen);
            p.drawEllipse(rect);
        } else {
            double startAngle = 90.0;
            auto drawSlice = [&](int val, const QColor &color) {
                if (val <= 0) return;
                double sweep = -360.0 * val / total;
                p.setBrush(color);
                p.setPen(Qt::NoPen);
                p.drawPie(rect, int(startAngle * 16), int(sweep * 16));
                startAngle += sweep;
            };
            drawSlice(dispo,   QColor("#10B981"));
            drawSlice(indispo, QColor("#EF4444"));
        }

        QFont f = p.font();
        f.setFamily("Segoe UI");
        f.setPointSize(8);
        f.setBold(true);
        p.setFont(f);

        int legY = 25;
        p.setBrush(QColor("#10B981"));
        p.setPen(Qt::NoPen);
        p.drawEllipse(135, legY, 8, 8);
        p.setPen(QColor("#E2E8F0"));
        p.drawText(148, legY + 8, QString("Dispo (%1)").arg(dispo));
        legY += 25;

        p.setBrush(QColor("#EF4444"));
        p.setPen(Qt::NoPen);
        p.drawEllipse(135, legY, 8, 8);
        p.setPen(QColor("#E2E8F0"));
        p.drawText(148, legY + 8, QString("Indispo (%1)").arg(indispo));

        f.setBold(false);
        f.setPointSize(8);
        p.setFont(f);
        p.setPen(QColor("#64748B"));
        p.drawText(10, 175, "Répartition par disponibilité");
        p.end();
        ui->sallesPage_labelStatutPie->setPixmap(pix);
    }

    {
        QPixmap pix(230, 190);
        pix.fill(Qt::transparent);
        QPainter p(&pix);
        p.setRenderHint(QPainter::Antialiasing);

        QMap<QString, int> types;
        for (int i = 0; i < ui->tableSalles->rowCount(); ++i) {
            QTableWidgetItem *it = ui->tableSalles->item(i, 3);
            if (it && !it->text().isEmpty()) types[it->text()]++;
        }

        if (types.isEmpty()) {
            QFont f = p.font();
            f.setFamily("Segoe UI");
            f.setPointSize(9);
            f.setBold(true);
            p.setFont(f);
            p.setPen(QColor("#64748B"));
            p.drawText(10, 100, "Aucune salle enregistrée");
        } else {
            int maxVal = 1;
            for (int v : types) if (v > maxVal) maxVal = v;

            auto couleur = [](const QString &t) -> QColor {
                if (t == "Salle informatique") return QColor("#818CF8");
                if (t == "Salle de réunion")   return QColor("#FBBF24");
                if (t == "Salle conférence")   return QColor("#10B981");
                return QColor("#EF4444");
            };

            int y = 20;
            const int barMaxW = 60;
            const int barH = 18;

            QFont f = p.font();
            f.setFamily("Segoe UI");
            f.setPointSize(8);
            f.setBold(true);
            p.setFont(f);

            for (auto it = types.begin(); it != types.end(); ++it) {
                p.setBrush(couleur(it.key()));
                p.setPen(Qt::NoPen);
                p.drawEllipse(10, y + 4, 8, 8);

                p.setPen(QColor("#E2E8F0"));
                p.drawText(25, y + 12, it.key());

                p.setPen(Qt::NoPen);
                p.setBrush(QColor("#1E293B"));
                p.drawRoundedRect(130, y, barMaxW, barH, 4, 4);

                int w = int(barMaxW * double(it.value()) / maxVal);
                p.setBrush(couleur(it.key()));
                p.drawRoundedRect(130, y, w, barH, 4, 4);

                p.setPen(QColor("#FFFFFF"));
                p.drawText(130 + w + 6, y + 13, QString::number(it.value()));

                y += barH + 20;
            }
        }

        QFont f = p.font();
        f.setFamily("Segoe UI");
        f.setPointSize(8);
        f.setBold(false);
        p.setFont(f);
        p.setPen(QColor("#64748B"));
        p.drawText(10, 175, "Nombre de salles par type");
        p.end();
        ui->sallesPage_labelRoleBar->setPixmap(pix);
    }
}

/* ============================================================
 *  FORMULAIRE SALLE — helpers
 * ============================================================ */
void integration::viderFormulaireSalle()
{
    if (ui->lineEditSalleID) ui->lineEditSalleID->clear();
    if (ui->lineEditSalleNom) ui->lineEditSalleNom->clear();
    if (ui->spinBoxSalleCapacite) ui->spinBoxSalleCapacite->setValue(0);
    if (ui->comboBoxSalleType) ui->comboBoxSalleType->setCurrentIndex(0);
    if (ui->comboBoxSalleDispo) ui->comboBoxSalleDispo->setCurrentIndex(0);
}

/* ============================================================
 *  B — ANNULER L'AJOUT
 * ============================================================ */
void integration::onAnnulerSalle()
{
    viderFormulaireSalle();
}

/* ============================================================
 *  A — AJOUTER UNE SALLE
 * ============================================================ */
void integration::onAjouterSalle()
{
    if (!ui->lineEditSalleNom || !ui->spinBoxSalleCapacite ||
        !ui->comboBoxSalleType || !ui->comboBoxSalleDispo) {
        return;
    }

    const QString nom = ui->lineEditSalleNom->text().trimmed();
    const int capacite = ui->spinBoxSalleCapacite->value();
    const QString type = ui->comboBoxSalleType->currentText().trimmed();
    const QString dispoUi = ui->comboBoxSalleDispo->currentText().trimmed();
    const QString dispo = normaliserDisponibilite(dispoUi);

    if (nom.isEmpty()) {
        QMessageBox::warning(this, "Ajouter une salle",
                             "Le nom de la salle est obligatoire.");
        return;
    }
    if (capacite <= 0) {
        QMessageBox::warning(this, "Ajouter une salle",
                             "La capacité doit être un entier strictement positif.");
        return;
    }
    if (type.isEmpty()) {
        QMessageBox::warning(this, "Ajouter une salle",
                             "Le type de salle est obligatoire.");
        return;
    }

    QSqlDatabase db = dbHacknova();
    if (!db.isValid() || !db.isOpen()) {
        QMessageBox::critical(this, "Ajouter une salle",
                              "Connexion Oracle indisponible. Aucune insertion effectuée.");
        return;
    }

    QSqlQuery q(db);
    q.prepare("INSERT INTO SALLE (ID_SALLE, NOM_SALLE, CAPACITE, TYPE_SALLE, DISPONIBILITE) "
              "VALUES (SEQ_SALLE.NEXTVAL, :nom, :cap, :type, :dispo)");
    q.bindValue(":nom",   nom,      QSql::In);
    q.bindValue(":cap",   capacite, QSql::In);
    q.bindValue(":type",  type,     QSql::In);
    q.bindValue(":dispo", dispo,    QSql::In);

    if (!q.exec()) {
        qDebug() << "onAjouterSalle : erreur SQL :" << q.lastError().text();
        QMessageBox::warning(this, "Ajouter une salle",
                             "L'insertion a échoué :\n" + q.lastError().text());
        return;
    }

    if (dispoUi.compare(dispo, Qt::CaseInsensitive) != 0) {
        QMessageBox::information(
            this, "Ajouter une salle",
            QString("Salle ajoutée dans Oracle.\n\n"
                    "Remarque : la valeur « %1 » a été enregistrée comme "
                    "« %2 », car la contrainte Oracle CK_SALLE_DISP n'autorise "
                    "que 'Disponible' et 'Indisponible'.")
                .arg(dispoUi, dispo));
    } else {
        QMessageBox::information(this, "Ajouter une salle",
                                 "Salle ajoutée dans Oracle.");
    }

    viderFormulaireSalle();
    rafraichirSallesEtStats();
}

/* ============================================================
 *  C — MODIFIER UNE SALLE
 * ============================================================ */
void integration::ouvrirModificationSalle(qint64 idSalle)
{
    QSqlDatabase db = dbHacknova();
    if (!db.isValid() || !db.isOpen()) {
        QMessageBox::critical(this, "Modifier une salle",
                              "Connexion Oracle indisponible.");
        return;
    }

    QSqlQuery q(db);
    q.prepare("SELECT NOM_SALLE, CAPACITE, TYPE_SALLE, DISPONIBILITE "
              "FROM SALLE WHERE ID_SALLE = :id");
    q.bindValue(":id", QString::number(idSalle), QSql::In);
    if (!q.exec() || !q.next()) {
        QMessageBox::warning(
            this, "Modifier une salle",
            QString("Salle introuvable dans Oracle (ID %1).\n\nDétail SQL : %2")
                .arg(idSalle)
                .arg(q.lastError().text()));
        return;
    }

    const QString nomActuel     = sqlStr(q.value(0));
    const int     capActuelle   = q.value(1).toInt();
    const QString typeActuel    = sqlStr(q.value(2));
    const QString dispoActuelle = sqlStr(q.value(3));
    q.finish();

    QDialog dlg(this);
    dlg.setWindowTitle(QString("Modifier la salle (ID %1)").arg(idSalle));
    dlg.setMinimumWidth(440);

    auto *root = new QVBoxLayout(&dlg);
    root->setContentsMargins(20, 20, 20, 20);
    root->setSpacing(12);

    auto *form = new QFormLayout;
    form->setLabelAlignment(Qt::AlignRight);
    form->setSpacing(10);

    auto *edNom = new QLineEdit(nomActuel, &dlg);

    auto *spCap = new QSpinBox(&dlg);
    spCap->setRange(1, 9999);
    spCap->setValue(capActuelle > 0 ? capActuelle : 1);

    auto *cbType = new QComboBox(&dlg);
    if (ui->comboBoxSalleType) {
        for (int i = 0; i < ui->comboBoxSalleType->count(); ++i)
            cbType->addItem(ui->comboBoxSalleType->itemText(i));
    }
    if (!typeActuel.isEmpty() && cbType->findText(typeActuel) < 0)
        cbType->addItem(typeActuel);
    int idxType = cbType->findText(typeActuel);
    cbType->setCurrentIndex(idxType >= 0 ? idxType : 0);

    auto *cbDispo = new QComboBox(&dlg);
    cbDispo->addItems({"Disponible", "Indisponible"});
    const bool estDispo =
        (dispoActuelle.compare("Disponible", Qt::CaseInsensitive) == 0);
    cbDispo->setCurrentIndex(estDispo ? 0 : 1);

    form->addRow("Nom :",           edNom);
    form->addRow("Capacité :",      spCap);
    form->addRow("Type :",          cbType);
    form->addRow("Disponibilité :", cbDispo);
    root->addLayout(form);

    auto *btns = new QHBoxLayout;
    btns->addStretch();
    auto *btnAnnuler = new QPushButton("Annuler", &dlg);
    auto *btnValider = new QPushButton("Enregistrer", &dlg);
    btnValider->setDefault(true);
    btns->addWidget(btnAnnuler);
    btns->addWidget(btnValider);
    root->addLayout(btns);

    QObject::connect(btnAnnuler, &QPushButton::clicked, &dlg, &QDialog::reject);

    bool modifie = false;

    QObject::connect(btnValider, &QPushButton::clicked, &dlg, [&]() {
        const QString nom   = edNom->text().trimmed();
        const int     cap   = spCap->value();
        const QString type  = cbType->currentText().trimmed();
        const QString dispo = cbDispo->currentText();

        if (nom.isEmpty()) {
            QMessageBox::warning(&dlg, "Modifier une salle",
                                 "Le nom de la salle est obligatoire.");
            return;
        }
        if (cap <= 0) {
            QMessageBox::warning(&dlg, "Modifier une salle",
                                 "La capacité doit être strictement positive.");
            return;
        }

        QSqlDatabase db2 = dbHacknova();
        if (!db2.isValid() || !db2.isOpen()) {
            QMessageBox::critical(&dlg, "Modifier une salle",
                                  "Connexion Oracle perdue.");
            return;
        }

        QSqlQuery u(db2);
        u.prepare("UPDATE SALLE SET NOM_SALLE = :nom, CAPACITE = :cap, "
                  "TYPE_SALLE = :type, DISPONIBILITE = :dispo "
                  "WHERE ID_SALLE = :id");
        u.bindValue(":nom",   nom,   QSql::In);
        u.bindValue(":cap",   cap,   QSql::In);
        u.bindValue(":type",  type,  QSql::In);
        u.bindValue(":dispo", dispo, QSql::In);
        u.bindValue(":id",    QString::number(idSalle), QSql::In);

        if (!u.exec()) {
            QMessageBox::warning(&dlg, "Modifier une salle",
                                 "La modification a échoué :\n" +
                                     u.lastError().text());
            return;
        }
        u.finish();

        QSqlQuery v(db2);
        v.prepare("SELECT COUNT(*) FROM SALLE "
                  "WHERE ID_SALLE = :id "
                  "AND NOM_SALLE = :nom "
                  "AND CAPACITE = :cap "
                  "AND DISPONIBILITE = :dispo");
        v.bindValue(":id",    QString::number(idSalle), QSql::In);
        v.bindValue(":nom",   nom,   QSql::In);
        v.bindValue(":cap",   cap,   QSql::In);
        v.bindValue(":dispo", dispo, QSql::In);
        if (!v.exec() || !v.next() || v.value(0).toInt() < 1) {
            QMessageBox::warning(&dlg, "Modifier une salle",
                                 "La modification n'a pas été confirmée dans Oracle.");
            return;
        }

        modifie = true;
        dlg.accept();
    });

    if (dlg.exec() != QDialog::Accepted || !modifie)
        return;

    QMessageBox::information(this, "Modifier une salle",
                             "Salle modifiée dans Oracle.");
    rafraichirSallesEtStats();
}

/* ============================================================
 *  D — SUPPRIMER UNE SALLE
 * ============================================================ */
void integration::ouvrirSuppressionSalle(qint64 idSalle, const QString &nomSalle)
{
    const auto rep = QMessageBox::question(
        this, "Supprimer une salle",
        QString("Supprimer définitivement la salle « %1 » (ID %2) dans Oracle ?")
            .arg(nomSalle).arg(idSalle),
        QMessageBox::Yes | QMessageBox::No);
    if (rep != QMessageBox::Yes)
        return;

    QSqlDatabase db = dbHacknova();
    if (!db.isValid() || !db.isOpen()) {
        QMessageBox::critical(this, "Supprimer une salle",
                              "Connexion Oracle indisponible. Aucune suppression effectuée.");
        return;
    }

    QSqlQuery q(db);
    q.prepare("DELETE FROM SALLE WHERE ID_SALLE = :id");
    q.bindValue(":id", QString::number(idSalle), QSql::In);

    if (!q.exec()) {
        QMessageBox::warning(
            this, "Supprimer une salle",
            "La suppression a échoué :\n" + q.lastError().text() +
                "\n\nVérifiez que la salle n'est pas référencée par une "
                "autre table (contrainte d'intégrité).");
        return;
    }
    q.finish();

    QSqlQuery v(db);
    v.prepare("SELECT COUNT(*) FROM SALLE WHERE ID_SALLE = :id");
    v.bindValue(":id", QString::number(idSalle), QSql::In);
    if (!v.exec() || !v.next()) {
        QMessageBox::warning(this, "Supprimer une salle",
                             "Impossible de vérifier la suppression :\n" +
                                 v.lastError().text());
        return;
    }
    if (v.value(0).toInt() > 0) {
        QMessageBox::warning(this, "Supprimer une salle",
                             "La salle existe toujours dans Oracle. "
                             "La suppression n'a pas été confirmée.");
        return;
    }

    QMessageBox::information(this, "Supprimer une salle",
                             "Salle supprimée dans Oracle.");
    rafraichirSallesEtStats();
}

/* ============================================================
 *  E — RECHERCHER UNE SALLE
 * ============================================================ */
void integration::onRechercherSalle()
{
    if (!ui->lineEdit_2 || !ui->tableSalles) return;

    const QString terme = ui->lineEdit_2->text().trimmed();
    if (terme.isEmpty()) {
        rafraichirSallesEtStats();
        return;
    }

    QSqlDatabase db = dbHacknova();
    if (!db.isValid() || !db.isOpen()) {
        QMessageBox::critical(this, "Rechercher une salle",
                              "Connexion Oracle indisponible.");
        return;
    }

    auto *t = ui->tableSalles;
    viderTableSalles();

    QSqlQuery q(db);
    q.prepare("SELECT ID_SALLE, NOM_SALLE, CAPACITE, TYPE_SALLE, DISPONIBILITE "
              "FROM SALLE "
              "WHERE UPPER(NOM_SALLE) LIKE UPPER(:terme) "
              "   OR TO_CHAR(ID_SALLE) = :id "
              "ORDER BY ID_SALLE");
    q.bindValue(":terme", "%" + terme + "%", QSql::In);
    q.bindValue(":id", terme, QSql::In);

    if (!q.exec()) {
        QMessageBox::warning(this, "Rechercher une salle",
                             "Erreur SQL :\n" + q.lastError().text());
        rafraichirSallesEtStats();
        return;
    }

    int row = 0;
    while (q.next()) {
        const qint64 id     = q.value(0).toLongLong();
        const QString nom   = sqlStr(q.value(1));
        const QString cap   = sqlStr(q.value(2));
        const QString type  = sqlStr(q.value(3));
        const QString dispo = sqlStr(q.value(4));

        t->insertRow(row);
        t->setItem(row, 0, new QTableWidgetItem(QString::number(id)));
        t->setItem(row, 1, new QTableWidgetItem(nom));
        t->setItem(row, 2, new QTableWidgetItem(cap));
        t->setItem(row, 3, new QTableWidgetItem(type));

        auto *itemDispo = new QTableWidgetItem(dispo);
        itemDispo->setForeground(
            dispo.compare("Disponible", Qt::CaseInsensitive) == 0
                ? QColor("#10B981") : QColor("#EF4444"));
        t->setItem(row, 4, itemDispo);

        t->setCellWidget(row, 5, creerWidgetActions(id, nom));
        ++row;
    }

    t->setSortingEnabled(true);
}

/* ============================================================
 *  F — TRIER LES SALLES
 * ============================================================ */
void integration::onTrierSalle()
{
    if (!ui->tableSalles) return;
    auto *t = ui->tableSalles;
    t->setSortingEnabled(true);
    t->sortItems(0, Qt::AscendingOrder);
}

/* ============================================================
 *  G — EXPORTER LES SALLES
 * ============================================================ */
void integration::onExporterSalle()
{
    QSqlDatabase db = dbHacknova();
    if (!db.isValid() || !db.isOpen()) {
        QMessageBox::critical(this, "Exporter les salles",
                              "Connexion Oracle indisponible. Aucun export effectué.");
        return;
    }

    const QString chemin = QFileDialog::getSaveFileName(
        this, "Exporter les salles",
        QString("salles_%1.csv")
            .arg(QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss")),
        "Fichiers CSV (*.csv)");
    if (chemin.isEmpty()) return;

    QSqlQuery q(db);
    if (!q.exec("SELECT ID_SALLE, NOM_SALLE, CAPACITE, TYPE_SALLE, DISPONIBILITE "
                "FROM SALLE ORDER BY ID_SALLE")) {
        QMessageBox::warning(this, "Exporter les salles",
                             "Erreur SQL :\n" + q.lastError().text());
        return;
    }

    QFile f(chemin);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Exporter les salles",
                             "Impossible d'écrire le fichier :\n" + f.errorString());
        return;
    }

    QTextStream out(&f);
    out.setEncoding(QStringConverter::Utf8);
    out << "ID_SALLE;NOM_SALLE;CAPACITE;TYPE_SALLE;DISPONIBILITE\n";

    auto echapper = [](const QString &s) -> QString {
        QString v = s;
        v.replace("\"", "\"\"");
        if (v.contains(';') || v.contains('\n') || v.contains('"'))
            v = "\"" + v + "\"";
        return v;
    };

    int nb = 0;
    while (q.next()) {
        out << q.value(0).toString() << ';'
            << echapper(sqlStr(q.value(1))) << ';'
            << q.value(2).toString() << ';'
            << echapper(sqlStr(q.value(3))) << ';'
            << echapper(sqlStr(q.value(4))) << '\n';
        ++nb;
    }
    f.close();

    QMessageBox::information(this, "Exporter les salles",
                             QString("%1 salle(s) exportée(s) vers :\n%2")
                                 .arg(nb).arg(chemin));
}

/* ============================================================
 *  ACTIONS EXISTANTES (affectation + QR)
 * ============================================================ */
void integration::onSuggererSalle()
{
    if (!ui->leEffectif_2) return;
    const QString effStr = ui->leEffectif_2->text().trimmed();
    bool ok = false;
    int effectif = effStr.toInt(&ok);

    if (!ok || effectif <= 0) {
        QMessageBox::warning(this, "Affectation",
                             "Veuillez saisir un effectif valide (nombre entier > 0).");
        return;
    }

    QSqlDatabase db = dbHacknova();
    if (!db.isValid() || !db.isOpen()) {
        QMessageBox::critical(this, "Affectation",
                              "Connexion Oracle indisponible.");
        return;
    }

    QSqlQuery q(db);
    q.prepare("SELECT NOM_SALLE, CAPACITE, TYPE_SALLE "
              "FROM SALLE "
              "WHERE DISPONIBILITE = 'Disponible' AND CAPACITE >= :cap "
              "ORDER BY CAPACITE ASC");
    q.bindValue(":cap", effectif, QSql::In);

    if (!q.exec()) {
        QMessageBox::warning(this, "Affectation",
                             "Erreur SQL :\n" + q.lastError().text());
        return;
    }

    if (q.next()) {
        const QString nom = sqlStr(q.value(0));
        const QString cap = sqlStr(q.value(1));
        const QString typ = sqlStr(q.value(2));
        QMessageBox::information(
            this, "Affectation intelligente",
            QString("Salle recommandée pour un effectif de %1 :<br><br>"
                    "<b>%2</b> (cap. %3) — type : %4")
                .arg(effectif).arg(nom, cap, typ));
    } else {
        QMessageBox::warning(
            this, "Affectation intelligente",
            "Aucune salle disponible ne peut accueillir " +
                QString::number(effectif) + " personnes.");
    }
}

void integration::onValiderQR()
{
    if (!ui->leCodeQR_4) return;
    const QString code = ui->leCodeQR_4->text().trimmed();

    if (code.isEmpty()) {
        QMessageBox::warning(this, "QR Code",
                             "Veuillez saisir ou scanner un code.");
        return;
    }

    QMessageBox::information(
        this, "QR Code · Accès",
        QString("Code <b>%1</b> validé.<br>Accès autorisé.")
            .arg(code.toHtmlEscaped()));
    ui->leCodeQR_4->clear();
}

/* ============================================================
 *  NAVIGATION
 * ============================================================ */
void integration::allerPage(int index)
{
    if (index < 0 || index >= ui->stackedWidget->count())
        return;
    ui->stackedWidget->setCurrentIndex(index);

    QWidget *w = ui->stackedWidget->currentWidget();
    if (w) {
        if (auto *sa = w->findChild<QScrollArea*>()) {
            sa->verticalScrollBar()->setValue(0);
        }
    }
}

void integration::goLogin()     { allerPage(0); }
void integration::goEmployes()  { allerPage(1); }
void integration::goSalles()    { allerPage(2); }

/* ============================================================
 *  LOGIN — connexions des widgets
 * ============================================================ */
void integration::connecterLogin()
{
    // Bouton SE CONNECTER
    if (ui->pushButtonConnexion)
        QObject::connect(ui->pushButtonConnexion, &QPushButton::clicked,
                         this, &integration::onConnexion);

    // Touche Entrée dans le champ mot de passe
    if (ui->lineEditMotDePasse)
        QObject::connect(ui->lineEditMotDePasse, &QLineEdit::returnPressed,
                         this, &integration::onConnexion);

    // Touche Entrée dans le champ identifiant (pour aller au mot de passe)
    if (ui->lineEditIdentifiant)
        QObject::connect(ui->lineEditIdentifiant, &QLineEdit::returnPressed,
                         this, [this]() {
                             if (ui->lineEditMotDePasse)
                                 ui->lineEditMotDePasse->setFocus();
                         });

    // Case « Afficher le mot de passe »
    if (ui->checkBoxSouvenir && ui->lineEditMotDePasse) {
        QObject::connect(ui->checkBoxSouvenir, &QCheckBox::toggled,
                         this, [this](bool checked) {
                             ui->lineEditMotDePasse->setEchoMode(
                                 checked ? QLineEdit::Normal
                                         : QLineEdit::Password);
                         });
    }
}

/* ============================================================
 *  LOGIN — authentification réelle sur la table UTILISATEURS
 * ============================================================ */
/* ============================================================
 *  LOGIN — authentification réelle sur la table UTILISATEURS
 * ============================================================ */
void integration::onConnexion()
{
    if (!ui->lineEditIdentifiant || !ui->lineEditMotDePasse)
        return;

    const QString user = ui->lineEditIdentifiant->text().trimmed();
    const QString pass = ui->lineEditMotDePasse->text();

    // --- 1. Champs vides ---
    if (user.isEmpty() || pass.isEmpty()) {
        QMessageBox::warning(
            this,
            "Connexion échouée",
            "Nom d'utilisateur ou mot de passe incorrect. Veuillez réessayer.");
        if (user.isEmpty())
            ui->lineEditIdentifiant->setFocus();
        else
            ui->lineEditMotDePasse->setFocus();
        return;
    }

    // --- 2. Connexion Oracle existante ---
    QSqlDatabase db = dbHacknova();
    if (!db.isValid() || !db.isOpen()) {
        QMessageBox::critical(
            this,
            "Connexion",
            "Connexion à la base Oracle indisponible.\n"
            "Veuillez vérifier le DSN HACKNOVA et réessayer.");
        return;
    }

    // --- 3. Requête préparée (SANS COUNT) ---
    QSqlQuery q(db);
    q.prepare("SELECT NOM_UTILISATEUR, ACTIF "
              "FROM UTILISATEURS "
              "WHERE UPPER(TRIM(NOM_UTILISATEUR)) = UPPER(TRIM(:u)) "
              "  AND MOT_DE_PASSE_HASH = :p");
    q.bindValue(":u", user, QSql::In);
    q.bindValue(":p", pass, QSql::In);

    if (!q.exec()) {
        QMessageBox::warning(
            this,
            "Connexion",
            "Erreur lors de la vérification des identifiants :\n" +
                q.lastError().text());
        return;
    }

    // --- 4. Vérification des résultats ---
    if (q.next()) {
        QString actifValue = q.value(1).toString().trimmed().toUpper();

        // CORRECTION ICI : Accepte la lettre 'O' OU le chiffre '0'
        if (actifValue == "O" || actifValue == "0") {
            // Succès !
            q.finish();
            ui->lineEditMotDePasse->clear();
            goEmployes();
            return;
        } else {
            // L'utilisateur existe mais le compte est inactif.
            QMessageBox::warning(
                this,
                "Connexion refusée",
                "Ce compte est inactif.\n"
                "Veuillez contacter l'administrateur.");
            q.finish();
            return;
        }
    }

    // --- 5. Échec : utilisateur ou mot de passe introuvable ---
    q.finish();
    QMessageBox::warning(
        this,
        "Connexion échouée",
        "Nom d'utilisateur ou mot de passe incorrect. Veuillez réessayer.");
    ui->lineEditMotDePasse->setFocus();
    ui->lineEditMotDePasse->selectAll();
}

/* ============================================================
 *  NAVIGATION
 * ============================================================ */
void integration::connecterNavigation()
{
    if (ui->btnEmployes) QObject::connect(ui->btnEmployes, &QPushButton::clicked, this, &integration::goEmployes);
    if (ui->btnSalles)   QObject::connect(ui->btnSalles,   &QPushButton::clicked, this, &integration::goSalles);
    if (ui->sallesPage_btnEmployes) QObject::connect(ui->sallesPage_btnEmployes, &QPushButton::clicked, this, &integration::goEmployes);
    if (ui->sallesPage_btnSalles)   QObject::connect(ui->sallesPage_btnSalles,   &QPushButton::clicked, this, &integration::goSalles);
    if (ui->btnNotifications) QObject::connect(ui->btnNotifications, &QPushButton::clicked, this, &integration::onNotifications);
    if (ui->btnProfil)        QObject::connect(ui->btnProfil,        &QPushButton::clicked, this, &integration::onProfil);
    if (ui->btnDeconnexion)   QObject::connect(ui->btnDeconnexion,   &QPushButton::clicked, this, &integration::onDeconnexion);
    if (ui->sallesPage_btnDeconnexion) QObject::connect(ui->sallesPage_btnDeconnexion, &QPushButton::clicked, this, &integration::onDeconnexion);
}

/* ============================================================
 *  CHATBOT
 * ============================================================ */
void integration::connecterChatbot()
{
    if (ui->leQuestion)
        QObject::connect(ui->leQuestion, &QLineEdit::returnPressed,
                         this, &integration::onEnvoyerQuestion);

    if (ui->textChatbot) {
        ui->textChatbot->setHtml(R"(
            <div style='color:#E2E8F0;font-family:Segoe UI, Arial;font-size:13px;'>
            <p><b style='color:#818CF8;'>HACKNOVA Bot :</b>
            Bonjour 👋 Je suis votre assistant. Posez-moi une question
            sur les salles, les employés, les statuts, etc.</p>
            </div>
        )");
    }
}

void integration::onEnvoyerQuestion()
{
    if (!ui->leQuestion || !ui->textChatbot) return;

    const QString q = ui->leQuestion->text().trimmed();
    if (q.isEmpty()) return;

    QString reponse;
    const QString ql = q.toLower();

    if (ql.contains("bonjour") || ql.contains("salut") || ql.contains("hello")) {
        reponse = "Bonjour 👋 Comment puis-je vous aider ?";
    }
    else if (ql.contains("combien") && ql.contains("salle")) {
        reponse = QString("Il y a actuellement <b>%1 salles</b> enregistrées.")
                      .arg(ui->tableSalles->rowCount());
    }
    else if (ql.contains("disponible") || ql.contains("dispo")) {
        int n = 0;
        for (int i = 0; i < ui->tableSalles->rowCount(); ++i) {
            QTableWidgetItem *it = ui->tableSalles->item(i, 4);
            if (it && it->text().compare("Disponible", Qt::CaseInsensitive) == 0) n++;
        }
        reponse = QString("<b>%1 salles</b> sont disponibles.").arg(n);
    }
    else if (ql.contains("combien") && ql.contains("employ")) {
        reponse = QString("Il y a actuellement <b>%1 employés</b> enregistrés.")
                      .arg(ui->tableEmployes->rowCount());
    }
    else if (ql.contains("statistique") || ql.contains("graphique")) {
        reponse = "Consultez les <b>statistiques</b> à droite : "
                  "camemberts et barres calculés à partir des données Oracle.";
    }
    else if (ql.contains("merci")) {
        reponse = "Avec plaisir 😊";
    }
    else if (ql.contains("aide") || ql.contains("help")) {
        reponse = "Je peux vous renseigner sur : "
                  "<ul><li>le nombre de salles / employés</li>"
                  "<li>les disponibilités des salles</li>"
                  "<li>les statistiques</li></ul>";
    }
    else {
        reponse = "Je n'ai pas compris votre question. Tapez <b>aide</b> pour voir ce que je sais faire.";
    }

    QString html = ui->textChatbot->toHtml();
    html += QString(R"(
        <div style='color:#E2E8F0;font-family:Segoe UI, Arial;font-size:13px;margin-top:8px;'>
        <p><b style='color:#10B981;'>Vous :</b> %1</p>
        <p><b style='color:#818CF8;'>Bot :</b> %2</p>
        </div>
    )").arg(q.toHtmlEscaped(), reponse);

    ui->textChatbot->setHtml(html);
    ui->textChatbot->verticalScrollBar()->setValue(
        ui->textChatbot->verticalScrollBar()->maximum());
    ui->leQuestion->clear();
}

/* ============================================================
 *  HELPERS DIALOGUES
 * ============================================================ */
static void afficherDialogue(QWidget *parent,
                             const QString &titre,
                             const QString &html)
{
    QDialog dlg(parent);
    dlg.setWindowTitle(titre);
    dlg.setMinimumSize(500, 400);

    auto *lay = new QVBoxLayout(&dlg);
    lay->setContentsMargins(20, 20, 20, 20);
    lay->setSpacing(12);

    auto *txt = new QTextBrowser;
    txt->setHtml(html);
    txt->setOpenExternalLinks(true);
    lay->addWidget(txt, 1);

    auto *btn = new QPushButton("FERMER");
    btn->setCursor(Qt::PointingHandCursor);
    btn->setMinimumHeight(32);
    QObject::connect(btn, &QPushButton::clicked, &dlg, &QDialog::accept);
    lay->addWidget(btn, 0, Qt::AlignRight);

    dlg.exec();
}

void integration::onNotifications()
{
    afficherDialogue(this, "Notifications", R"(
        <div style='color:#E2E8F0;font-family:Segoe UI, Arial;font-size:13px;'>
        <p style='color:#818CF8;font-weight:800;'>Notification</p>
        <p style='color:#94A3B8;font-size:12px;'>Aucune notification disponible.</p>
        </div>
    )");
}

void integration::onProfil()
{
    afficherDialogue(this, "Profil utilisateur", R"(
        <div style='color:#E2E8F0;text-align:center;font-family:Segoe UI, Arial;'>
        <h1 style='color:#818CF8;'>HN</h1>
        <h2 style='color:#FFFFFF;'>Utilisateur</h2>
        <p style='color:#94A3B8;'><b>Rôle :</b> Administrateur</p>
        <p style='color:#94A3B8;'><b>Statut :</b>
        <span style='color:#10B981;'>Actif</span></p>
        <hr/>
        <p style='color:#64748B;font-size:11px;'>
        HACKNOVA — Smart Hackathon Management</p>
        </div>
    )");
}

void integration::onDeconnexion()
{
    const auto rep = QMessageBox::question(
        this, "Déconnexion",
        "Voulez-vous vraiment vous déconnecter ?",
        QMessageBox::Yes | QMessageBox::No);

    if (rep == QMessageBox::Yes) {
        if (ui->lineEditIdentifiant) ui->lineEditIdentifiant->clear();
        if (ui->lineEditMotDePasse)  ui->lineEditMotDePasse->clear();
        if (ui->checkBoxSouvenir)    ui->checkBoxSouvenir->setChecked(false);
        goLogin();
    }
}

/* ============================================================
 *  THÈME (inchangé, complet)
 * ============================================================ */
void integration::appliquerTheme()
{
    this->setStyleSheet(R"(
/* ================== GLOBAL ================== */
QMainWindow, QWidget#centralWidget { background-color: #080C14; }
QWidget { color: #E2E8F0; font-family: 'Segoe UI', 'Arial', sans-serif; font-size: 13px; }
QToolTip { background-color: #1E293B; color: #F1F5F9; border: 1px solid #475569; border-radius: 6px; padding: 6px 10px; font-size: 12px; }

/* ================== SCROLLBARS ================== */
QScrollBar:vertical { background: transparent; width: 10px; margin: 0; }
QScrollBar::handle:vertical { background: #334155; min-height: 40px; border-radius: 5px; }
QScrollBar::handle:vertical:hover { background: #475569; }
QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0; }
QScrollBar:horizontal { background: transparent; height: 10px; }
QScrollBar::handle:horizontal { background: #334155; min-width: 40px; border-radius: 5px; }
QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal { width: 0; }

/* ================== LOGIN PAGE ================== */
QFrame#frameLoginBrand { background-color: #1E1B4B; border: 1px solid #312E81; border-top-left-radius: 20px; border-bottom-left-radius: 20px; }
QFrame#frameLoginCard { background-color: #111827; border: 1px solid #1E293B; border-top-right-radius: 20px; border-bottom-right-radius: 20px; }
QLabel#labelLoginLogo { background-color: #4338CA; color: #FFFFFF; border-radius: 60px; font-family: 'Segoe UI', 'Arial'; font-size: 44px; font-weight: 900; }
QLabel#labelLoginBrandTitle { color: #FFFFFF; font-size: 28px; font-weight: 900; letter-spacing: 2px; }
QLabel#labelLoginBrandSub { color: #A5B4FC; font-size: 11px; font-weight: 800; letter-spacing: 1.5px; }
QLabel#labelLoginBrandSlogan { color: #C7D2FE; font-size: 13px; }
QLabel#labelLoginBrandTeam { color: #6366F1; font-size: 10px; font-weight: 800; letter-spacing: 1px; }
QLabel#labelLoginTitle { color: #FFFFFF; font-size: 24px; font-weight: 900; }
QLabel#labelLoginField, QLabel#labelLoginField2 { color: #818CF8; font-size: 11px; font-weight: 800; letter-spacing: 1px; }
QLineEdit#lineEditIdentifiant, QLineEdit#lineEditMotDePasse { background-color: #0F172A; color: #F1F5F9; border: 1px solid #334155; border-radius: 8px; padding: 0 12px; font-size: 14px; selection-background-color: #4338CA; }
QLineEdit#lineEditIdentifiant:focus, QLineEdit#lineEditMotDePasse:focus { border: 1px solid #6366F1; background-color: #111827; }
QCheckBox#checkBoxSouvenir { color: #94A3B8; font-size: 12px; spacing: 6px; }
QCheckBox#checkBoxSouvenir::indicator { width: 16px; height: 16px; border: 1px solid #475569; border-radius: 3px; background: #0F172A; }
QCheckBox#checkBoxSouvenir::indicator:checked { background: #4338CA; border: 1px solid #6366F1; }
QPushButton#pushButtonConnexion { background-color: #4338CA; color: #FFFFFF; border: none; border-radius: 8px; font-size: 14px; font-weight: 900; letter-spacing: 1px; }
QPushButton#pushButtonConnexion:hover { background-color: #6366F1; }
QPushButton#pushButtonConnexion:pressed { background-color: #3730A3; }
QLabel#labelLoginFooter { color: #6366F1; font-size: 12px; font-weight: bold; }

/* ================== NAVBAR ================== */
QFrame#frameNavbar, QFrame#sallesPage_frameNavbar { background-color: #111827; border: 1px solid #1E293B; border-radius: 12px; }
QLabel#labelNavLogo, QLabel#sallesPage_labelNavLogo { background-color: #4338CA; color: #FFFFFF; border-radius: 8px; font-size: 18px; font-weight: 900; }
QLabel#labelNavBrand, QLabel#sallesPage_labelNavBrand { color: #FFFFFF; font-size: 14px; font-weight: 900; letter-spacing: 1.5px; }
QLabel#labelNavSub, QLabel#sallesPage_labelNavSub { color: #818CF8; font-size: 9px; font-weight: 800; letter-spacing: 0.5px; }

QPushButton#btnDashboard, QPushButton#btnEmployes, QPushButton#btnSalles,
QPushButton#btnEvaluateurs, QPushButton#btnParticipants, QPushButton#btnSponsors,
QPushButton#btnProjets, QPushButton#btnNotifications, QPushButton#btnProfil,
QPushButton#sallesPage_btnDashboard, QPushButton#sallesPage_btnEmployes,
QPushButton#sallesPage_btnEvaluateurs, QPushButton#sallesPage_btnParticipants,
QPushButton#sallesPage_btnSponsors, QPushButton#sallesPage_btnProjets,
QPushButton#sallesPage_btnNotifications, QPushButton#sallesPage_btnProfil {
    background-color: transparent; color: #94A3B8; border: none; border-radius: 6px;
    font-size: 10px; font-weight: 800; letter-spacing: 0.5px; padding: 6px 8px;
}
QPushButton#btnDashboard:hover, QPushButton#btnEmployes:hover, QPushButton#btnSalles:hover,
QPushButton#btnEvaluateurs:hover, QPushButton#btnParticipants:hover, QPushButton#btnSponsors:hover,
QPushButton#btnProjets:hover, QPushButton#btnNotifications:hover, QPushButton#btnProfil:hover,
QPushButton#sallesPage_btnDashboard:hover, QPushButton#sallesPage_btnEmployes:hover,
QPushButton#sallesPage_btnEvaluateurs:hover, QPushButton#sallesPage_btnParticipants:hover,
QPushButton#sallesPage_btnSponsors:hover, QPushButton#sallesPage_btnProjets:hover,
QPushButton#sallesPage_btnNotifications:hover, QPushButton#sallesPage_btnProfil:hover {
    background-color: #1F2937; color: #F1F5F9;
}
QPushButton#btnEmployes, QPushButton#sallesPage_btnSalles { background-color: #312E81; color: #FFFFFF; border-bottom: 2px solid #6366F1; }
QPushButton#btnDeconnexion, QPushButton#sallesPage_btnDeconnexion { background-color: #7F1D1D; color: #FFFFFF; border: 1px solid #991B1B; border-radius: 6px; font-size: 10px; font-weight: 800; padding: 6px 8px; }
QPushButton#btnDeconnexion:hover, QPushButton#sallesPage_btnDeconnexion:hover { background-color: #991B1B; }

/* ================== TITRES ================== */
QLabel#labelTitleEmployes, QLabel#labelTitleSalles { color: #FFFFFF; font-size: 22px; font-weight: 900; letter-spacing: 0.5px; }
QLabel#labelSubEmployes, QLabel#labelSubSalles { color: #64748B; font-size: 13px; }

/* ================== CARTES ================== */
QFrame#cardFormEmployes, QFrame#cardFormSalles, QFrame#cardTableEmployes, QFrame#cardTableSalles,
QFrame#frameChatbot, QFrame#cardStats, QFrame#sallesPage_cardStats,
QFrame#frameAffectation_2, QFrame#frameQR_4 {
    background-color: #111827; border: 1px solid #1E293B; border-radius: 12px;
}
QLabel#labelFormTitleEmployes, QLabel#labelFormTitleSalles,
QLabel#labelTableTitleEmployes, QLabel#labelTableTitleSalles,
QLabel#lblTitreChatbot, QLabel#labelStatsTitle, QLabel#sallesPage_labelStatsTitle,
QLabel#lblTitreAffect_2, QLabel#lblTitreQR_4 {
    color: #FFFFFF; font-size: 15px; font-weight: 800; letter-spacing: 0.5px;
}
QLabel#lblDescAffect_2, QLabel#lblDescQR_4 { color: #94A3B8; font-size: 12px; }

/* ================== FORMULAIRES ================== */
QLabel#lblEmpID, QLabel#lblEmpNom, QLabel#lblEmpPrenom, QLabel#lblEmpMdp,
QLabel#lblEmpRole, QLabel#lblEmpStatut, QLabel#lblSalleID, QLabel#lblSalleNom,
QLabel#lblSalleCapacite, QLabel#lblSalleType, QLabel#lblSalleDispo {
    color: #818CF8; font-size: 11px; font-weight: 800; letter-spacing: 0.5px;
}
QLineEdit#lineEditEmpID, QLineEdit#lineEditEmpNom, QLineEdit#lineEditEmpPrenom,
QLineEdit#lineEditEmpMdp, QLineEdit#lineEditSalleID, QLineEdit#lineEditSalleNom,
QLineEdit#lineEdit, QLineEdit#lineEdit_2, QSpinBox#spinBoxSalleCapacite {
    background-color: #0B1120; color: #F1F5F9; border: 1px solid #334155; border-radius: 6px;
    padding: 0 10px; font-size: 13px; selection-background-color: #4338CA; min-height: 32px;
}
QLineEdit#lineEditEmpID:focus, QLineEdit#lineEditEmpNom:focus, QLineEdit#lineEditEmpPrenom:focus,
QLineEdit#lineEditEmpMdp:focus, QLineEdit#lineEditSalleID:focus, QLineEdit#lineEditSalleNom:focus,
QLineEdit#lineEdit:focus, QLineEdit#lineEdit_2:focus, QSpinBox#spinBoxSalleCapacite:focus {
    border: 1px solid #6366F1; background-color: #111827;
}
QLineEdit#lineEditEmpID:read-only, QLineEdit#lineEditSalleID:read-only { background-color: #1E293B; color: #64748B; }

QComboBox#comboBoxEmpRole, QComboBox#comboBoxEmpStatut, QComboBox#comboBoxSalleType, QComboBox#comboBoxSalleDispo {
    background-color: #0B1120; color: #F1F5F9; border: 1px solid #334155; border-radius: 6px;
    padding: 0 10px; font-size: 13px; min-height: 32px;
}
QComboBox#comboBoxEmpRole:focus, QComboBox#comboBoxEmpStatut:focus,
QComboBox#comboBoxSalleType:focus, QComboBox#comboBoxSalleDispo:focus { border: 1px solid #6366F1; }
QComboBox::drop-down { border: none; width: 20px; }
QComboBox::down-arrow { border-left: 4px solid transparent; border-right: 4px solid transparent; border-top: 5px solid #818CF8; width: 0; height: 0; margin-right: 6px; }
QComboBox QAbstractItemView { background-color: #1E293B; color: #F1F5F9; border: 1px solid #334155; border-radius: 6px; selection-background-color: #4338CA; selection-color: #FFFFFF; padding: 4px; font-size: 13px; }

/* ================== BOUTONS D'ACTION ================== */
QPushButton#btnAjouterEmploye, QPushButton#btnExporterEmploye,
QPushButton#btnAjouterSalle, QPushButton#btnExporterSalle {
    background-color: #059669; color: #FFFFFF; border: 1px solid #047857; border-radius: 6px;
    font-size: 12px; font-weight: 800; letter-spacing: 0.5px; padding: 0 14px;
    min-width: 100px; min-height: 32px;
}
QPushButton#btnAjouterEmploye:hover, QPushButton#btnExporterEmploye:hover,
QPushButton#btnAjouterSalle:hover, QPushButton#btnExporterSalle:hover {
    background-color: #10B981; border-color: #34D399;
}
QPushButton#btnAjouterEmploye:pressed, QPushButton#btnExporterEmploye:pressed,
QPushButton#btnAjouterSalle:pressed, QPushButton#btnExporterSalle:pressed {
    background-color: #047857;
}
QPushButton#btn_Annuler, QPushButton#btnTrierEmploye, QPushButton#btnAnnuler, QPushButton#btnTrierSalle {
    background-color: #1E293B; color: #CBD5E1; border: 1px solid #334155; border-radius: 6px;
    font-size: 12px; font-weight: 800; padding: 0 14px; min-width: 100px; min-height: 32px;
}
QPushButton#btn_Annuler:hover, QPushButton#btnTrierEmploye:hover, QPushButton#btnAnnuler:hover, QPushButton#btnTrierSalle:hover {
    background-color: #334155; color: #FFFFFF; border-color: #475569;
}
QPushButton#btnRechercherEmploye, QPushButton#btnRechercherSalle {
    background-color: #4338CA; color: #FFFFFF; border: 1px solid #3730A3; border-radius: 6px;
    font-size: 12px; font-weight: 800; letter-spacing: 0.5px; padding: 0 14px;
    min-width: 100px; min-height: 32px;
}
QPushButton#btnRechercherEmploye:hover, QPushButton#btnRechercherSalle:hover {
    background-color: #6366F1; border-color: #818CF8;
}
QPushButton#btnSuggerer_2, QPushButton#btnValiderQR_4, QPushButton#btnEnvoyer {
    background-color: #4338CA; color: #FFFFFF; border: 1px solid #3730A3; border-radius: 6px;
    font-size: 12px; font-weight: 800; padding: 0 12px; min-width: 80px; min-height: 30px;
}
QPushButton#btnSuggerer_2:hover, QPushButton#btnValiderQR_4:hover, QPushButton#btnEnvoyer:hover {
    background-color: #6366F1; border-color: #818CF8;
}

/* ================== TABLEAUX ================== */
QTableWidget { background-color: #0B1120; alternate-background-color: #111827; color: #E2E8F0; border: 1px solid #1E293B; border-radius: 8px; gridline-color: #1E293B; font-size: 13px; selection-background-color: #312E81; selection-color: #FFFFFF; outline: none; }
QTableWidget::item { padding: 4px 8px; border-bottom: 1px solid #1E293B; }
QTableWidget::item:selected { background-color: #312E81; color: #FFFFFF; }
QHeaderView::section { background-color: #111827; color: #94A3B8; font-size: 11px; font-weight: 800; letter-spacing: 0.5px; padding: 8px 6px; border: none; border-bottom: 2px solid #312E81; }
QHeaderView::section:first { border-top-left-radius: 8px; }
QHeaderView::section:last  { border-top-right-radius: 8px; }
QTableCornerButton::section { background-color: #111827; border: none; }

/* ================== CHATBOT ================== */
QFrame#frameChatbot, QFrame#frameAffectation_2, QFrame#frameQR_4 { background-color: #111827; border: 1px solid #1E293B; border-radius: 12px; }
QLabel#lblTitreChatbot, QLabel#lblTitreAffect_2, QLabel#lblTitreQR_4 { color: #FFFFFF; font-size: 13px; font-weight: 800; }
QFrame#frameSaisieChatbot, QFrame#frameSaisieAffect_2, QFrame#frameSaisieQR_4 { background-color: #0B1120; border: 1px solid #1E293B; border-radius: 8px; }
QTextBrowser#textChatbot { background-color: #0B1120; color: #E2E8F0; border: 1px solid #1E293B; border-radius: 8px; font-size: 13px; padding: 6px; selection-background-color: #4338CA; }
QLineEdit#leQuestion, QLineEdit#leEffectif_2, QLineEdit#leCodeQR_4 { background-color: #111827; color: #F1F5F9; border: 1px solid #334155; border-radius: 6px; padding: 0 10px; font-size: 13px; }
QLineEdit#leQuestion:focus, QLineEdit#leEffectif_2:focus, QLineEdit#leCodeQR_4:focus { border: 1px solid #6366F1; background-color: #0B1120; }

/* ================== STATISTIQUES ================== */
QFrame#cardStats, QFrame#sallesPage_cardStats { background-color: #111827; border: 1px solid #312E81; border-radius: 12px; }
QLabel#labelStatsTitle, QLabel#sallesPage_labelStatsTitle { color: #FFFFFF; font-size: 14px; font-weight: 800; }
QLabel#labelStatutPie, QLabel#labelRoleBar, QLabel#sallesPage_labelStatutPie, QLabel#sallesPage_labelRoleBar { background-color: transparent; }

/* ================== FOOTER ================== */
QLabel#labelFooterEmployes, QLabel#labelFooterSalles { color: #475569; font-size: 11px; }

/* ================== DIALOGUES ================== */
QMessageBox, QDialog { background-color: #111827; color: #E2E8F0; }
QMessageBox QLabel, QDialog QLabel { color: #E2E8F0; font-size: 13px; }
QMessageBox QPushButton, QDialog QPushButton { background-color: #4338CA; color: #FFFFFF; border: none; border-radius: 6px; font-size: 12px; font-weight: 800; padding: 6px 16px; min-width: 70px; }
QMessageBox QPushButton:hover, QDialog QPushButton:hover { background-color: #6366F1; }

/* ================== DÉSACTIVÉS ================== */
QPushButton:disabled { background-color: #1E293B !important; color: #475569 !important; border: 1px solid #334155 !important; }
)");
}