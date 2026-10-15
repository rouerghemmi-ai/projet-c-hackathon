/********************************************************************************
** Form generated from reading UI file 'GestionEvaluateurs.ui'
**
** Created by: Qt User Interface Compiler version 6.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GESTIONEVALUATEURS_H
#define UI_GESTIONEVALUATEURS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GestionEvaluateurs
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayoutRoot;
    QWidget *sidebar;
    QVBoxLayout *verticalLayoutSidebar;
    QLabel *brandTitle;
    QLabel *brandSubtitle;
    QSpacerItem *spacerTop;
    QLabel *menuLabel;
    QPushButton *menuButton;
    QPushButton *menuButton2;
    QPushButton *menuButton3;
    QPushButton *menuButton4;
    QPushButton *menuButton5;
    QPushButton *menuButton6;
    QSpacerItem *spacerBottom;
    QLabel *lblConnecte;
    QPushButton *btnDeconnexion;
    QWidget *mainContent;
    QVBoxLayout *verticalLayoutMain;
    QLabel *pageTitle;
    QLabel *subTitle;
    QHBoxLayout *statsLayout;
    QFrame *statCardBlue;
    QVBoxLayout *statLayout1;
    QLabel *labelTotal;
    QLabel *lblTotal;
    QFrame *statCard;
    QVBoxLayout *statLayout2;
    QLabel *labelActifs;
    QLabel *lblActifs;
    QFrame *statCardRed;
    QVBoxLayout *statLayout3;
    QLabel *labelInactifs;
    QLabel *lblInactifs;
    QFrame *statCardDistribution;
    QVBoxLayout *statLayout4;
    QLabel *labelRepartition;
    QLabel *lblRepartition;
    QSpacerItem *spacerMid;
    QLabel *formTitle;
    QHBoxLayout *formLayout;
    QVBoxLayout *idLayout;
    QLabel *idLabel;
    QLineEdit *inputId;
    QVBoxLayout *nomLayout;
    QLabel *nomLabel;
    QLineEdit *inputNom;
    QVBoxLayout *prenomLayout;
    QLabel *prenomLabel;
    QLineEdit *inputPrenom;
    QVBoxLayout *statutLayout;
    QLabel *statutLabel;
    QComboBox *comboStatut;
    QPushButton *btnAjouter;
    QPushButton *btnAnnuler;
    QSpacerItem *spacerList;
    QHBoxLayout *listHeaderLayout;
    QLabel *listTitle;
    QSpacerItem *listHeaderSpacer;
    QLineEdit *inputRecherche;
    QPushButton *btnRechercher;
    QPushButton *btnTrier;
    QPushButton *btnExporter;
    QTableView *tableEvaluateurs;
    QSpacerItem *bottomSpacer;
    QHBoxLayout *metiersLayout;
    QPushButton *btnMetier1;
    QPushButton *btnMetier2;
    QSpacerItem *metierSpacer;

    void setupUi(QMainWindow *GestionEvaluateurs)
    {
        if (GestionEvaluateurs->objectName().isEmpty())
            GestionEvaluateurs->setObjectName("GestionEvaluateurs");
        GestionEvaluateurs->resize(1450, 850);
        GestionEvaluateurs->setMinimumSize(QSize(1100, 700));
        GestionEvaluateurs->setStyleSheet(QString::fromUtf8("\n"
"QMainWindow, QWidget#centralwidget { background-color: #f4f7fc; color: #0b1b42; font-family: \"Segoe UI\"; font-size: 13px; }\n"
"QWidget#sidebar { background-color: #07143b; color: white; }\n"
"QLabel#brandTitle { color: white; font-size: 25px; font-weight: 700; }\n"
"QLabel#brandSubtitle { color: #e0e8fb; font-size: 20px; }\n"
"QLabel#pageTitle { color: #0a1c42; font-size: 30px; font-weight: 700; }\n"
"QLabel#subTitle { color: #7184a5; font-size: 13px; }\n"
"QFrame#statCard { background: white; border: 1px solid #dfe7f2; border-radius: 13px; }\n"
"QFrame#statCardBlue { background: #eef5ff; border: 1px solid #d5e5ff; border-radius: 13px; }\n"
"QFrame#statCardRed { background: #fff1f1; border: 1px solid #ffd5d5; border-radius: 13px; }\n"
"QFrame#statCardDistribution { background: white; border: 1px solid #dfe7f2; border-radius: 13px; }\n"
"QLabel.statLabel { color: #5b7195; font-size: 13px; }\n"
"QLabel.statValue { color: #173e78; font-size: 31px; font-weight: 700; }\n"
"QLabel#lblInactifs { color: #d83f3"
                        "f; font-size: 31px; font-weight: 700; }\n"
"QLabel.sectionTitle { color: #0b1b42; font-size: 17px; font-weight: 700; }\n"
"QLineEdit, QComboBox { background: white; border: 1px solid #d4deec; border-radius: 7px; padding: 9px 12px; min-height: 23px; color: #14294d; }\n"
"QLineEdit:focus, QComboBox:focus { border: 1px solid #2870e3; }\n"
"QPushButton { border-radius: 7px; padding: 10px 17px; font-weight: 600; }\n"
"QPushButton#btnAjouter, QPushButton#btnRechercher, QPushButton#btnMetier1 { background: #2468df; color: white; border: none; }\n"
"QPushButton#btnAjouter:hover, QPushButton#btnRechercher:hover, QPushButton#btnMetier1:hover { background: #1758c5; }\n"
"QPushButton#btnAnnuler, QPushButton#btnTrier { background: #eef2f8; color: #18345d; border: 1px solid #d5deeb; }\n"
"QPushButton#btnExporter { background: #168957; color: white; border: none; }\n"
"QPushButton#btnModifier { background: #138b57; color: white; border: none; }\n"
"QPushButton#btnSupprimer, QPushButton#btnDeconnexion { background: #dc4545; c"
                        "olor: white; border: none; }\n"
"QPushButton#btnMetier2 { background: transparent; color: #12845a; border: none; text-align: left; }\n"
"QPushButton#menuButton { background: transparent; color: #e6edff; text-align: left; border: none; padding: 13px 15px; font-size: 14px; }\n"
"QPushButton#menuButton:checked { background: #2468df; color: white; }\n"
"QTableView { background: white; alternate-background-color: #f4f6f9; border: 1px solid #e0e7f1; gridline-color: #e6edf5; selection-background-color: #dce9ff; selection-color: #0b1b42; }\n"
"QHeaderView::section { background: #234779; color: white; padding: 13px 8px; border: none; font-weight: 700; }\n"
"QTableView QTableCornerButton::section { background: #234779; }\n"
"  "));
        centralwidget = new QWidget(GestionEvaluateurs);
        centralwidget->setObjectName("centralwidget");
        horizontalLayoutRoot = new QHBoxLayout(centralwidget);
        horizontalLayoutRoot->setSpacing(0);
        horizontalLayoutRoot->setObjectName("horizontalLayoutRoot");
        horizontalLayoutRoot->setContentsMargins(0, 0, 0, 0);
        sidebar = new QWidget(centralwidget);
        sidebar->setObjectName("sidebar");
        sidebar->setMinimumSize(QSize(260, 0));
        sidebar->setMaximumSize(QSize(260, 16777215));
        verticalLayoutSidebar = new QVBoxLayout(sidebar);
        verticalLayoutSidebar->setObjectName("verticalLayoutSidebar");
        verticalLayoutSidebar->setContentsMargins(20, 28, 20, 18);
        brandTitle = new QLabel(sidebar);
        brandTitle->setObjectName("brandTitle");

        verticalLayoutSidebar->addWidget(brandTitle);

        brandSubtitle = new QLabel(sidebar);
        brandSubtitle->setObjectName("brandSubtitle");

        verticalLayoutSidebar->addWidget(brandSubtitle);

        spacerTop = new QSpacerItem(20, 85, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayoutSidebar->addItem(spacerTop);

        menuLabel = new QLabel(sidebar);
        menuLabel->setObjectName("menuLabel");

        verticalLayoutSidebar->addWidget(menuLabel);

        menuButton = new QPushButton(sidebar);
        menuButton->setObjectName("menuButton");
        menuButton->setCheckable(true);

        verticalLayoutSidebar->addWidget(menuButton);

        menuButton2 = new QPushButton(sidebar);
        menuButton2->setObjectName("menuButton2");

        verticalLayoutSidebar->addWidget(menuButton2);

        menuButton3 = new QPushButton(sidebar);
        menuButton3->setObjectName("menuButton3");

        verticalLayoutSidebar->addWidget(menuButton3);

        menuButton4 = new QPushButton(sidebar);
        menuButton4->setObjectName("menuButton4");

        verticalLayoutSidebar->addWidget(menuButton4);

        menuButton5 = new QPushButton(sidebar);
        menuButton5->setObjectName("menuButton5");

        verticalLayoutSidebar->addWidget(menuButton5);

        menuButton6 = new QPushButton(sidebar);
        menuButton6->setObjectName("menuButton6");

        verticalLayoutSidebar->addWidget(menuButton6);

        spacerBottom = new QSpacerItem(20, 150, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayoutSidebar->addItem(spacerBottom);

        lblConnecte = new QLabel(sidebar);
        lblConnecte->setObjectName("lblConnecte");

        verticalLayoutSidebar->addWidget(lblConnecte);

        btnDeconnexion = new QPushButton(sidebar);
        btnDeconnexion->setObjectName("btnDeconnexion");

        verticalLayoutSidebar->addWidget(btnDeconnexion);


        horizontalLayoutRoot->addWidget(sidebar);

        mainContent = new QWidget(centralwidget);
        mainContent->setObjectName("mainContent");
        verticalLayoutMain = new QVBoxLayout(mainContent);
        verticalLayoutMain->setSpacing(15);
        verticalLayoutMain->setObjectName("verticalLayoutMain");
        verticalLayoutMain->setContentsMargins(32, 28, 28, 18);
        pageTitle = new QLabel(mainContent);
        pageTitle->setObjectName("pageTitle");

        verticalLayoutMain->addWidget(pageTitle);

        subTitle = new QLabel(mainContent);
        subTitle->setObjectName("subTitle");

        verticalLayoutMain->addWidget(subTitle);

        statsLayout = new QHBoxLayout();
        statsLayout->setSpacing(16);
        statsLayout->setObjectName("statsLayout");
        statCardBlue = new QFrame(mainContent);
        statCardBlue->setObjectName("statCardBlue");
        statCardBlue->setMinimumSize(QSize(0, 105));
        statLayout1 = new QVBoxLayout(statCardBlue);
        statLayout1->setObjectName("statLayout1");
        statLayout1->setContentsMargins(20, -1, -1, -1);
        labelTotal = new QLabel(statCardBlue);
        labelTotal->setObjectName("labelTotal");

        statLayout1->addWidget(labelTotal);

        lblTotal = new QLabel(statCardBlue);
        lblTotal->setObjectName("lblTotal");

        statLayout1->addWidget(lblTotal);


        statsLayout->addWidget(statCardBlue);

        statCard = new QFrame(mainContent);
        statCard->setObjectName("statCard");
        statCard->setMinimumSize(QSize(0, 105));
        statLayout2 = new QVBoxLayout(statCard);
        statLayout2->setObjectName("statLayout2");
        statLayout2->setContentsMargins(20, -1, -1, -1);
        labelActifs = new QLabel(statCard);
        labelActifs->setObjectName("labelActifs");

        statLayout2->addWidget(labelActifs);

        lblActifs = new QLabel(statCard);
        lblActifs->setObjectName("lblActifs");

        statLayout2->addWidget(lblActifs);


        statsLayout->addWidget(statCard);

        statCardRed = new QFrame(mainContent);
        statCardRed->setObjectName("statCardRed");
        statCardRed->setMinimumSize(QSize(0, 105));
        statLayout3 = new QVBoxLayout(statCardRed);
        statLayout3->setObjectName("statLayout3");
        statLayout3->setContentsMargins(20, -1, -1, -1);
        labelInactifs = new QLabel(statCardRed);
        labelInactifs->setObjectName("labelInactifs");

        statLayout3->addWidget(labelInactifs);

        lblInactifs = new QLabel(statCardRed);
        lblInactifs->setObjectName("lblInactifs");

        statLayout3->addWidget(lblInactifs);


        statsLayout->addWidget(statCardRed);

        statCardDistribution = new QFrame(mainContent);
        statCardDistribution->setObjectName("statCardDistribution");
        statCardDistribution->setMinimumSize(QSize(0, 105));
        statLayout4 = new QVBoxLayout(statCardDistribution);
        statLayout4->setObjectName("statLayout4");
        statLayout4->setContentsMargins(20, -1, -1, -1);
        labelRepartition = new QLabel(statCardDistribution);
        labelRepartition->setObjectName("labelRepartition");

        statLayout4->addWidget(labelRepartition);

        lblRepartition = new QLabel(statCardDistribution);
        lblRepartition->setObjectName("lblRepartition");

        statLayout4->addWidget(lblRepartition);


        statsLayout->addWidget(statCardDistribution);


        verticalLayoutMain->addLayout(statsLayout);

        spacerMid = new QSpacerItem(20, 8, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayoutMain->addItem(spacerMid);

        formTitle = new QLabel(mainContent);
        formTitle->setObjectName("formTitle");

        verticalLayoutMain->addWidget(formTitle);

        formLayout = new QHBoxLayout();
        formLayout->setSpacing(14);
        formLayout->setObjectName("formLayout");
        idLayout = new QVBoxLayout();
        idLayout->setObjectName("idLayout");
        idLabel = new QLabel(mainContent);
        idLabel->setObjectName("idLabel");

        idLayout->addWidget(idLabel);

        inputId = new QLineEdit(mainContent);
        inputId->setObjectName("inputId");

        idLayout->addWidget(inputId);


        formLayout->addLayout(idLayout);

        nomLayout = new QVBoxLayout();
        nomLayout->setObjectName("nomLayout");
        nomLabel = new QLabel(mainContent);
        nomLabel->setObjectName("nomLabel");

        nomLayout->addWidget(nomLabel);

        inputNom = new QLineEdit(mainContent);
        inputNom->setObjectName("inputNom");

        nomLayout->addWidget(inputNom);


        formLayout->addLayout(nomLayout);

        prenomLayout = new QVBoxLayout();
        prenomLayout->setObjectName("prenomLayout");
        prenomLabel = new QLabel(mainContent);
        prenomLabel->setObjectName("prenomLabel");

        prenomLayout->addWidget(prenomLabel);

        inputPrenom = new QLineEdit(mainContent);
        inputPrenom->setObjectName("inputPrenom");

        prenomLayout->addWidget(inputPrenom);


        formLayout->addLayout(prenomLayout);

        statutLayout = new QVBoxLayout();
        statutLayout->setObjectName("statutLayout");
        statutLabel = new QLabel(mainContent);
        statutLabel->setObjectName("statutLabel");

        statutLayout->addWidget(statutLabel);

        comboStatut = new QComboBox(mainContent);
        comboStatut->setObjectName("comboStatut");

        statutLayout->addWidget(comboStatut);


        formLayout->addLayout(statutLayout);

        btnAjouter = new QPushButton(mainContent);
        btnAjouter->setObjectName("btnAjouter");
        btnAjouter->setMinimumSize(QSize(88, 42));

        formLayout->addWidget(btnAjouter);

        btnAnnuler = new QPushButton(mainContent);
        btnAnnuler->setObjectName("btnAnnuler");
        btnAnnuler->setMinimumSize(QSize(88, 42));

        formLayout->addWidget(btnAnnuler);


        verticalLayoutMain->addLayout(formLayout);

        spacerList = new QSpacerItem(20, 12, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayoutMain->addItem(spacerList);

        listHeaderLayout = new QHBoxLayout();
        listHeaderLayout->setObjectName("listHeaderLayout");
        listTitle = new QLabel(mainContent);
        listTitle->setObjectName("listTitle");

        listHeaderLayout->addWidget(listTitle);

        listHeaderSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        listHeaderLayout->addItem(listHeaderSpacer);

        inputRecherche = new QLineEdit(mainContent);
        inputRecherche->setObjectName("inputRecherche");
        inputRecherche->setMinimumSize(QSize(300, 0));

        listHeaderLayout->addWidget(inputRecherche);

        btnRechercher = new QPushButton(mainContent);
        btnRechercher->setObjectName("btnRechercher");

        listHeaderLayout->addWidget(btnRechercher);

        btnTrier = new QPushButton(mainContent);
        btnTrier->setObjectName("btnTrier");

        listHeaderLayout->addWidget(btnTrier);

        btnExporter = new QPushButton(mainContent);
        btnExporter->setObjectName("btnExporter");

        listHeaderLayout->addWidget(btnExporter);


        verticalLayoutMain->addLayout(listHeaderLayout);

        tableEvaluateurs = new QTableView(mainContent);
        tableEvaluateurs->setObjectName("tableEvaluateurs");
        tableEvaluateurs->setMinimumSize(QSize(0, 280));
        tableEvaluateurs->setAlternatingRowColors(true);
        tableEvaluateurs->setShowGrid(true);
        tableEvaluateurs->setSortingEnabled(false);
        tableEvaluateurs->setSelectionMode(QAbstractItemView::SingleSelection);

        verticalLayoutMain->addWidget(tableEvaluateurs);

        bottomSpacer = new QSpacerItem(20, 5, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayoutMain->addItem(bottomSpacer);

        metiersLayout = new QHBoxLayout();
        metiersLayout->setObjectName("metiersLayout");
        btnMetier1 = new QPushButton(mainContent);
        btnMetier1->setObjectName("btnMetier1");

        metiersLayout->addWidget(btnMetier1);

        btnMetier2 = new QPushButton(mainContent);
        btnMetier2->setObjectName("btnMetier2");

        metiersLayout->addWidget(btnMetier2);

        metierSpacer = new QSpacerItem(20, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        metiersLayout->addItem(metierSpacer);


        verticalLayoutMain->addLayout(metiersLayout);


        horizontalLayoutRoot->addWidget(mainContent);

        GestionEvaluateurs->setCentralWidget(centralwidget);

        retranslateUi(GestionEvaluateurs);

        QMetaObject::connectSlotsByName(GestionEvaluateurs);
    } // setupUi

    void retranslateUi(QMainWindow *GestionEvaluateurs)
    {
        GestionEvaluateurs->setWindowTitle(QCoreApplication::translate("GestionEvaluateurs", "Smart Hackathon Management - Gestion des \303\251valuateurs", nullptr));
        brandTitle->setText(QCoreApplication::translate("GestionEvaluateurs", "Smart Hackathon", nullptr));
        brandSubtitle->setText(QCoreApplication::translate("GestionEvaluateurs", "Management", nullptr));
        menuLabel->setText(QCoreApplication::translate("GestionEvaluateurs", "MENU PRINCIPAL", nullptr));
        menuLabel->setStyleSheet(QCoreApplication::translate("GestionEvaluateurs", "color:#b9c9e9; padding-top:10px;", nullptr));
        menuButton->setText(QCoreApplication::translate("GestionEvaluateurs", "\342\226\246   Tableau de Bord", nullptr));
        menuButton2->setText(QCoreApplication::translate("GestionEvaluateurs", "\342\227\217   Gestion des \303\251valuateurs", nullptr));
        menuButton2->setStyleSheet(QCoreApplication::translate("GestionEvaluateurs", "background:#2468df;color:white;border-radius:8px;text-align:left;padding:13px;font-weight:700;", nullptr));
        menuButton3->setText(QCoreApplication::translate("GestionEvaluateurs", "\342\227\217   Gestion des participants", nullptr));
        menuButton3->setStyleSheet(QCoreApplication::translate("GestionEvaluateurs", "background:transparent;color:#e6edff;text-align:left;padding:13px;", nullptr));
        menuButton4->setText(QCoreApplication::translate("GestionEvaluateurs", "\342\227\206   Gestion des sponsors", nullptr));
        menuButton4->setStyleSheet(QCoreApplication::translate("GestionEvaluateurs", "background:transparent;color:#e6edff;text-align:left;padding:13px;", nullptr));
        menuButton5->setText(QCoreApplication::translate("GestionEvaluateurs", "\342\226\243   Gestion des projets", nullptr));
        menuButton5->setStyleSheet(QCoreApplication::translate("GestionEvaluateurs", "background:transparent;color:#e6edff;text-align:left;padding:13px;", nullptr));
        menuButton6->setText(QCoreApplication::translate("GestionEvaluateurs", "\342\226\244   Gestion des salles", nullptr));
        menuButton6->setStyleSheet(QCoreApplication::translate("GestionEvaluateurs", "background:transparent;color:#e6edff;text-align:left;padding:13px;", nullptr));
        lblConnecte->setText(QCoreApplication::translate("GestionEvaluateurs", "Connect\303\251 : Responsable", nullptr));
        lblConnecte->setStyleSheet(QCoreApplication::translate("GestionEvaluateurs", "color:#b9c9e9;border-top:1px solid #26365d;padding-top:12px;", nullptr));
        btnDeconnexion->setText(QCoreApplication::translate("GestionEvaluateurs", "D\303\251connexion s\303\251curis\303\251e", nullptr));
        pageTitle->setText(QCoreApplication::translate("GestionEvaluateurs", "Gestion des \303\251valuateurs", nullptr));
        subTitle->setText(QCoreApplication::translate("GestionEvaluateurs", "Entit\303\251 : \303\251valuateur  \342\200\242  Utilisateurs : Responsables", nullptr));
        labelTotal->setText(QCoreApplication::translate("GestionEvaluateurs", "Nombre total d'\303\251valuateurs", nullptr));
        labelTotal->setStyleSheet(QCoreApplication::translate("GestionEvaluateurs", "color:#5b7195;", nullptr));
        lblTotal->setText(QCoreApplication::translate("GestionEvaluateurs", "6", nullptr));
        lblTotal->setStyleSheet(QCoreApplication::translate("GestionEvaluateurs", "color:#173e78;font-size:31px;font-weight:700;", nullptr));
        labelActifs->setText(QCoreApplication::translate("GestionEvaluateurs", "\303\211valuateurs actifs", nullptr));
        labelActifs->setStyleSheet(QCoreApplication::translate("GestionEvaluateurs", "color:#5b7195;", nullptr));
        lblActifs->setText(QCoreApplication::translate("GestionEvaluateurs", "4", nullptr));
        lblActifs->setStyleSheet(QCoreApplication::translate("GestionEvaluateurs", "color:#173e78;font-size:31px;font-weight:700;", nullptr));
        labelInactifs->setText(QCoreApplication::translate("GestionEvaluateurs", "\303\211valuateurs inactifs", nullptr));
        labelInactifs->setStyleSheet(QCoreApplication::translate("GestionEvaluateurs", "color:#a35a5a;", nullptr));
        lblInactifs->setText(QCoreApplication::translate("GestionEvaluateurs", "2", nullptr));
        labelRepartition->setText(QCoreApplication::translate("GestionEvaluateurs", "R\303\251partition", nullptr));
        labelRepartition->setStyleSheet(QCoreApplication::translate("GestionEvaluateurs", "color:#5b7195;", nullptr));
        lblRepartition->setText(QCoreApplication::translate("GestionEvaluateurs", "\342\227\217 Actifs : 66 %\\n\342\227\217 Inactifs : 34 %", nullptr));
        lblRepartition->setStyleSheet(QCoreApplication::translate("GestionEvaluateurs", "color:#168957;font-weight:700;", nullptr));
        formTitle->setText(QCoreApplication::translate("GestionEvaluateurs", "Informations de l'\303\251valuateur", nullptr));
        formTitle->setStyleSheet(QCoreApplication::translate("GestionEvaluateurs", "font-size:17px;font-weight:700;color:#0b1b42;", nullptr));
        idLabel->setText(QCoreApplication::translate("GestionEvaluateurs", "ID \303\251valuateur", nullptr));
        idLabel->setStyleSheet(QCoreApplication::translate("GestionEvaluateurs", "font-weight:600;color:#506587;", nullptr));
        inputId->setPlaceholderText(QCoreApplication::translate("GestionEvaluateurs", "E007", nullptr));
        nomLabel->setText(QCoreApplication::translate("GestionEvaluateurs", "Nom", nullptr));
        nomLabel->setStyleSheet(QCoreApplication::translate("GestionEvaluateurs", "font-weight:600;color:#506587;", nullptr));
        inputNom->setPlaceholderText(QCoreApplication::translate("GestionEvaluateurs", "Nom", nullptr));
        prenomLabel->setText(QCoreApplication::translate("GestionEvaluateurs", "Pr\303\251nom", nullptr));
        prenomLabel->setStyleSheet(QCoreApplication::translate("GestionEvaluateurs", "font-weight:600;color:#506587;", nullptr));
        inputPrenom->setPlaceholderText(QCoreApplication::translate("GestionEvaluateurs", "Pr\303\251nom", nullptr));
        statutLabel->setText(QCoreApplication::translate("GestionEvaluateurs", "Statut", nullptr));
        statutLabel->setStyleSheet(QCoreApplication::translate("GestionEvaluateurs", "font-weight:600;color:#506587;", nullptr));
        btnAjouter->setText(QCoreApplication::translate("GestionEvaluateurs", "Ajouter", nullptr));
        btnAnnuler->setText(QCoreApplication::translate("GestionEvaluateurs", "Annuler", nullptr));
        listTitle->setText(QCoreApplication::translate("GestionEvaluateurs", "Liste des \303\251valuateurs", nullptr));
        listTitle->setStyleSheet(QCoreApplication::translate("GestionEvaluateurs", "font-size:17px;font-weight:700;color:#0b1b42;", nullptr));
        inputRecherche->setPlaceholderText(QCoreApplication::translate("GestionEvaluateurs", "Rechercher un \303\251valuateur...", nullptr));
        btnRechercher->setText(QCoreApplication::translate("GestionEvaluateurs", "Rechercher", nullptr));
        btnTrier->setText(QCoreApplication::translate("GestionEvaluateurs", "Trier", nullptr));
        btnExporter->setText(QCoreApplication::translate("GestionEvaluateurs", "Exporter", nullptr));
        btnMetier1->setText(QCoreApplication::translate("GestionEvaluateurs", "\342\227\206  M\303\251tier 1 : Tech Challenge Analyst", nullptr));
        btnMetier2->setText(QCoreApplication::translate("GestionEvaluateurs", "\342\227\206  M\303\251tier 2 : Project Feasibility Analyst", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GestionEvaluateurs: public Ui_GestionEvaluateurs {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GESTIONEVALUATEURS_H
