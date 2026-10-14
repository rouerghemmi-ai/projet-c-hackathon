/********************************************************************************
** Form generated from reading UI file 'integration.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INTEGRATION_H
#define UI_INTEGRATION_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_integration
{
public:
    QWidget *centralWidget;
    QStackedWidget *stackedWidget;
    QWidget *pageLogin;
    QFrame *frameLoginCard;
    QFrame *frameLoginBrand;
    QLabel *labelLoginLogo;
    QLabel *labelLoginBrandTitle;
    QLabel *labelLoginBrandSub;
    QLabel *labelLoginBrandSlogan;
    QLabel *labelLoginBrandVersion;
    QLabel *labelLoginBrandTeam;
    QLabel *labelLoginTitle;
    QLabel *labelLoginSub;
    QLabel *labelLoginField;
    QLineEdit *lineEditIdentifiant;
    QLabel *labelLoginField2;
    QLineEdit *lineEditMotDePasse;
    QCheckBox *checkBoxSouvenir;
    QPushButton *pushButtonConnexion;
    QPushButton *pushButtonCGU;
    QPushButton *pushButtonPolitique;
    QPushButton *pushButtonAPropos;
    QLabel *labelLoginFooter;
    QWidget *pageEmployes;
    QScrollArea *scrollEmployes;
    QWidget *scrollEmployesContent;
    QFrame *frameNavbar;
    QLabel *labelNavLogo;
    QLabel *labelNavBrand;
    QLabel *labelNavSub;
    QPushButton *btnDashboard;
    QPushButton *btnEmployes;
    QPushButton *btnSalles;
    QPushButton *btnEvaluateurs;
    QPushButton *btnParticipants;
    QPushButton *btnSponsors;
    QPushButton *btnProjets;
    QPushButton *btnNotifications;
    QPushButton *btnProfil;
    QPushButton *btnDeconnexion;
    QLabel *labelTitleEmployes;
    QLabel *labelSubEmployes;
    QFrame *cardFormEmployes;
    QLabel *labelFormTitleEmployes;
    QLabel *lblEmpID;
    QLineEdit *lineEditEmpID;
    QLabel *lblEmpNom;
    QLineEdit *lineEditEmpNom;
    QLabel *lblEmpPrenom;
    QLineEdit *lineEditEmpPrenom;
    QLabel *lblEmpMdp;
    QLineEdit *lineEditEmpMdp;
    QLabel *lblEmpRole;
    QComboBox *comboBoxEmpRole;
    QLabel *lblEmpStatut;
    QComboBox *comboBoxEmpStatut;
    QPushButton *btnAjouterEmploye;
    QPushButton *btn_Annuler;
    QLineEdit *lineEdit;
    QPushButton *btnRechercherEmploye;
    QPushButton *btnTrierEmploye;
    QPushButton *btnExporterEmploye;
    QFrame *cardTableEmployes;
    QTableWidget *tableEmployes;
    QLabel *labelTableTitleEmployes;
    QFrame *frameChatbot;
    QVBoxLayout *verticalLayout_6;
    QLabel *lblTitreChatbot;
    QTextBrowser *textChatbot;
    QFrame *frameSaisieChatbot;
    QHBoxLayout *horizontalLayout_6;
    QLineEdit *leQuestion;
    QPushButton *btnEnvoyer;
    QFrame *cardStats;
    QLabel *labelStatsTitle;
    QLabel *labelStatutPie;
    QLabel *labelRoleBar;
    QLabel *labelFooterEmployes;

    void setupUi(QMainWindow *integration)
    {
        if (integration->objectName().isEmpty())
            integration->setObjectName("integration");
        integration->resize(1440, 900);
        integration->setMinimumSize(QSize(1280, 720));
        centralWidget = new QWidget(integration);
        centralWidget->setObjectName("centralWidget");
        stackedWidget = new QStackedWidget(centralWidget);
        stackedWidget->setObjectName("stackedWidget");
        stackedWidget->setGeometry(QRect(0, 0, 1440, 900));
        pageLogin = new QWidget();
        pageLogin->setObjectName("pageLogin");
        frameLoginCard = new QFrame(pageLogin);
        frameLoginCard->setObjectName("frameLoginCard");
        frameLoginCard->setGeometry(QRect(320, 140, 800, 620));
        frameLoginCard->setFrameShape(QFrame::Shape::StyledPanel);
        frameLoginCard->setFrameShadow(QFrame::Shadow::Raised);
        frameLoginBrand = new QFrame(pageLogin);
        frameLoginBrand->setObjectName("frameLoginBrand");
        frameLoginBrand->setGeometry(QRect(320, 140, 380, 620));
        frameLoginBrand->setFrameShape(QFrame::Shape::StyledPanel);
        frameLoginBrand->setFrameShadow(QFrame::Shadow::Raised);
        labelLoginLogo = new QLabel(pageLogin);
        labelLoginLogo->setObjectName("labelLoginLogo");
        labelLoginLogo->setGeometry(QRect(440, 220, 140, 140));
        QFont font;
        font.setFamilies({QString::fromUtf8("Arial")});
        font.setPointSize(48);
        font.setBold(true);
        labelLoginLogo->setFont(font);
        labelLoginLogo->setAlignment(Qt::AlignmentFlag::AlignCenter);
        labelLoginBrandTitle = new QLabel(pageLogin);
        labelLoginBrandTitle->setObjectName("labelLoginBrandTitle");
        labelLoginBrandTitle->setGeometry(QRect(340, 390, 340, 50));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Arial")});
        font1.setPointSize(28);
        font1.setBold(true);
        labelLoginBrandTitle->setFont(font1);
        labelLoginBrandTitle->setAlignment(Qt::AlignmentFlag::AlignCenter);
        labelLoginBrandSub = new QLabel(pageLogin);
        labelLoginBrandSub->setObjectName("labelLoginBrandSub");
        labelLoginBrandSub->setGeometry(QRect(340, 440, 340, 25));
        QFont font2;
        font2.setFamilies({QString::fromUtf8("Arial")});
        font2.setPointSize(10);
        font2.setBold(true);
        labelLoginBrandSub->setFont(font2);
        labelLoginBrandSub->setAlignment(Qt::AlignmentFlag::AlignCenter);
        labelLoginBrandSlogan = new QLabel(pageLogin);
        labelLoginBrandSlogan->setObjectName("labelLoginBrandSlogan");
        labelLoginBrandSlogan->setGeometry(QRect(360, 500, 300, 80));
        QFont font3;
        font3.setFamilies({QString::fromUtf8("Times New Roman")});
        font3.setPointSize(12);
        labelLoginBrandSlogan->setFont(font3);
        labelLoginBrandSlogan->setAlignment(Qt::AlignmentFlag::AlignCenter);
        labelLoginBrandSlogan->setWordWrap(true);
        labelLoginBrandVersion = new QLabel(pageLogin);
        labelLoginBrandVersion->setObjectName("labelLoginBrandVersion");
        labelLoginBrandVersion->setGeometry(QRect(420, 670, 180, 20));
        QFont font4;
        font4.setFamilies({QString::fromUtf8("Arial")});
        font4.setPointSize(9);
        labelLoginBrandVersion->setFont(font4);
        labelLoginBrandVersion->setAlignment(Qt::AlignmentFlag::AlignCenter);
        labelLoginBrandTeam = new QLabel(pageLogin);
        labelLoginBrandTeam->setObjectName("labelLoginBrandTeam");
        labelLoginBrandTeam->setGeometry(QRect(420, 690, 180, 20));
        QFont font5;
        font5.setFamilies({QString::fromUtf8("Arial")});
        font5.setPointSize(9);
        font5.setBold(true);
        labelLoginBrandTeam->setFont(font5);
        labelLoginBrandTeam->setAlignment(Qt::AlignmentFlag::AlignCenter);
        labelLoginTitle = new QLabel(pageLogin);
        labelLoginTitle->setObjectName("labelLoginTitle");
        labelLoginTitle->setGeometry(QRect(780, 220, 280, 40));
        QFont font6;
        font6.setFamilies({QString::fromUtf8("Arial")});
        font6.setPointSize(24);
        font6.setBold(true);
        labelLoginTitle->setFont(font6);
        labelLoginSub = new QLabel(pageLogin);
        labelLoginSub->setObjectName("labelLoginSub");
        labelLoginSub->setGeometry(QRect(780, 265, 320, 30));
        QFont font7;
        font7.setFamilies({QString::fromUtf8("Times New Roman")});
        font7.setPointSize(11);
        labelLoginSub->setFont(font7);
        labelLoginSub->setWordWrap(true);
        labelLoginField = new QLabel(pageLogin);
        labelLoginField->setObjectName("labelLoginField");
        labelLoginField->setGeometry(QRect(780, 340, 200, 20));
        labelLoginField->setFont(font2);
        lineEditIdentifiant = new QLineEdit(pageLogin);
        lineEditIdentifiant->setObjectName("lineEditIdentifiant");
        lineEditIdentifiant->setGeometry(QRect(780, 365, 320, 45));
        lineEditIdentifiant->setFont(font7);
        labelLoginField2 = new QLabel(pageLogin);
        labelLoginField2->setObjectName("labelLoginField2");
        labelLoginField2->setGeometry(QRect(780, 430, 200, 20));
        labelLoginField2->setFont(font2);
        lineEditMotDePasse = new QLineEdit(pageLogin);
        lineEditMotDePasse->setObjectName("lineEditMotDePasse");
        lineEditMotDePasse->setGeometry(QRect(780, 455, 320, 45));
        lineEditMotDePasse->setFont(font7);
        lineEditMotDePasse->setEchoMode(QLineEdit::EchoMode::Password);
        checkBoxSouvenir = new QCheckBox(pageLogin);
        checkBoxSouvenir->setObjectName("checkBoxSouvenir");
        checkBoxSouvenir->setGeometry(QRect(780, 515, 200, 25));
        QFont font8;
        font8.setFamilies({QString::fromUtf8("Times New Roman")});
        font8.setPointSize(10);
        checkBoxSouvenir->setFont(font8);
        pushButtonConnexion = new QPushButton(pageLogin);
        pushButtonConnexion->setObjectName("pushButtonConnexion");
        pushButtonConnexion->setGeometry(QRect(780, 555, 320, 50));
        QFont font9;
        font9.setFamilies({QString::fromUtf8("Arial")});
        font9.setPointSize(12);
        font9.setBold(true);
        pushButtonConnexion->setFont(font9);
        pushButtonConnexion->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        pushButtonCGU = new QPushButton(pageLogin);
        pushButtonCGU->setObjectName("pushButtonCGU");
        pushButtonCGU->setGeometry(QRect(780, 630, 320, 25));
        pushButtonCGU->setFont(font8);
        pushButtonCGU->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        pushButtonPolitique = new QPushButton(pageLogin);
        pushButtonPolitique->setObjectName("pushButtonPolitique");
        pushButtonPolitique->setGeometry(QRect(780, 655, 320, 25));
        pushButtonPolitique->setFont(font8);
        pushButtonPolitique->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        pushButtonAPropos = new QPushButton(pageLogin);
        pushButtonAPropos->setObjectName("pushButtonAPropos");
        pushButtonAPropos->setGeometry(QRect(780, 680, 320, 25));
        pushButtonAPropos->setFont(font8);
        pushButtonAPropos->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        labelLoginFooter = new QLabel(pageLogin);
        labelLoginFooter->setObjectName("labelLoginFooter");
        labelLoginFooter->setGeometry(QRect(780, 720, 320, 20));
        QFont font10;
        font10.setFamilies({QString::fromUtf8("Times New Roman")});
        font10.setPointSize(8);
        labelLoginFooter->setFont(font10);
        labelLoginFooter->setAlignment(Qt::AlignmentFlag::AlignCenter);
        stackedWidget->addWidget(pageLogin);
        pageEmployes = new QWidget();
        pageEmployes->setObjectName("pageEmployes");
        scrollEmployes = new QScrollArea(pageEmployes);
        scrollEmployes->setObjectName("scrollEmployes");
        scrollEmployes->setGeometry(QRect(0, 0, 1440, 900));
        scrollEmployes->setFrameShape(QFrame::Shape::NoFrame);
        scrollEmployes->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);
        scrollEmployes->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        scrollEmployes->setWidgetResizable(false);
        scrollEmployesContent = new QWidget();
        scrollEmployesContent->setObjectName("scrollEmployesContent");
        scrollEmployesContent->setGeometry(QRect(0, 0, 1420, 1000));
        frameNavbar = new QFrame(scrollEmployesContent);
        frameNavbar->setObjectName("frameNavbar");
        frameNavbar->setGeometry(QRect(9, 10, 1401, 76));
        frameNavbar->setFrameShape(QFrame::Shape::StyledPanel);
        frameNavbar->setFrameShadow(QFrame::Shadow::Raised);
        labelNavLogo = new QLabel(frameNavbar);
        labelNavLogo->setObjectName("labelNavLogo");
        labelNavLogo->setGeometry(QRect(10, 10, 61, 61));
        QFont font11;
        font11.setFamilies({QString::fromUtf8("Arial")});
        font11.setPointSize(13);
        font11.setBold(true);
        labelNavLogo->setFont(font11);
        labelNavLogo->setAlignment(Qt::AlignmentFlag::AlignCenter);
        labelNavBrand = new QLabel(scrollEmployesContent);
        labelNavBrand->setObjectName("labelNavBrand");
        labelNavBrand->setGeometry(QRect(90, 20, 140, 28));
        labelNavBrand->setFont(font11);
        labelNavSub = new QLabel(scrollEmployesContent);
        labelNavSub->setObjectName("labelNavSub");
        labelNavSub->setGeometry(QRect(90, 50, 140, 18));
        QFont font12;
        font12.setFamilies({QString::fromUtf8("Arial")});
        font12.setPointSize(8);
        font12.setBold(true);
        labelNavSub->setFont(font12);
        btnDashboard = new QPushButton(scrollEmployesContent);
        btnDashboard->setObjectName("btnDashboard");
        btnDashboard->setGeometry(QRect(230, 30, 130, 40));
        btnDashboard->setFont(font12);
        btnDashboard->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        btnEmployes = new QPushButton(scrollEmployesContent);
        btnEmployes->setObjectName("btnEmployes");
        btnEmployes->setGeometry(QRect(370, 30, 100, 40));
        btnEmployes->setFont(font12);
        btnEmployes->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        btnSalles = new QPushButton(scrollEmployesContent);
        btnSalles->setObjectName("btnSalles");
        btnSalles->setGeometry(QRect(480, 30, 100, 40));
        btnSalles->setFont(font12);
        btnSalles->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        btnEvaluateurs = new QPushButton(scrollEmployesContent);
        btnEvaluateurs->setObjectName("btnEvaluateurs");
        btnEvaluateurs->setGeometry(QRect(590, 30, 130, 40));
        btnEvaluateurs->setFont(font12);
        btnEvaluateurs->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        btnParticipants = new QPushButton(scrollEmployesContent);
        btnParticipants->setObjectName("btnParticipants");
        btnParticipants->setGeometry(QRect(730, 30, 130, 40));
        btnParticipants->setFont(font12);
        btnParticipants->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        btnSponsors = new QPushButton(scrollEmployesContent);
        btnSponsors->setObjectName("btnSponsors");
        btnSponsors->setGeometry(QRect(870, 30, 110, 40));
        btnSponsors->setFont(font12);
        btnSponsors->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        btnProjets = new QPushButton(scrollEmployesContent);
        btnProjets->setObjectName("btnProjets");
        btnProjets->setGeometry(QRect(990, 30, 100, 40));
        btnProjets->setFont(font12);
        btnProjets->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        btnNotifications = new QPushButton(scrollEmployesContent);
        btnNotifications->setObjectName("btnNotifications");
        btnNotifications->setGeometry(QRect(1100, 30, 110, 40));
        btnNotifications->setFont(font12);
        btnNotifications->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        btnProfil = new QPushButton(scrollEmployesContent);
        btnProfil->setObjectName("btnProfil");
        btnProfil->setGeometry(QRect(1215, 30, 80, 40));
        btnProfil->setFont(font12);
        btnProfil->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        btnDeconnexion = new QPushButton(scrollEmployesContent);
        btnDeconnexion->setObjectName("btnDeconnexion");
        btnDeconnexion->setGeometry(QRect(1300, 30, 101, 40));
        btnDeconnexion->setFont(font12);
        btnDeconnexion->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        labelTitleEmployes = new QLabel(scrollEmployesContent);
        labelTitleEmployes->setObjectName("labelTitleEmployes");
        labelTitleEmployes->setGeometry(QRect(10, 100, 600, 36));
        QFont font13;
        font13.setFamilies({QString::fromUtf8("Arial")});
        font13.setPointSize(18);
        font13.setBold(true);
        labelTitleEmployes->setFont(font13);
        labelSubEmployes = new QLabel(scrollEmployesContent);
        labelSubEmployes->setObjectName("labelSubEmployes");
        labelSubEmployes->setGeometry(QRect(20, 140, 800, 22));
        labelSubEmployes->setFont(font7);
        cardFormEmployes = new QFrame(scrollEmployesContent);
        cardFormEmployes->setObjectName("cardFormEmployes");
        cardFormEmployes->setGeometry(QRect(10, 170, 1391, 181));
        cardFormEmployes->setFrameShape(QFrame::Shape::StyledPanel);
        cardFormEmployes->setFrameShadow(QFrame::Shadow::Raised);
        labelFormTitleEmployes = new QLabel(scrollEmployesContent);
        labelFormTitleEmployes->setObjectName("labelFormTitleEmployes");
        labelFormTitleEmployes->setGeometry(QRect(30, 180, 500, 25));
        labelFormTitleEmployes->setFont(font11);
        lblEmpID = new QLabel(scrollEmployesContent);
        lblEmpID->setObjectName("lblEmpID");
        lblEmpID->setGeometry(QRect(50, 220, 120, 18));
        lblEmpID->setFont(font5);
        lineEditEmpID = new QLineEdit(scrollEmployesContent);
        lineEditEmpID->setObjectName("lineEditEmpID");
        lineEditEmpID->setGeometry(QRect(40, 240, 150, 40));
        lineEditEmpID->setFont(font7);
        lineEditEmpID->setReadOnly(true);
        lblEmpNom = new QLabel(scrollEmployesContent);
        lblEmpNom->setObjectName("lblEmpNom");
        lblEmpNom->setGeometry(QRect(230, 220, 120, 18));
        lblEmpNom->setFont(font5);
        lineEditEmpNom = new QLineEdit(scrollEmployesContent);
        lineEditEmpNom->setObjectName("lineEditEmpNom");
        lineEditEmpNom->setGeometry(QRect(230, 240, 200, 40));
        lineEditEmpNom->setFont(font7);
        lblEmpPrenom = new QLabel(scrollEmployesContent);
        lblEmpPrenom->setObjectName("lblEmpPrenom");
        lblEmpPrenom->setGeometry(QRect(450, 220, 120, 18));
        lblEmpPrenom->setFont(font5);
        lineEditEmpPrenom = new QLineEdit(scrollEmployesContent);
        lineEditEmpPrenom->setObjectName("lineEditEmpPrenom");
        lineEditEmpPrenom->setGeometry(QRect(450, 240, 200, 40));
        lineEditEmpPrenom->setFont(font7);
        lblEmpMdp = new QLabel(scrollEmployesContent);
        lblEmpMdp->setObjectName("lblEmpMdp");
        lblEmpMdp->setGeometry(QRect(670, 220, 140, 18));
        lblEmpMdp->setFont(font5);
        lineEditEmpMdp = new QLineEdit(scrollEmployesContent);
        lineEditEmpMdp->setObjectName("lineEditEmpMdp");
        lineEditEmpMdp->setGeometry(QRect(670, 240, 200, 40));
        lineEditEmpMdp->setFont(font7);
        lineEditEmpMdp->setEchoMode(QLineEdit::EchoMode::Password);
        lblEmpRole = new QLabel(scrollEmployesContent);
        lblEmpRole->setObjectName("lblEmpRole");
        lblEmpRole->setGeometry(QRect(890, 220, 140, 18));
        lblEmpRole->setFont(font5);
        comboBoxEmpRole = new QComboBox(scrollEmployesContent);
        comboBoxEmpRole->addItem(QString());
        comboBoxEmpRole->addItem(QString());
        comboBoxEmpRole->setObjectName("comboBoxEmpRole");
        comboBoxEmpRole->setGeometry(QRect(890, 240, 220, 40));
        comboBoxEmpRole->setFont(font7);
        lblEmpStatut = new QLabel(scrollEmployesContent);
        lblEmpStatut->setObjectName("lblEmpStatut");
        lblEmpStatut->setGeometry(QRect(1130, 220, 140, 18));
        lblEmpStatut->setFont(font5);
        comboBoxEmpStatut = new QComboBox(scrollEmployesContent);
        comboBoxEmpStatut->addItem(QString());
        comboBoxEmpStatut->addItem(QString());
        comboBoxEmpStatut->setObjectName("comboBoxEmpStatut");
        comboBoxEmpStatut->setGeometry(QRect(1130, 240, 200, 40));
        comboBoxEmpStatut->setFont(font7);
        btnAjouterEmploye = new QPushButton(scrollEmployesContent);
        btnAjouterEmploye->setObjectName("btnAjouterEmploye");
        btnAjouterEmploye->setGeometry(QRect(40, 300, 120, 40));
        btnAjouterEmploye->setFont(font2);
        btnAjouterEmploye->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        btn_Annuler = new QPushButton(scrollEmployesContent);
        btn_Annuler->setObjectName("btn_Annuler");
        btn_Annuler->setGeometry(QRect(180, 300, 120, 40));
        btn_Annuler->setFont(font2);
        btn_Annuler->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        lineEdit = new QLineEdit(scrollEmployesContent);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(10, 360, 351, 51));
        btnRechercherEmploye = new QPushButton(scrollEmployesContent);
        btnRechercherEmploye->setObjectName("btnRechercherEmploye");
        btnRechercherEmploye->setGeometry(QRect(380, 360, 130, 51));
        btnRechercherEmploye->setFont(font2);
        btnRechercherEmploye->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        btnTrierEmploye = new QPushButton(scrollEmployesContent);
        btnTrierEmploye->setObjectName("btnTrierEmploye");
        btnTrierEmploye->setGeometry(QRect(530, 359, 120, 51));
        btnTrierEmploye->setFont(font2);
        btnTrierEmploye->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        btnExporterEmploye = new QPushButton(scrollEmployesContent);
        btnExporterEmploye->setObjectName("btnExporterEmploye");
        btnExporterEmploye->setGeometry(QRect(670, 359, 130, 51));
        btnExporterEmploye->setFont(font2);
        btnExporterEmploye->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        cardTableEmployes = new QFrame(scrollEmployesContent);
        cardTableEmployes->setObjectName("cardTableEmployes");
        cardTableEmployes->setGeometry(QRect(10, 420, 931, 461));
        cardTableEmployes->setFrameShape(QFrame::Shape::StyledPanel);
        cardTableEmployes->setFrameShadow(QFrame::Shadow::Raised);
        tableEmployes = new QTableWidget(cardTableEmployes);
        if (tableEmployes->columnCount() < 7)
            tableEmployes->setColumnCount(7);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableEmployes->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableEmployes->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableEmployes->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableEmployes->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableEmployes->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableEmployes->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableEmployes->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        tableEmployes->setObjectName("tableEmployes");
        tableEmployes->setGeometry(QRect(20, 50, 891, 391));
        tableEmployes->setFont(font7);
        tableEmployes->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
        tableEmployes->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
        tableEmployes->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
        labelTableTitleEmployes = new QLabel(cardTableEmployes);
        labelTableTitleEmployes->setObjectName("labelTableTitleEmployes");
        labelTableTitleEmployes->setGeometry(QRect(20, 10, 500, 25));
        labelTableTitleEmployes->setFont(font11);
        frameChatbot = new QFrame(scrollEmployesContent);
        frameChatbot->setObjectName("frameChatbot");
        frameChatbot->setGeometry(QRect(951, 420, 451, 220));
        frameChatbot->setFrameShape(QFrame::Shape::Box);
        frameChatbot->setFrameShadow(QFrame::Shadow::Raised);
        verticalLayout_6 = new QVBoxLayout(frameChatbot);
        verticalLayout_6->setSpacing(10);
        verticalLayout_6->setObjectName("verticalLayout_6");
        verticalLayout_6->setContentsMargins(15, 15, 15, 15);
        lblTitreChatbot = new QLabel(frameChatbot);
        lblTitreChatbot->setObjectName("lblTitreChatbot");
        QFont font14;
        font14.setFamilies({QString::fromUtf8("Arial")});
        font14.setPointSize(11);
        font14.setBold(true);
        lblTitreChatbot->setFont(font14);
        lblTitreChatbot->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_6->addWidget(lblTitreChatbot);

        textChatbot = new QTextBrowser(frameChatbot);
        textChatbot->setObjectName("textChatbot");
        textChatbot->setMinimumSize(QSize(0, 110));

        verticalLayout_6->addWidget(textChatbot);

        frameSaisieChatbot = new QFrame(frameChatbot);
        frameSaisieChatbot->setObjectName("frameSaisieChatbot");
        frameSaisieChatbot->setFrameShape(QFrame::Shape::Box);
        frameSaisieChatbot->setFrameShadow(QFrame::Shadow::Raised);
        horizontalLayout_6 = new QHBoxLayout(frameSaisieChatbot);
        horizontalLayout_6->setObjectName("horizontalLayout_6");
        leQuestion = new QLineEdit(frameSaisieChatbot);
        leQuestion->setObjectName("leQuestion");

        horizontalLayout_6->addWidget(leQuestion);

        btnEnvoyer = new QPushButton(frameSaisieChatbot);
        btnEnvoyer->setObjectName("btnEnvoyer");

        horizontalLayout_6->addWidget(btnEnvoyer);


        verticalLayout_6->addWidget(frameSaisieChatbot);

        cardStats = new QFrame(scrollEmployesContent);
        cardStats->setObjectName("cardStats");
        cardStats->setGeometry(QRect(951, 640, 451, 230));
        cardStats->setFrameShape(QFrame::Shape::Box);
        cardStats->setFrameShadow(QFrame::Shadow::Raised);
        labelStatsTitle = new QLabel(cardStats);
        labelStatsTitle->setObjectName("labelStatsTitle");
        labelStatsTitle->setGeometry(QRect(15, 10, 400, 25));
        labelStatsTitle->setFont(font14);
        labelStatutPie = new QLabel(cardStats);
        labelStatutPie->setObjectName("labelStatutPie");
        labelStatutPie->setGeometry(QRect(10, 40, 210, 185));
        labelRoleBar = new QLabel(cardStats);
        labelRoleBar->setObjectName("labelRoleBar");
        labelRoleBar->setGeometry(QRect(225, 40, 220, 185));
        labelFooterEmployes = new QLabel(scrollEmployesContent);
        labelFooterEmployes->setObjectName("labelFooterEmployes");
        labelFooterEmployes->setGeometry(QRect(40, 930, 900, 20));
        QFont font15;
        font15.setFamilies({QString::fromUtf8("Times New Roman")});
        font15.setPointSize(9);
        labelFooterEmployes->setFont(font15);
        scrollEmployes->setWidget(scrollEmployesContent);
        stackedWidget->addWidget(pageEmployes);
        integration->setCentralWidget(centralWidget);

        retranslateUi(integration);

        stackedWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(integration);
    } // setupUi

    void retranslateUi(QMainWindow *integration)
    {
        integration->setWindowTitle(QCoreApplication::translate("integration", "HACKNOVA \342\200\224 Smart Hackathon Management", nullptr));
        labelLoginLogo->setText(QCoreApplication::translate("integration", "HN", nullptr));
        labelLoginBrandTitle->setText(QCoreApplication::translate("integration", "HACKNOVA", nullptr));
        labelLoginBrandSub->setText(QCoreApplication::translate("integration", "SMART HACKATHON MANAGEMENT", nullptr));
        labelLoginBrandSlogan->setText(QCoreApplication::translate("integration", "Plateforme intelligente de gestion de hackathons universitaires.", nullptr));
        labelLoginBrandVersion->setText(QCoreApplication::translate("integration", "VERSION 1.0.0", nullptr));
        labelLoginBrandTeam->setText(QCoreApplication::translate("integration", "GROUPE EL FALLEGA", nullptr));
        labelLoginTitle->setText(QCoreApplication::translate("integration", "Bienvenue", nullptr));
        labelLoginSub->setText(QCoreApplication::translate("integration", "Connectez-vous pour acc\303\251der \303\240 votre espace.", nullptr));
        labelLoginField->setText(QCoreApplication::translate("integration", "IDENTIFIANT", nullptr));
        lineEditIdentifiant->setPlaceholderText(QCoreApplication::translate("integration", "Entrez votre identifiant", nullptr));
        labelLoginField2->setText(QCoreApplication::translate("integration", "MOT DE PASSE", nullptr));
        lineEditMotDePasse->setPlaceholderText(QCoreApplication::translate("integration", "Entrez votre mot de passe", nullptr));
        checkBoxSouvenir->setText(QCoreApplication::translate("integration", "Se souvenir de moi", nullptr));
        pushButtonConnexion->setText(QCoreApplication::translate("integration", "SE CONNECTER", nullptr));
        pushButtonCGU->setText(QCoreApplication::translate("integration", "Conditions g\303\251n\303\251rales d'utilisation", nullptr));
        pushButtonPolitique->setText(QCoreApplication::translate("integration", "Politique d'utilisation", nullptr));
        pushButtonAPropos->setText(QCoreApplication::translate("integration", "\303\200 propos de HACKNOVA", nullptr));
        labelLoginFooter->setText(QCoreApplication::translate("integration", "\302\251 2025 HACKNOVA \342\200\224 Tous droits r\303\251serv\303\251s", nullptr));
        labelNavLogo->setText(QString());
        labelNavBrand->setText(QCoreApplication::translate("integration", "HACKNOVA", nullptr));
        labelNavSub->setText(QCoreApplication::translate("integration", "SMART HACKATHON", nullptr));
        btnDashboard->setText(QCoreApplication::translate("integration", "TABLEAU DE BORD", nullptr));
        btnEmployes->setText(QCoreApplication::translate("integration", "\360\237\221\244 EMPLOY\303\211S", nullptr));
        btnSalles->setText(QCoreApplication::translate("integration", "\360\237\217\233 SALLES", nullptr));
        btnEvaluateurs->setText(QCoreApplication::translate("integration", "\360\237\223\212 \303\211VALUATEURS", nullptr));
        btnParticipants->setText(QCoreApplication::translate("integration", "\360\237\221\245 PARTICIPANTS", nullptr));
        btnSponsors->setText(QCoreApplication::translate("integration", "\360\237\244\235 SPONSORS", nullptr));
        btnProjets->setText(QCoreApplication::translate("integration", "\360\237\222\241 PROJETS", nullptr));
        btnNotifications->setText(QCoreApplication::translate("integration", "NOTIFICATIONS", nullptr));
        btnProfil->setText(QCoreApplication::translate("integration", "PROFIL", nullptr));
        btnDeconnexion->setText(QCoreApplication::translate("integration", "D\303\211CONNEXION", nullptr));
        labelTitleEmployes->setText(QCoreApplication::translate("integration", "\360\237\221\244 Gestion des employ\303\251s", nullptr));
        labelSubEmployes->setText(QCoreApplication::translate("integration", "G\303\251rez les comptes, r\303\264les et privil\303\250ges des employ\303\251s", nullptr));
        labelFormTitleEmployes->setText(QCoreApplication::translate("integration", "Ajouter un employ\303\251", nullptr));
        lblEmpID->setText(QCoreApplication::translate("integration", "IDENTIFIANT", nullptr));
        lineEditEmpID->setPlaceholderText(QCoreApplication::translate("integration", "Auto", nullptr));
        lblEmpNom->setText(QCoreApplication::translate("integration", "NOM", nullptr));
        lineEditEmpNom->setPlaceholderText(QCoreApplication::translate("integration", "Nom", nullptr));
        lblEmpPrenom->setText(QCoreApplication::translate("integration", "PR\303\211NOM", nullptr));
        lineEditEmpPrenom->setPlaceholderText(QCoreApplication::translate("integration", "Pr\303\251nom", nullptr));
        lblEmpMdp->setText(QCoreApplication::translate("integration", "MOT DE PASSE", nullptr));
        lineEditEmpMdp->setPlaceholderText(QCoreApplication::translate("integration", "\342\200\242\342\200\242\342\200\242\342\200\242\342\200\242\342\200\242\342\200\242\342\200\242", nullptr));
        lblEmpRole->setText(QCoreApplication::translate("integration", "R\303\224LE", nullptr));
        comboBoxEmpRole->setItemText(0, QCoreApplication::translate("integration", "Administrateur", nullptr));
        comboBoxEmpRole->setItemText(1, QCoreApplication::translate("integration", "Responsable RH", nullptr));

        lblEmpStatut->setText(QCoreApplication::translate("integration", "STATUT", nullptr));
        comboBoxEmpStatut->setItemText(0, QCoreApplication::translate("integration", "Actif", nullptr));
        comboBoxEmpStatut->setItemText(1, QCoreApplication::translate("integration", "Inactif", nullptr));

        btnAjouterEmploye->setText(QCoreApplication::translate("integration", "AJOUTER", nullptr));
        btn_Annuler->setText(QCoreApplication::translate("integration", "ANNULER", nullptr));
        lineEdit->setPlaceholderText(QCoreApplication::translate("integration", "Rechercher un employ\303\251 par son id ", nullptr));
        btnRechercherEmploye->setText(QCoreApplication::translate("integration", "RECHERCHER", nullptr));
        btnTrierEmploye->setText(QCoreApplication::translate("integration", "TRIER", nullptr));
        btnExporterEmploye->setText(QCoreApplication::translate("integration", "EXPORTER", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableEmployes->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("integration", "ID", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableEmployes->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("integration", "Nom", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableEmployes->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("integration", "Pr\303\251nom", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableEmployes->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("integration", "Mot de passe", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableEmployes->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("integration", "R\303\264le", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableEmployes->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("integration", "Statut", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tableEmployes->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("integration", "Action", nullptr));
        labelTableTitleEmployes->setText(QCoreApplication::translate("integration", "Liste des employ\303\251s", nullptr));
        lblTitreChatbot->setText(QCoreApplication::translate("integration", "\360\237\244\226 Chatbot intelligent d'assistance", nullptr));
        leQuestion->setPlaceholderText(QCoreApplication::translate("integration", "\303\211crivez votre question...", nullptr));
        btnEnvoyer->setText(QCoreApplication::translate("integration", "Envoyer", nullptr));
        labelStatsTitle->setText(QCoreApplication::translate("integration", "\360\237\223\212 Statistiques des employ\303\251s", nullptr));
        labelStatutPie->setText(QString());
        labelRoleBar->setText(QString());
        labelFooterEmployes->setText(QCoreApplication::translate("integration", "HACKNOVA \342\200\224 Smart Hackathon Management \342\200\224 Version 1.0.0", nullptr));
    } // retranslateUi

};

namespace Ui {
    class integration: public Ui_integration {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INTEGRATION_H
