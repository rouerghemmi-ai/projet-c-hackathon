#include "gestion_evaluateurs.h"
#include "ui_gestion_evaluateurs.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QHeaderView>
#include <QRegularExpression>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QFile>
#include <QItemSelectionModel>
#include <QPersistentModelIndex>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QPushButton>
#include <QFormLayout>
#include <QGroupBox>
#include <QComboBox>
#include <QAbstractItemView>
#include <QtMath>
#include <functional>
#include <algorithm>
#include <QSet>
#include <QMap>
#include <QBoxLayout>

GestionEvaluateurs::GestionEvaluateurs(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::GestionEvaluateurs),
      model(new QStandardItemModel(this)), ligneEnEdition(-1)
{
    ui->setupUi(this);
    setWindowTitle("Smart Hackathon Management - Gestion des évaluateurs");

    model->setHorizontalHeaderLabels({"ID évaluateur", "Nom", "Prénom", "Statut", "Projet", "Action"});
    ui->tableEvaluateurs->setModel(model);
    ui->tableEvaluateurs->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableEvaluateurs->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableEvaluateurs->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableEvaluateurs->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableEvaluateurs->verticalHeader()->setVisible(true);
    ui->tableEvaluateurs->verticalHeader()->setDefaultSectionSize(38);

    ui->comboStatut->clear();
    ui->comboStatut->addItems({"Actif", "Inactif"});
    ui->inputId->setText("E007");

    const QStringList ids = {"E001", "E002", "E003", "E004", "E005", "E006"};
    const QStringList noms = {"Ben Salah", "Trabelsi", "Gharbi", "Ayari", "Jaziri", "Chaabane"};
    const QStringList prenoms = {"Amine", "Sara", "Yassine", "Hiba", "Malek", "Omar"};
    const QStringList statuts = {"Actif", "Actif", "Inactif", "Actif", "Actif", "Inactif"};
    for (int i = 0; i < ids.size(); ++i) {
        QList<QStandardItem*> row;
        const QStringList projetsEvaluateurs = {"Smart Campus", "Cloud Platform", "Green City", "Smart Campus", "FinTech Junior", "Cloud Platform"};
        row << new QStandardItem(ids[i]) << new QStandardItem(noms[i])
            << new QStandardItem(prenoms[i]) << new QStandardItem(statuts[i])
            << new QStandardItem(projetsEvaluateurs[i]) << new QStandardItem("");
        model->appendRow(row);
    }
    reconstruireBoutonsAction();

    connect(ui->btnAjouter, &QPushButton::clicked, this, &GestionEvaluateurs::ajouterEvaluateur);
    connect(ui->btnAnnuler, &QPushButton::clicked, this, &GestionEvaluateurs::annulerFormulaire);
    connect(ui->btnRechercher, &QPushButton::clicked, this, &GestionEvaluateurs::rechercherEvaluateur);
    connect(ui->btnTrier, &QPushButton::clicked, this, &GestionEvaluateurs::trierEvaluateurs);
    connect(ui->btnExporter, &QPushButton::clicked, this, &GestionEvaluateurs::exporterEvaluateurs);
    connect(ui->tableEvaluateurs->selectionModel(), &QItemSelectionModel::currentRowChanged,
            this, [this](const QModelIndex &, const QModelIndex &) { selectionnerLigne(); });
    connect(ui->btnMetier1, &QPushButton::clicked, this, &GestionEvaluateurs::lancerTechChallenge);
    connect(ui->btnMetier2, &QPushButton::clicked, this, &GestionEvaluateurs::lancerAnalyseProjet);

    // ---- Navigation (intégration) ----
    if (auto *lay = qobject_cast<QBoxLayout*>(ui->sidebar->layout())) {
        auto *btnEmp = new QPushButton(QStringLiteral("👤   Gestion des employés"), ui->sidebar);
        btnEmp->setObjectName("menuButtonEmployes");
        btnEmp->setCursor(Qt::PointingHandCursor);
        btnEmp->setStyleSheet("background:transparent;color:#e6edff;text-align:left;padding:13px;border:none;");
        lay->insertWidget(lay->indexOf(ui->menuButton2), btnEmp);
        connect(btnEmp, &QPushButton::clicked, this, &GestionEvaluateurs::demandeEmployes);
    }
    connect(ui->menuButton,  &QPushButton::clicked, this, &GestionEvaluateurs::demandeEmployes);
    connect(ui->menuButton6, &QPushButton::clicked, this, &GestionEvaluateurs::demandeSalles);
    connect(ui->btnDeconnexion, &QPushButton::clicked, this, &GestionEvaluateurs::demandeDeconnexion);
    mettreAJourStatistiques();
}

GestionEvaluateurs::~GestionEvaluateurs() { delete ui; }

void GestionEvaluateurs::reconstruireBoutonsAction()
{
    for (int row = 0; row < model->rowCount(); ++row) {
        QWidget *container = new QWidget(ui->tableEvaluateurs);
        QHBoxLayout *layout = new QHBoxLayout(container);
        layout->setContentsMargins(4, 2, 4, 2);
        layout->setSpacing(6);
        QPushButton *modifier = new QPushButton("Modifier", container);
        QPushButton *supprimer = new QPushButton("Supprimer", container);
        modifier->setStyleSheet("QPushButton{background:#138b57;color:white;border:none;border-radius:5px;padding:6px 9px;font-weight:700;} QPushButton:hover{background:#0f7548;}");
        supprimer->setStyleSheet("QPushButton{background:#dc4545;color:white;border:none;border-radius:5px;padding:6px 9px;font-weight:700;} QPushButton:hover{background:#bd3030;}");
        layout->addWidget(modifier);
        layout->addWidget(supprimer);
        QPersistentModelIndex pIndex(model->index(row, 0));
        ui->tableEvaluateurs->setIndexWidget(model->index(row, 5), container);
        connect(modifier, &QPushButton::clicked, this, [this, pIndex]() {
            if (!pIndex.isValid()) return;
            ligneEnEdition = pIndex.row();
            ui->tableEvaluateurs->selectRow(ligneEnEdition);
            ui->inputId->setText(model->item(ligneEnEdition, 0)->text());
            ui->inputNom->setText(model->item(ligneEnEdition, 1)->text());
            ui->inputPrenom->setText(model->item(ligneEnEdition, 2)->text());
            ui->comboStatut->setCurrentText(model->item(ligneEnEdition, 3)->text());
            ui->btnAjouter->setText("Enregistrer");
        });
        connect(supprimer, &QPushButton::clicked, this, [this, pIndex]() {
            if (!pIndex.isValid()) return;
            if (QMessageBox::question(this, "Confirmer la suppression",
                                      "Supprimer cet évaluateur ?") != QMessageBox::Yes) return;
            model->removeRow(pIndex.row());
            annulerFormulaire();
            mettreAJourStatistiques();
            reconstruireBoutonsAction();
        });
    }
}

void GestionEvaluateurs::afficherMessage(const QString &titre, const QString &message)
{
    QMessageBox::information(this, titre, message);
}

bool GestionEvaluateurs::formulaireValide() const
{
    if (ui->inputId->text().trimmed().isEmpty() ||
        ui->inputNom->text().trimmed().isEmpty() ||
        ui->inputPrenom->text().trimmed().isEmpty()) {
        QMessageBox::warning(const_cast<GestionEvaluateurs*>(this), "Champs obligatoires",
                             "Veuillez saisir l'ID, le nom et le prénom de l'évaluateur.");
        return false;
    }
    return true;
}

void GestionEvaluateurs::ajouterEvaluateur()
{
    if (!formulaireValide()) return;
    const QString id = ui->inputId->text().trimmed();
    if (ligneEnEdition >= 0 && ligneEnEdition < model->rowCount()) {
        for (int r = 0; r < model->rowCount(); ++r) {
            if (r != ligneEnEdition && model->item(r, 0)->text().compare(id, Qt::CaseInsensitive) == 0) {
                QMessageBox::warning(this, "ID existant", "Un autre évaluateur utilise déjà cet ID.");
                return;
            }
        }
        model->item(ligneEnEdition, 0)->setText(id);
        model->item(ligneEnEdition, 1)->setText(ui->inputNom->text().trimmed());
        model->item(ligneEnEdition, 2)->setText(ui->inputPrenom->text().trimmed());
        model->item(ligneEnEdition, 3)->setText(ui->comboStatut->currentText());
    } else {
        for (int r = 0; r < model->rowCount(); ++r) {
            if (model->item(r, 0)->text().compare(id, Qt::CaseInsensitive) == 0) {
                QMessageBox::warning(this, "ID existant", "Cet ID évaluateur existe déjà.");
                return;
            }
        }
        QList<QStandardItem*> row;
        row << new QStandardItem(id) << new QStandardItem(ui->inputNom->text().trimmed())
            << new QStandardItem(ui->inputPrenom->text().trimmed())
            << new QStandardItem(ui->comboStatut->currentText()) << new QStandardItem("À affecter") << new QStandardItem("");
        model->appendRow(row);
    }
    reconstruireBoutonsAction();
    annulerFormulaire();
    mettreAJourStatistiques();
}

void GestionEvaluateurs::selectionnerLigne()
{
    QModelIndex index = ui->tableEvaluateurs->currentIndex();
    if (!index.isValid()) return;
    int r = index.row();
    ui->inputId->setText(model->item(r, 0)->text());
    ui->inputNom->setText(model->item(r, 1)->text());
    ui->inputPrenom->setText(model->item(r, 2)->text());
    ui->comboStatut->setCurrentText(model->item(r, 3)->text());
    ligneEnEdition = r;
}

void GestionEvaluateurs::modifierEvaluateur()
{
    ajouterEvaluateur();
}

void GestionEvaluateurs::supprimerEvaluateur()
{
    int r = ui->tableEvaluateurs->currentIndex().row();
    if (r < 0) {
        QMessageBox::information(this, "Sélection", "Sélectionnez un évaluateur à supprimer.");
        return;
    }
    if (QMessageBox::question(this, "Confirmer la suppression", "Supprimer l'évaluateur sélectionné ?") == QMessageBox::Yes) {
        model->removeRow(r);
        reconstruireBoutonsAction();
        annulerFormulaire();
        mettreAJourStatistiques();
    }
}

void GestionEvaluateurs::annulerFormulaire()
{
    ui->inputNom->clear();
    ui->inputPrenom->clear();
    ui->comboStatut->setCurrentIndex(0);
    ui->tableEvaluateurs->clearSelection();
    ligneEnEdition = -1;
    ui->btnAjouter->setText("Ajouter");
    int maxId = 0;
    for (int r = 0; r < model->rowCount(); ++r) {
        QString digits = model->item(r, 0)->text();
        digits.remove(QRegularExpression("[^0-9]"));
        maxId = qMax(maxId, digits.toInt());
    }
    ui->inputId->setText(QString("E%1").arg(maxId + 1, 3, 10, QChar('0')));
}

void GestionEvaluateurs::rechercherEvaluateur()
{
    QString q = ui->inputRecherche->text().trimmed();
    bool found = false;
    for (int r = 0; r < model->rowCount(); ++r) {
        bool match = q.isEmpty();
        for (int c = 0; c < 4; ++c)
            if (model->item(r, c)->text().contains(q, Qt::CaseInsensitive)) match = true;
        ui->tableEvaluateurs->setRowHidden(r, !match);
        if (match && !found && !q.isEmpty()) {
            ui->tableEvaluateurs->selectRow(r);
            found = true;
        }
    }
    if (!q.isEmpty() && !found) QMessageBox::information(this, "Recherche", "Aucun évaluateur trouvé.");
}

void GestionEvaluateurs::trierEvaluateurs()
{
    static bool ascending = true;
    model->sort(1, ascending ? Qt::AscendingOrder : Qt::DescendingOrder);
    ascending = !ascending;
    reconstruireBoutonsAction();
}

void GestionEvaluateurs::exporterEvaluateurs()
{
    QString path = QFileDialog::getSaveFileName(this, "Exporter les évaluateurs", "evaluateurs.csv", "Fichier CSV (*.csv)");
    if (path.isEmpty()) return;
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Export impossible", "Impossible de créer le fichier.");
        return;
    }
    QTextStream out(&file);
    out << "ID évaluateur;Nom;Prénom;Statut\n";
    for (int r = 0; r < model->rowCount(); ++r)
        out << model->item(r,0)->text() << ';' << model->item(r,1)->text() << ';'
            << model->item(r,2)->text() << ';' << model->item(r,3)->text() << '\n';
    file.close();
    afficherMessage("Export terminé", "La liste a été exportée au format CSV.");
}

void GestionEvaluateurs::mettreAJourStatistiques()
{
    int actifs = 0;
    for (int r = 0; r < model->rowCount(); ++r)
        if (model->item(r, 3)->text() == "Actif") ++actifs;
    int total = model->rowCount();
    ui->lblTotal->setText(QString::number(total));
    ui->lblActifs->setText(QString::number(actifs));
    ui->lblInactifs->setText(QString::number(total - actifs));
    int pctActifs = total ? qRound(100.0 * actifs / total) : 0;
    ui->lblRepartition->setText(QString("● Actifs : %1 %\n● Inactifs : %2 %").arg(pctActifs).arg(total ? 100 - pctActifs : 0));
}

// Métier 1 : le jury saisit les notes par critère. Le score n'est jamais automatique
// à partir d'informations inexistantes : il est calculé explicitement avec le bouton.
void GestionEvaluateurs::lancerTechChallenge()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Métier 1 — Tech Challenge Analyst");
    dialog.resize(1120, 560);
    dialog.setStyleSheet("QDialog{background:#f4f7fc;color:#0b1b42;} QLabel{color:#0b1b42;} QHeaderView::section{background:#234779;color:white;padding:9px;font-weight:bold;} QTableWidget{background:white;gridline-color:#e3eaf4;} QPushButton{padding:9px 16px;border-radius:6px;font-weight:600;}");

    QVBoxLayout *root = new QVBoxLayout(&dialog);
    QLabel *title = new QLabel("Tech Challenge Analyst — Analyse des candidats");
    title->setStyleSheet("font-size:22px;font-weight:700;color:#183f77;");
    root->addWidget(title);
    QLabel *desc = new QLabel("Le jury saisit chaque note sur 20, puis clique sur « Calculer les scores ».");
    root->addWidget(desc);
    QLabel *formula = new QLabel("Score / 20 = Technique × 50 % + Innovation × 20 % + Faisabilité × 20 % + Présentation × 10 %   |   Compatibilité = Score / 20 × 100");
    formula->setWordWrap(true);
    formula->setStyleSheet("background:#eaf1fd;padding:10px;border-radius:6px;");
    root->addWidget(formula);

    QTableWidget *table = new QTableWidget(4, 8, &dialog);
    table->setHorizontalHeaderLabels({"Candidat", "Projet", "Technique /20", "Innovation /20", "Faisabilité /20", "Présentation /20", "Score /20", "Compatibilité / Recommandation"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->verticalHeader()->setVisible(false);
    table->setAlternatingRowColors(true);
    const QStringList candidates = {"Amine Ben Salah", "Sara Trabelsi", "Hiba Ayari", "Malek Jaziri"};
    const QStringList projects = {"Smart Campus", "Green City", "Health AI", "FinTech Junior"};
    // Notes d'exemple pour montrer le fonctionnement; elles restent modifiables par le jury.
    const double initial[4][4] = {{18, 17, 19, 18}, {17, 18, 16, 17}, {19, 18, 17, 18}, {16, 15, 17, 16}};
    for (int r = 0; r < 4; ++r) {
        table->setItem(r, 0, new QTableWidgetItem(candidates[r]));
        table->setItem(r, 1, new QTableWidgetItem(projects[r]));
        for (int c = 0; c < 4; ++c) {
            QDoubleSpinBox *score = new QDoubleSpinBox(table);
            score->setRange(0, 20);
            score->setDecimals(1);
            score->setSingleStep(0.5);
            score->setValue(initial[r][c]);
            table->setCellWidget(r, c + 2, score);
        }
        table->setItem(r, 6, new QTableWidgetItem("—"));
        table->setItem(r, 7, new QTableWidgetItem("À calculer"));
    }
    root->addWidget(table, 1);
    QHBoxLayout *buttons = new QHBoxLayout();
    QPushButton *calculate = new QPushButton("Calculer les scores");
    calculate->setStyleSheet("background:#2468df;color:white;");
    QPushButton *close = new QPushButton("Fermer");
    close->setStyleSheet("background:#eef2f8;color:#18345d;border:1px solid #d5deeb;");
    buttons->addStretch();
    buttons->addWidget(calculate);
    buttons->addWidget(close);
    root->addLayout(buttons);

    connect(calculate, &QPushButton::clicked, &dialog, [table]() {
        for (int r = 0; r < table->rowCount(); ++r) {
            double technique = qobject_cast<QDoubleSpinBox*>(table->cellWidget(r,2))->value();
            double innovation = qobject_cast<QDoubleSpinBox*>(table->cellWidget(r,3))->value();
            double faisabilite = qobject_cast<QDoubleSpinBox*>(table->cellWidget(r,4))->value();
            double presentation = qobject_cast<QDoubleSpinBox*>(table->cellWidget(r,5))->value();
            double score = technique * 0.50 + innovation * 0.20 + faisabilite * 0.20 + presentation * 0.10;
            double compat = score / 20.0 * 100.0;
            table->item(r,6)->setText(QString::number(score, 'f', 2));
            QString recommandation = (score >= 14.0 && compat >= 75.0) ? "Recommandé" : "À examiner";
            table->item(r,7)->setText(QString("%1 % — %2").arg(compat, 0, 'f', 1).arg(recommandation));
        }
    });
    connect(close, &QPushButton::clicked, &dialog, &QDialog::accept);
    dialog.exec();
}

// Métier 2 : nouvelle interface centrée sur une affectation simple et stricte.
// Un projet sélectionné -> un seul évaluateur actif dont la spécialité correspond.
void GestionEvaluateurs::lancerAnalyseProjet()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Métier 2 — Affectation d'un évaluateur par spécialité");
    dialog.resize(1120, 760);
    dialog.setMinimumSize(900, 620);
    dialog.setStyleSheet(
        "QDialog{background:#f4f7fc;color:#10284b;}"
        "QLabel{color:#10284b;font-size:10pt;}"
        "QLabel#pageTitle{font-size:21pt;font-weight:700;color:#234779;}"
        "QLabel#subTitle{color:#526780;font-size:10pt;}"
        "QGroupBox{background:white;border:1px solid #d8e2ef;border-radius:10px;margin-top:12px;padding:12px;font-weight:600;}"
        "QGroupBox::title{subcontrol-origin:margin;left:12px;padding:0 5px;color:#234779;}"
        "QHeaderView::section{background:#234779;color:white;padding:10px;border:0;font-weight:700;}"
        "QTableWidget{background:white;alternate-background-color:#f1f5fb;gridline-color:#e0e8f2;border:1px solid #d8e2ef;border-radius:6px;}"
        "QLineEdit,QComboBox{background:white;border:1px solid #cbd8e8;border-radius:6px;padding:8px;color:#10284b;}"
        "QPushButton{padding:9px 15px;border-radius:6px;font-weight:600;}"
        "QPushButton#primary{background:#138b57;color:white;border:0;}"
        "QPushButton#secondary{background:#2468df;color:white;border:0;}"
        "QPushButton#closeButton{background:#e9eef6;color:#234779;border:1px solid #d2dceb;}"
    );

    QVBoxLayout *root = new QVBoxLayout(&dialog);
    root->setContentsMargins(22, 18, 22, 18);
    root->setSpacing(12);

    QLabel *title = new QLabel("Affectation intelligente des évaluateurs");
    title->setObjectName("pageTitle");
    root->addWidget(title);
    QLabel *subtitle = new QLabel("Choisissez un projet : un seul évaluateur actif sera proposé, uniquement si sa spécialité correspond au besoin du projet.");
    subtitle->setObjectName("subTitle");
    subtitle->setWordWrap(true);
    root->addWidget(subtitle);

    struct Project { QString name; QString skill; };
    const QList<Project> projects = {
        {"Smart Campus", "Smart Campus"},
        {"Green City", "Smart City"},
        {"Cloud Platform", "Cloud"},
        {"FinTech Junior", "Business"},
        {"Réseaux", "Réseaux"},
        {"Développement logiciel", "Développement"}
    };

    QGroupBox *projectBox = new QGroupBox("1. Projet à évaluer");
    QFormLayout *projectForm = new QFormLayout(projectBox);
    projectForm->setLabelAlignment(Qt::AlignLeft);
    QComboBox *projectChoice = new QComboBox;
    for (const Project &p : projects) projectChoice->addItem(p.name);
    QLineEdit *requiredSkill = new QLineEdit;
    requiredSkill->setReadOnly(true);
    requiredSkill->setStyleSheet("background:#f4f7fc;font-weight:600;");
    projectForm->addRow("Projet :", projectChoice);
    projectForm->addRow("Spécialité demandée :", requiredSkill);
    root->addWidget(projectBox);

    QGroupBox *evaluatorsBox = new QGroupBox("2. Évaluateurs et leurs spécialités");
    QVBoxLayout *evaluatorsLayout = new QVBoxLayout(evaluatorsBox);
    QLabel *tableHint = new QLabel("Les spécialités sont modifiables. Un évaluateur inactif ne peut pas être affecté.");
    tableHint->setWordWrap(true);
    evaluatorsLayout->addWidget(tableHint);
    QTableWidget *evaluatorTable = new QTableWidget(0, 4);
    evaluatorTable->setHorizontalHeaderLabels({"ID", "Évaluateur", "Statut", "Spécialité(s)"});
    evaluatorTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    evaluatorTable->verticalHeader()->setVisible(false);
    evaluatorTable->setAlternatingRowColors(true);
    evaluatorTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    evaluatorTable->setMinimumHeight(190);
    evaluatorsLayout->addWidget(evaluatorTable);
    root->addWidget(evaluatorsBox, 3);

    QHBoxLayout *buttons = new QHBoxLayout;
    QPushButton *assign = new QPushButton("Trouver un évaluateur");
    assign->setObjectName("primary");
    QPushButton *exportButton = new QPushButton("Exporter le résultat CSV");
    exportButton->setObjectName("secondary");
    QPushButton *close = new QPushButton("Fermer");
    close->setObjectName("closeButton");
    buttons->addWidget(assign);
    buttons->addWidget(exportButton);
    buttons->addStretch();
    buttons->addWidget(close);
    root->addLayout(buttons);

    QGroupBox *resultBox = new QGroupBox("3. Résultat de l'affectation");
    QVBoxLayout *resultLayout = new QVBoxLayout(resultBox);
    QLabel *resultStatus = new QLabel("Sélectionnez un projet puis cliquez sur « Trouver un évaluateur ».");
    resultStatus->setWordWrap(true);
    resultStatus->setStyleSheet("background:#eaf1fb;border:1px solid #d3e1f4;border-radius:7px;padding:12px;font-weight:600;");
    resultLayout->addWidget(resultStatus);
    QTableWidget *resultTable = new QTableWidget(0, 4);
    resultTable->setHorizontalHeaderLabels({"Projet", "Évaluateur affecté", "Spécialité correspondante", "Décision"});
    resultTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    resultTable->verticalHeader()->setVisible(false);
    resultTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    resultTable->setMaximumHeight(125);
    resultLayout->addWidget(resultTable);
    root->addWidget(resultBox, 2);

    auto normalize = [](QString s) {
        s = s.trimmed().toCaseFolded();
        s.replace(QRegularExpression("\\s+"), " ");
        return s;
    };
    auto splitSkills = [normalize](const QString &s) {
        QSet<QString> out;
        const QStringList parts = s.split(QRegularExpression("[,;]"), Qt::SkipEmptyParts);
        for (const QString &part : parts) {
            const QString skill = normalize(part);
            if (!skill.isEmpty()) out.insert(skill);
        }
        return out;
    };

    // La spécialité est enregistrée dans la table d'interface et peut être ajustée par l'utilisateur.
    for (int r = 0; r < model->rowCount(); ++r) {
        const int row = evaluatorTable->rowCount();
        evaluatorTable->insertRow(row);
        const QString id = model->item(r, 0)->text();
        const QString fullName = model->item(r, 1)->text() + " " + model->item(r, 2)->text();
        const QString status = model->item(r, 3)->text().trimmed();
        QString skills;
        if (fullName.contains("Amine", Qt::CaseInsensitive)) skills = "IoT, Smart Campus, Réseaux";
        else if (fullName.contains("Sara", Qt::CaseInsensitive)) skills = "Cloud, DevOps, Architecture logicielle";
        else if (fullName.contains("Yassine", Qt::CaseInsensitive)) skills = "Environnement, Énergie, Smart City";
        else if (fullName.contains("Hiba", Qt::CaseInsensitive)) skills = "IoT, Smart City, Réseaux";
        else if (fullName.contains("Malek", Qt::CaseInsensitive)) skills = "Business, UX, Innovation";
        else skills = "Développement";
        evaluatorTable->setItem(row, 0, new QTableWidgetItem(id));
        evaluatorTable->setItem(row, 1, new QTableWidgetItem(fullName));
        evaluatorTable->setItem(row, 2, new QTableWidgetItem(status));
        evaluatorTable->setItem(row, 3, new QTableWidgetItem(skills));
        for (int c = 0; c < 3; ++c) evaluatorTable->item(row, c)->setFlags(evaluatorTable->item(row, c)->flags() & ~Qt::ItemIsEditable);
        if (status.compare("Actif", Qt::CaseInsensitive) != 0) {
            for (int c = 0; c < 4; ++c) evaluatorTable->item(row, c)->setForeground(QBrush(QColor("#929bad")));
        }
    }

    auto updateRequiredSkill = [&]() {
        const int index = projectChoice->currentIndex();
        requiredSkill->setText(index >= 0 && index < projects.size() ? projects[index].skill : QString());
        resultTable->setRowCount(0);
        resultStatus->setText("Projet sélectionné : " + projectChoice->currentText() + ". Cliquez sur « Trouver un évaluateur » pour obtenir une seule proposition.");
        resultStatus->setStyleSheet("background:#eaf1fb;border:1px solid #d3e1f4;border-radius:7px;padding:12px;font-weight:600;");
    };
    QObject::connect(projectChoice, QOverload<int>::of(&QComboBox::currentIndexChanged), &dialog, [&, updateRequiredSkill](int) { updateRequiredSkill(); });
    updateRequiredSkill();

    QString lastProject, lastEvaluator, lastSkill, lastDecision;
    QObject::connect(assign, &QPushButton::clicked, &dialog, [&]() {
        const int index = projectChoice->currentIndex();
        if (index < 0 || index >= projects.size()) return;
        const Project p = projects[index];
        const QString wanted = normalize(p.skill);
        int bestRow = -1;
        int bestScore = 0;
        QString matchedSkill;
        for (int r = 0; r < evaluatorTable->rowCount(); ++r) {
            if (evaluatorTable->item(r, 2)->text().compare("Actif", Qt::CaseInsensitive) != 0) continue;
            const QString skillsText = evaluatorTable->item(r, 3)->text();
            const QSet<QString> skills = splitSkills(skillsText);
            // Une spécialité doit correspondre exactement; développeur != réseau.
            if (!skills.contains(wanted)) continue;
            const int score = 1;
            if (score > bestScore) { bestScore = score; bestRow = r; matchedSkill = p.skill; }
        }
        resultTable->setRowCount(0);
        lastProject = p.name;
        if (bestRow < 0) {
            lastEvaluator.clear();
            lastSkill.clear();
            lastDecision = "Aucun évaluateur actif compatible";
            resultStatus->setText("Aucun évaluateur actif ne possède exactement la spécialité « " + p.skill + " ». Vérifiez les spécialités du tableau ou activez un évaluateur compatible.");
            resultStatus->setStyleSheet("background:#fff4e5;border:1px solid #f1d19a;border-radius:7px;padding:12px;font-weight:600;color:#8a4b08;");
            const int row = resultTable->rowCount(); resultTable->insertRow(row);
            const QStringList vals = {p.name, "Aucun évaluateur trouvé", p.skill, "Non affecté"};
            for (int c = 0; c < vals.size(); ++c) resultTable->setItem(row, c, new QTableWidgetItem(vals[c]));
            return;
        }
        lastEvaluator = evaluatorTable->item(bestRow, 1)->text();
        lastSkill = matchedSkill;
        lastDecision = "Affecté";
        resultStatus->setText("Évaluateur trouvé : " + lastEvaluator + " — spécialité correspondante : " + matchedSkill + ". Une seule personne est affectée à ce projet.");
        resultStatus->setStyleSheet("background:#e7f7ee;border:1px solid #b9e4ca;border-radius:7px;padding:12px;font-weight:600;color:#12653f;");
        const int row = resultTable->rowCount(); resultTable->insertRow(row);
        const QStringList vals = {p.name, lastEvaluator, lastSkill, lastDecision};
        for (int c = 0; c < vals.size(); ++c) resultTable->setItem(row, c, new QTableWidgetItem(vals[c]));
    });

    QObject::connect(exportButton, &QPushButton::clicked, &dialog, [&]() {
        if (lastProject.isEmpty()) {
            QMessageBox::information(&dialog, "Aucun résultat", "Lancez d'abord une affectation.");
            return;
        }
        const QString path = QFileDialog::getSaveFileName(&dialog, "Exporter le résultat", "affectation_evaluateur.csv", "CSV (*.csv)");
        if (path.isEmpty()) return;
        QFile file(path);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::warning(&dialog, "Erreur", "Impossible de créer le fichier CSV.");
            return;
        }
        QTextStream out(&file);
        out << "Projet;Evaluateur;Specialite;Decision\n";
        out << lastProject << ';' << lastEvaluator << ';' << lastSkill << ';' << lastDecision << '\n';
    });
    QObject::connect(close, &QPushButton::clicked, &dialog, &QDialog::accept);
    dialog.exec();
}

