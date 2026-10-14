#include "integration.h"
#include "ui_integration.h"

#include <QPushButton>
#include <QStackedWidget>
#include <QMessageBox>
#include <QDialog>
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

    ui->stackedWidget->setCurrentIndex(0);

    appliquerTheme();
    connecterLogin();
    connecterNavigation();
    connecterChatbot();

    remplirTableEmployes();
    dessinerStatistiques();
}

integration::~integration()
{
    delete ui;
}

/* ============================================================
 *  HELPERS — DONNÉES ALÉATOIRES
 * ============================================================ */
static QString randomNom()
{
    static const QStringList noms = {
        "Benali", "Trabelsi", "Gharbi", "Mansouri", "Khelifi",
        "Jebali", "Sassi", "Bouazizi", "Hamdi", "Mejri",
        "Chaabane", "Ferchichi", "Zouari", "Ayadi", "Krichen",
        "Ben Amor", "Riahi", "Karoui", "Hammami", "Dridi"
    };
    return noms[QRandomGenerator::global()->bounded(noms.size())];
}

static QString randomPrenom()
{
    static const QStringList prenoms = {
        "Ahmed", "Sarra", "Yassine", "Leila", "Mehdi",
        "Nour", "Karim", "Ines", "Rania", "Sami",
        "Amine", "Mariem", "Hatem", "Salma", "Walid",
        "Dorra", "Mohamed", "Yasmine", "Anis", "Lina"
    };
    return prenoms[QRandomGenerator::global()->bounded(prenoms.size())];
}

/* ============================================================
 *  REMPLIR LE TABLEAU DES EMPLOYÉS
 *  → 2 rôles seulement : Administrateur / Responsable RH
 * ============================================================ */
void integration::remplirTableEmployes()
{
    QString styleModifier = R"(
        QPushButton {
            background-color: #4338CA;
            color: #FFFFFF;
            border: none;
            border-radius: 6px;
            font-size: 10px;
            font-weight: 800;
            padding: 6px 12px;
            min-width: 90px;
        }
        QPushButton:hover { background-color: #6366F1; }
    )";

    QString styleSupprimer = R"(
        QPushButton {
            background-color: #B91C1C;
            color: #FFFFFF;
            border: none;
            border-radius: 6px;
            font-size: 10px;
            font-weight: 800;
            padding: 6px 12px;
            min-width: 90px;
        }
        QPushButton:hover { background-color: #F87171; }
    )";

    auto creerWidgetActions = [this, styleModifier, styleSupprimer](int row) {
        auto *container = new QWidget;
        auto *lay = new QHBoxLayout(container);
        lay->setContentsMargins(4, 2, 4, 2);
        lay->setSpacing(8);

        auto *btnModif = new QPushButton("MODIFIER");
        btnModif->setCursor(Qt::PointingHandCursor);
        btnModif->setStyleSheet(styleModifier);
        btnModif->setMinimumWidth(95);

        auto *btnSuppr = new QPushButton("SUPPRIMER");
        btnSuppr->setCursor(Qt::PointingHandCursor);
        btnSuppr->setStyleSheet(styleSupprimer);
        btnSuppr->setMinimumWidth(95);

        lay->addWidget(btnModif);
        lay->addWidget(btnSuppr);
        lay->addStretch();

        QObject::connect(btnModif, &QPushButton::clicked, this, [this, row]() {
            QMessageBox::information(this, "Modifier",
                                     QString("Modifier la ligne %1\n(Design uniquement)").arg(row + 1));
        });

        QObject::connect(btnSuppr, &QPushButton::clicked, this, [this, row]() {
            QMessageBox::question(this, "Supprimer",
                                  QString("Supprimer la ligne %1 ?\n(Design uniquement)").arg(row + 1));
        });

        return container;
    };

    auto *t = ui->tableEmployes;
    const int NB = 8;
    t->setRowCount(NB);

    for (int i = 0; i < NB; ++i) {
        QString nom = randomNom();
        QString prenom = randomPrenom();
        QString id = QString("EMP-%1").arg(i + 1, 3, 10, QChar('0'));

        // ⚠️ PLUS QUE 2 RÔLES : Administrateur / Responsable RH
        QString role = (i % 2 == 0) ? "Administrateur" : "Responsable RH";
        QString statut = (i % 4 == 0) ? "Inactif" : "Actif";

        t->setItem(i, 0, new QTableWidgetItem(id));
        t->setItem(i, 1, new QTableWidgetItem(nom));
        t->setItem(i, 2, new QTableWidgetItem(prenom));
        t->setItem(i, 3, new QTableWidgetItem("••••••••"));
        t->setItem(i, 4, new QTableWidgetItem(role));

        auto *itemStatut = new QTableWidgetItem(statut);
        itemStatut->setForeground(statut == "Actif"
                                      ? QColor("#34D399") : QColor("#F87171"));
        t->setItem(i, 5, itemStatut);

        t->setCellWidget(i, 6, creerWidgetActions(i));
    }

    // Configuration finale du tableau
    t->verticalHeader()->setVisible(false);
    t->setEditTriggers(QAbstractItemView::NoEditTriggers);
    t->setSelectionBehavior(QAbstractItemView::SelectRows);
    t->setAlternatingRowColors(true);

    int lastCol = t->columnCount() - 1;
    for (int c = 0; c < lastCol; ++c) {
        t->horizontalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);
    }
    if (lastCol >= 0) {
        t->horizontalHeader()->setSectionResizeMode(lastCol, QHeaderView::Fixed);
        t->setColumnWidth(lastCol, 240);
    }
    t->verticalHeader()->setDefaultSectionSize(46);
}

/* ============================================================
 *  DESSINER LES STATISTIQUES (camembert + barres horizontales)
 * ============================================================ */
void integration::dessinerStatistiques()
{
    /* ---------- 1) CAMEMBERT : Actif / Inactif ---------- */
    {
        QPixmap pix(300, 260);
        pix.fill(Qt::transparent);
        QPainter p(&pix);
        p.setRenderHint(QPainter::Antialiasing);

        // Compte depuis le tableau
        int actifs = 0, inactifs = 0;
        for (int i = 0; i < ui->tableEmployes->rowCount(); ++i) {
            QString s = ui->tableEmployes->item(i, 5)->text();
            if (s == "Actif") actifs++; else inactifs++;
        }
        int total = actifs + inactifs;
        if (total == 0) total = 1;

        // Camembert centré
        QRectF rect(20, 30, 160, 160);
        double angleActif = 360.0 * actifs / total;

        // Partie Actif (vert)
        p.setBrush(QColor("#34D399"));
        p.setPen(Qt::NoPen);
        p.drawPie(rect, 90 * 16, -angleActif * 16);

        // Partie Inactif (rouge)
        p.setBrush(QColor("#F87171"));
        p.drawPie(rect, (90 - angleActif) * 16,
                  -(360 - angleActif) * 16);

        // Légende
        p.setPen(QColor("#E5E7EB"));
        QFont f = p.font();
        f.setPointSize(9);
        f.setBold(true);
        p.setFont(f);

        p.setBrush(QColor("#34D399"));
        p.drawRect(200, 60, 12, 12);
        p.drawText(220, 71, QString("Actif (%1)").arg(actifs));

        p.setBrush(QColor("#F87171"));
        p.drawRect(200, 90, 12, 12);
        p.drawText(220, 101, QString("Inactif (%1)").arg(inactifs));

        f.setPointSize(8);
        f.setBold(false);
        p.setFont(f);
        p.setPen(QColor("#94A3B8"));
        p.drawText(20, 220, "Répartition par statut");

        p.end();
        ui->labelStatutPie->setPixmap(pix);
    }

    /* ---------- 2) BARRES HORIZONTALES : par rôle ---------- */
    {
        QPixmap pix(460, 260);
        pix.fill(Qt::transparent);
        QPainter p(&pix);
        p.setRenderHint(QPainter::Antialiasing);

        // Compte par rôle
        QMap<QString, int> roles;
        for (int i = 0; i < ui->tableEmployes->rowCount(); ++i) {
            QString r = ui->tableEmployes->item(i, 4)->text();
            roles[r]++;
        }

        int maxVal = 1;
        for (int v : roles) if (v > maxVal) maxVal = v;

        // Couleurs par rôle (2 rôles)
        auto couleur = [](const QString &r) -> QColor {
            if (r == "Administrateur")  return QColor("#6366F1");
            if (r == "Responsable RH")  return QColor("#FBBF24");
            return QColor("#34D399");
        };

        int y = 40;
        const int barMaxW = 320;
        const int barH = 30;

        QFont f = p.font();
        f.setPointSize(9);
        f.setBold(true);
        p.setFont(f);

        for (auto it = roles.begin(); it != roles.end(); ++it) {
            // Label
            p.setPen(QColor("#E5E7EB"));
            p.drawText(10, y + 20, it.key());

            // Fond de barre
            p.setPen(Qt::NoPen);
            p.setBrush(QColor("#1F2937"));
            p.drawRoundedRect(120, y, barMaxW, barH, 6, 6);

            // Barre remplie
            int w = int(barMaxW * (double)it.value() / maxVal);
            p.setBrush(couleur(it.key()));
            p.drawRoundedRect(120, y, w, barH, 6, 6);

            // Valeur
            p.setPen(QColor("#FFFFFF"));
            p.drawText(120 + w + 10, y + 20, QString::number(it.value()));

            y += barH + 25;
        }

        f.setPointSize(8);
        f.setBold(false);
        p.setFont(f);
        p.setPen(QColor("#94A3B8"));
        p.drawText(10, 240, "Nombre d'employés par rôle");

        p.end();
        ui->labelRoleBar->setPixmap(pix);
    }
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

/* ============================================================
 *  LOGIN
 * ============================================================ */
void integration::connecterLogin()
{
    if (ui->pushButtonConnexion)
        QObject::connect(ui->pushButtonConnexion, &QPushButton::clicked,
                         this, &integration::onConnexion);
    if (ui->pushButtonCGU)
        QObject::connect(ui->pushButtonCGU, &QPushButton::clicked,
                         this, &integration::onCGU);
    if (ui->pushButtonPolitique)
        QObject::connect(ui->pushButtonPolitique, &QPushButton::clicked,
                         this, &integration::onPolitique);
    if (ui->pushButtonAPropos)
        QObject::connect(ui->pushButtonAPropos, &QPushButton::clicked,
                         this, &integration::onAPropos);
    if (ui->lineEditMotDePasse)
        QObject::connect(ui->lineEditMotDePasse, &QLineEdit::returnPressed,
                         this, &integration::onConnexion);
}

/* ============================================================
 *  NAVIGATION — BOUTONS NAVBAR
 *  Seuls Notifications / Profil / Déconnexion sont actifs.
 *  Les autres sont purement visuels (aucun connect).
 * ============================================================ */
void integration::connecterNavigation()
{
    if (ui->btnNotifications) QObject::connect(ui->btnNotifications, &QPushButton::clicked, this, &integration::onNotifications);
    if (ui->btnProfil)        QObject::connect(ui->btnProfil,        &QPushButton::clicked, this, &integration::onProfil);
    if (ui->btnDeconnexion)   QObject::connect(ui->btnDeconnexion,   &QPushButton::clicked, this, &integration::onDeconnexion);
}

/* ============================================================
 *  CHATBOT
 * ============================================================ */
void integration::connecterChatbot()
{
    if (ui->btnEnvoyer)
        QObject::connect(ui->btnEnvoyer, &QPushButton::clicked,
                         this, &integration::onEnvoyerQuestion);
    if (ui->leQuestion)
        QObject::connect(ui->leQuestion, &QLineEdit::returnPressed,
                         this, &integration::onEnvoyerQuestion);

    // Message d'accueil
    if (ui->textChatbot) {
        ui->textChatbot->setHtml(R"(
            <div style='color:#E5E7EB;font-family:Arial;font-size:12px;'>
            <p><b style='color:#818CF8;'>HACKNOVA Bot :</b>
            Bonjour 👋 Je suis votre assistant. Posez-moi une question
            sur les employés, les rôles, les statuts, etc.</p>
            </div>
        )");
    }
}

void integration::onEnvoyerQuestion()
{
    if (!ui->leQuestion || !ui->textChatbot) return;

    QString q = ui->leQuestion->text().trimmed();
    if (q.isEmpty()) return;

    QString reponse;
    QString ql = q.toLower();

    if (ql.contains("bonjour") || ql.contains("salut") || ql.contains("hello")) {
        reponse = "Bonjour 👋 Comment puis-je vous aider ?";
    }
    else if (ql.contains("combien") && ql.contains("employ")) {
        reponse = QString("Il y a actuellement <b>%1 employés</b> enregistrés.")
                      .arg(ui->tableEmployes->rowCount());
    }
    else if (ql.contains("actif")) {
        int n = 0;
        for (int i = 0; i < ui->tableEmployes->rowCount(); ++i)
            if (ui->tableEmployes->item(i, 5)->text() == "Actif") n++;
        reponse = QString("<b>%1 employés</b> sont actifs.").arg(n);
    }
    else if (ql.contains("inactif")) {
        int n = 0;
        for (int i = 0; i < ui->tableEmployes->rowCount(); ++i)
            if (ui->tableEmployes->item(i, 5)->text() == "Inactif") n++;
        reponse = QString("<b>%1 employés</b> sont inactifs.").arg(n);
    }
    else if (ql.contains("admin")) {
        int n = 0;
        for (int i = 0; i < ui->tableEmployes->rowCount(); ++i)
            if (ui->tableEmployes->item(i, 4)->text() == "Administrateur") n++;
        reponse = QString("Il y a <b>%1 administrateurs</b>.").arg(n);
    }
    else if (ql.contains("rh") || ql.contains("responsable")) {
        int n = 0;
        for (int i = 0; i < ui->tableEmployes->rowCount(); ++i)
            if (ui->tableEmployes->item(i, 4)->text() == "Responsable RH") n++;
        reponse = QString("Il y a <b>%1 responsables RH</b>.").arg(n);
    }
    else if (ql.contains("rôle") || ql.contains("role")) {
        QMap<QString, int> roles;
        for (int i = 0; i < ui->tableEmployes->rowCount(); ++i)
            roles[ui->tableEmployes->item(i, 4)->text()]++;
        QString liste;
        for (auto it = roles.begin(); it != roles.end(); ++it)
            liste += QString("<li>%1 : <b>%2</b></li>").arg(it.key()).arg(it.value());
        reponse = "Répartition par rôle :<ul>" + liste + "</ul>";
    }
    else if (ql.contains("statistique") || ql.contains("graphique")) {
        reponse = "Consultez les <b>statistiques</b> à droite : "
                  "un camembert pour les statuts et des barres pour les rôles.";
    }
    else if (ql.contains("merci")) {
        reponse = "Avec plaisir 😊";
    }
    else if (ql.contains("aide") || ql.contains("help")) {
        reponse = "Je peux vous renseigner sur : "
                  "<ul><li>le nombre d'employés</li>"
                  "<li>les actifs / inactifs</li>"
                  "<li>les rôles (Administrateur, Responsable RH)</li>"
                  "<li>les statistiques</li></ul>";
    }
    else {
        reponse = "Je n'ai pas compris votre question. Tapez <b>aide</b> pour voir ce que je sais faire.";
    }

    // Ajoute la question + réponse dans le textBrowser
    QString html = ui->textChatbot->toHtml();
    html += QString(R"(
        <div style='color:#E5E7EB;font-family:Arial;font-size:12px;margin-top:8px;'>
        <p><b style='color:#34D399;'>Vous :</b> %1</p>
        <p><b style='color:#818CF8;'>Bot :</b> %2</p>
        </div>
    )").arg(q.toHtmlEscaped(), reponse);

    ui->textChatbot->setHtml(html);
    ui->textChatbot->verticalScrollBar()->setValue(
        ui->textChatbot->verticalScrollBar()->maximum());

    ui->leQuestion->clear();
}

/* ============================================================
 *  LOGIN
 * ============================================================ */
void integration::onConnexion()
{
    const QString user = ui->lineEditIdentifiant->text().trimmed();
    const QString pass = ui->lineEditMotDePasse->text();

    if (user.isEmpty() || pass.isEmpty()) {
        QMessageBox::warning(this, "Connexion",
                             "Veuillez saisir votre identifiant et votre mot de passe.");
        return;
    }
    goEmployes();
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
    dlg.setMinimumSize(600, 520);

    auto *lay = new QVBoxLayout(&dlg);
    lay->setContentsMargins(20, 20, 20, 20);
    lay->setSpacing(12);

    auto *txt = new QTextBrowser;
    txt->setHtml(html);
    txt->setOpenExternalLinks(true);
    lay->addWidget(txt, 1);

    auto *btn = new QPushButton("FERMER");
    btn->setCursor(Qt::PointingHandCursor);
    btn->setMinimumHeight(38);
    QObject::connect(btn, &QPushButton::clicked, &dlg, &QDialog::accept);
    lay->addWidget(btn, 0, Qt::AlignRight);

    dlg.exec();
}

void integration::onCGU()
{
    afficherDialogue(this, "Conditions Générales d'Utilisation", R"(
        <div style='color:#E5E7EB;font-family:Arial;font-size:13px;line-height:1.6;'>
        <h2 style='color:#818CF8;'>1. Objet</h2>
        <p>Les présentes conditions régissent l'utilisation de la plateforme HACKNOVA.</p>
        <h2 style='color:#818CF8;'>2. Accès au service</h2>
        <p>L'accès est réservé aux utilisateurs autorisés par l'organisation.</p>
        <h2 style='color:#818CF8;'>3. Données personnelles</h2>
        <p>Les données sont traitées conformément à la réglementation en vigueur.</p>
        <h2 style='color:#818CF8;'>4. Responsabilités</h2>
        <p>L'utilisateur s'engage à ne pas divulguer ses identifiants.</p>
        </div>
    )");
}

void integration::onPolitique()
{
    afficherDialogue(this, "Politique d'utilisation", R"(
        <div style='color:#E5E7EB;font-family:Arial;font-size:13px;line-height:1.6;'>
        <h2 style='color:#818CF8;'>Confidentialité</h2>
        <p>Vos données ne sont jamais partagées avec des tiers.</p>
        <h2 style='color:#818CF8;'>Sécurité</h2>
        <p>Les mots de passe sont chiffrés et les sessions sécurisées.</p>
        <h2 style='color:#818CF8;'>Cookies</h2>
        <p>Aucun cookie de tracking n'est utilisé par la plateforme.</p>
        </div>
    )");
}

void integration::onAPropos()
{
    afficherDialogue(this, "À propos de HACKNOVA", R"(
        <div style='color:#E5E7EB;text-align:center;font-family:Arial;'>
        <h1 style='color:#818CF8;letter-spacing:5px;'>HACKNOVA</h1>
        <p style='color:#94A3B8;'>Smart Hackathon Management</p>
        <hr/>
        <p><b>Version :</b> 1.0.0</p>
        <p><b>Établissement :</b> Esprit School of Engineering</p>
        <p><b>Année :</b> 2025</p>
        <hr/>
        <h2 style='color:#818CF8;letter-spacing:3px;'>GROUPE EL FALLEGA</h2>
        <p style='color:#94A3B8;'>Groupe de développement</p>
        <hr/>
        <p style='color:#64748B;font-size:11px;'>
        © 2025 HACKNOVA. Tous droits réservés.</p>
        </div>
    )");
}

void integration::onNotifications()
{
    afficherDialogue(this, "Notifications", R"(
        <div style='color:#E5E7EB;font-family:Arial;font-size:13px;'>
        <p style='color:#818CF8;font-weight:800;'>Nouveau employé</p>
        <p style='color:#94A3B8;font-size:12px;'>Un nouvel employé a été ajouté il y a 5 min.</p>
        <hr/>
        <p style='color:#FBBF24;font-weight:800;'>Rappel</p>
        <p style='color:#94A3B8;font-size:12px;'>Vérifiez les statuts des employés inactifs.</p>
        </div>
    )");
}

void integration::onProfil()
{
    afficherDialogue(this, "Profil utilisateur", R"(
        <div style='color:#E5E7EB;text-align:center;font-family:Arial;'>
        <h1 style='color:#818CF8;'>HN</h1>
        <h2 style='color:#FFFFFF;'>Utilisateur</h2>
        <p style='color:#94A3B8;'><b>Rôle :</b> Administrateur</p>
        <p style='color:#94A3B8;'><b>Email :</b> user@hacknova.tn</p>
        <p style='color:#94A3B8;'><b>Statut :</b>
        <span style='color:#34D399;'>Actif</span></p>
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
        goLogin();
    }
}

/* ============================================================
 *  THÈME DARK MODE
 * ============================================================ */
void integration::appliquerTheme()
{
    this->setStyleSheet(R"(
QMainWindow, QWidget {
    background-color: #0B1020;
    color: #E5E7EB;
    font-family: "Arial";
}
QWidget#centralWidget { background-color: #0B1020; }

QToolTip {
    background-color: #1F2937; color: #E5E7EB;
    border: 1px solid #6366F1; padding: 6px 10px;
    border-radius: 6px; font-size: 11px;
}

QScrollArea { background-color: #0B1020; border: none; }
QScrollArea > QWidget > QWidget { background-color: #0B1020; }
QScrollBar:vertical {
    background: #111827; width: 10px; border-radius: 5px; margin: 0;
}
QScrollBar::handle:vertical {
    background: #374151; border-radius: 5px; min-height: 30px;
}
QScrollBar::handle:vertical:hover { background: #6366F1; }
QScrollBar::add-line:vertical,
QScrollBar::sub-line:vertical { height: 0; }

QFrame#frameNavbar {
    background-color: #111827; border: none;
    border-bottom: 1px solid #1F2937; border-radius: 0;
}

QLabel#labelNavLogo {
    background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
        stop:0 #6366F1, stop:1 #8B5CF6);
    color: #FFFFFF; font-size: 17px; font-weight: 900;
    border-radius: 12px; qproperty-alignment: AlignCenter;
}

QLabel#labelNavBrand {
    color: #FFFFFF; font-size: 15px; font-weight: 900; letter-spacing: 2px;
}
QLabel#labelNavSub {
    color: #818CF8; font-size: 8px; font-weight: 700; letter-spacing: 2px;
}

QPushButton#btnDashboard, QPushButton#btnEmployes,
QPushButton#btnSalles, QPushButton#btnEvaluateurs,
QPushButton#btnParticipants, QPushButton#btnSponsors,
QPushButton#btnProjets, QPushButton#btnNotifications,
QPushButton#btnProfil {
    background-color: transparent; color: #C7D2FE;
    font-size: 11px; font-weight: 800; letter-spacing: 0.5px;
    border: none; border-radius: 8px; padding: 8px 14px;
}
QPushButton#btnDashboard:hover, QPushButton#btnEmployes:hover,
QPushButton#btnSalles:hover, QPushButton#btnEvaluateurs:hover,
QPushButton#btnParticipants:hover, QPushButton#btnSponsors:hover,
QPushButton#btnProjets:hover, QPushButton#btnNotifications:hover,
QPushButton#btnProfil:hover {
    background-color: #1F2937; color: #FFFFFF;
}

QPushButton#btnEmployes {
    background-color: #312E81; color: #FFFFFF;
    border-bottom: 2px solid #6366F1;
}

QPushButton#btnDeconnexion {
    color: #F87171; border: 1px solid #7F1D1D;
    background-color: transparent;
    font-size: 11px; font-weight: 800;
    border-radius: 8px; padding: 8px 14px;
}
QPushButton#btnDeconnexion:hover {
    background-color: #B91C1C; color: #FFFFFF; border: 1px solid #B91C1C;
}

QLabel#labelTitleEmployes {
    color: #FFFFFF; font-size: 22px; font-weight: 900;
}
QLabel#labelSubEmployes { color: #94A3B8; font-size: 12px; }

QWidget#pageLogin {
    background: qradialgradient(cx:0.3, cy:0.3, radius:1.4,
        stop:0 #1E1B4B, stop:0.5 #0B1020, stop:1 #05070F);
}
QFrame#frameLoginCard {
    background-color: #111827; border: 1px solid #1F2937;
    border-radius: 20px;
}
QFrame#frameLoginBrand {
    background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
        stop:0 #1E1B4B, stop:0.6 #312E81, stop:1 #4338CA);
    border-top-left-radius: 20px; border-bottom-left-radius: 20px;
}
QLabel#labelLoginLogo {
    background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
        stop:0 #6366F1, stop:1 #8B5CF6);
    color: #FFFFFF; border-radius: 20px;
    qproperty-alignment: AlignCenter;
}
QLabel#labelLoginBrandTitle  { color: #FFFFFF; letter-spacing: 5px; }
QLabel#labelLoginBrandSub    { color: #A5B4FC; letter-spacing: 3px; }
QLabel#labelLoginBrandSlogan { color: #E0E7FF; }
QLabel#labelLoginBrandVersion{ color: #94A3B8; }
QLabel#labelLoginBrandTeam   { color: #818CF8; }
QLabel#labelLoginTitle       { color: #FFFFFF; }
QLabel#labelLoginSub         { color: #94A3B8; }
QLabel#labelLoginField,
QLabel#labelLoginField2      { color: #94A3B8; letter-spacing: 1px; }
QLabel#labelLoginFooter      { color: #475569; }

QLineEdit#lineEditIdentifiant,
QLineEdit#lineEditMotDePasse {
    background-color: #0B1020; border: 1.5px solid #374151;
    border-radius: 8px; padding: 8px 12px;
    color: #E5E7EB; selection-background-color: #6366F1;
}
QLineEdit#lineEditIdentifiant:focus,
QLineEdit#lineEditMotDePasse:focus {
    border: 1.5px solid #6366F1; background-color: #111827;
}

QCheckBox#checkBoxSouvenir { color: #94A3B8; spacing: 8px; }
QCheckBox#checkBoxSouvenir::indicator {
    width: 16px; height: 16px;
    border: 1.5px solid #374151;
    border-radius: 4px; background-color: #0B1020;
}
QCheckBox#checkBoxSouvenir::indicator:checked {
    background-color: #6366F1; border: 1.5px solid #6366F1;
}

QPushButton#pushButtonConnexion {
    background-color: #6366F1; color: #FFFFFF;
    font-size: 13px; font-weight: 900; letter-spacing: 2px;
    border: none; border-radius: 10px; padding: 16px;
}
QPushButton#pushButtonConnexion:hover   { background-color: #818CF8; }
QPushButton#pushButtonConnexion:pressed { background-color: #4F46E5; }

QPushButton#pushButtonCGU,
QPushButton#pushButtonPolitique,
QPushButton#pushButtonAPropos {
    color: #94A3B8; background: transparent;
    border: none; font-size: 11px;
    text-align: left; padding: 2px 0;
}
QPushButton#pushButtonCGU:hover,
QPushButton#pushButtonPolitique:hover,
QPushButton#pushButtonAPropos:hover { color: #818CF8; }

QFrame#cardFormEmployes, QFrame#cardTableEmployes {
    background-color: #111827; border: 1px solid #1F2937;
    border-radius: 14px;
}

QLabel#labelFormTitleEmployes {
    color: #FFFFFF; font-size: 14px; font-weight: 800;
}

QLabel#lblEmpID, QLabel#lblEmpNom, QLabel#lblEmpPrenom, QLabel#lblEmpMdp,
QLabel#lblEmpRole, QLabel#lblEmpStatut {
    color: #94A3B8; font-size: 10px; font-weight: 800; letter-spacing: 1px;
}

QLineEdit, QComboBox, QSpinBox {
    background-color: #0B1020; border: 1.5px solid #374151;
    border-radius: 8px; padding: 8px 12px;
    color: #E5E7EB; font-size: 13px; min-height: 20px;
    selection-background-color: #6366F1; selection-color: #FFFFFF;
}
QLineEdit:focus, QComboBox:focus, QSpinBox:focus {
    border: 1.5px solid #6366F1; background-color: #111827;
}
QLineEdit:read-only { background-color: #1F2937; color: #94A3B8; }
QComboBox::drop-down { border: none; width: 24px; }
QComboBox QAbstractItemView {
    background-color: #111827; border: 1px solid #374151;
    selection-background-color: #6366F1; selection-color: #FFFFFF;
    color: #E5E7EB; outline: none;
}

QPushButton#btnAjouterEmploye, QPushButton#btnExporterEmploye {
    background-color: #047857; color: #FFFFFF;
    font-size: 11px; font-weight: 800; letter-spacing: 1px;
    border: none; border-radius: 8px; padding: 10px 18px;
}
QPushButton#btnAjouterEmploye:hover, QPushButton#btnExporterEmploye:hover {
    background-color: #34D399;
}

QPushButton#btnModifierEmploye, QPushButton#btnRechercherEmploye {
    background-color: #4338CA; color: #FFFFFF;
    font-size: 11px; font-weight: 800; letter-spacing: 1px;
    border: none; border-radius: 8px; padding: 10px 18px;
}
QPushButton#btnModifierEmploye:hover, QPushButton#btnRechercherEmploye:hover {
    background-color: #6366F1;
}

QPushButton#btnSupprimerEmploye {
    background-color: #B91C1C; color: #FFFFFF;
    font-size: 11px; font-weight: 800; letter-spacing: 1px;
    border: none; border-radius: 8px; padding: 10px 18px;
}
QPushButton#btnSupprimerEmploye:hover {
    background-color: #F87171;
}

QPushButton#btnTrierEmploye, QPushButton#btn_Annuler {
    background-color: #475569; color: #E5E7EB;
    font-size: 11px; font-weight: 700;
    border: none; border-radius: 8px; padding: 10px 18px;
}
QPushButton#btnTrierEmploye:hover, QPushButton#btn_Annuler:hover {
    background-color: #64748B; color: #FFFFFF;
}

QTableWidget {
    background-color: #0B1020; border: 1px solid #1F2937;
    border-radius: 10px; gridline-color: #1F2937;
    font-size: 12px; color: #E5E7EB;
    selection-background-color: #312E81; selection-color: #FFFFFF;
    alternate-background-color: #111827; outline: none;
}
QTableWidget::item { padding: 8px; border-bottom: 1px solid #1F2937; }
QTableWidget::item:selected { background-color: #312E81; color: #FFFFFF; }
QHeaderView::section {
    background-color: #111827; color: #818CF8;
    font-size: 10px; font-weight: 800; letter-spacing: 1px;
    padding: 12px 10px; border: none;
    border-right: 1px solid #1F2937;
    border-bottom: 2px solid #6366F1;
}
QTableCornerButton::section { background-color: #111827; border: none; }

QLabel#labelTableTitleEmployes {
    color: #FFFFFF; font-size: 14px; font-weight: 800;
}

QLabel#labelFooterEmployes {
    color: #475569; font-size: 10px;
}

/* ---------- CHATBOT ---------- */
QFrame#frameChatbot {
    background-color: #111827; border: 1px solid #1F2937;
    border-radius: 14px;
}
QTextBrowser#textChatbot {
    background-color: #0B1020; border: 1px solid #1F2937;
    border-radius: 8px; color: #E5E7EB;
    font-size: 12px; padding: 10px;
}
QPushButton#btnEnvoyer {
    background-color: #6366F1; color: #FFFFFF;
    font-size: 11px; font-weight: 800; letter-spacing: 1px;
    border: none; border-radius: 8px; padding: 10px 18px;
}
QPushButton#btnEnvoyer:hover { background-color: #818CF8; }

/* ---------- STATISTIQUES ---------- */
QFrame#cardStats {
    background-color: #111827; border: 1px solid #1F2937;
    border-radius: 14px;
}
QLabel#labelStatsTitle {
    color: #FFFFFF; font-size: 14px; font-weight: 800;
}
QLabel#labelStatutPie, QLabel#labelRoleBar {
    background-color: transparent;
}
)");
}