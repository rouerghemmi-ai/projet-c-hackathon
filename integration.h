#ifndef INTEGRATION_H
#define INTEGRATION_H

#include <QMainWindow>
#include <QRandomGenerator>
#include <QPushButton>
#include <QList>
#include <QVariantMap>
#include <QString>

class GestionEvaluateurs;

QT_BEGIN_NAMESPACE
namespace Ui {
class integration;
}
QT_END_NAMESPACE

class integration : public QMainWindow
{
    Q_OBJECT

public:
    explicit integration(QWidget *parent = nullptr);
    ~integration() override;

private slots:
    void allerPage(int index);
    void goLogin();
    void goEmployes();
    void goSalles();
    void goEvaluateurs();

    void onConnexion();
    void onCGU();
    void onPolitique();
    void onAPropos();
    void onNotifications();
    void onProfil();
    void onDeconnexion();

    void onEnvoyerQuestion();   // Chatbot
    void onSuggererSalle();
    void onValiderQR();

    // Employés (Oracle)
    void onAjouterEmploye();
    void onAnnulerEmploye();
    void onRechercherEmploye();
    void onTrierEmploye();
    void onExporterEmploye();
    // Salles (Oracle)
    void onAjouterSalle();
    void onAnnulerSalle();
    void onRechercherSalle();
    void onTrierSalle();
    void onExporterSalle();

private:
    Ui::integration *ui;
    GestionEvaluateurs *m_evaluateurs = nullptr;

    void appliquerTheme();
    void connecterLogin();
    void connecterNavigation();
    void connecterChatbot();
    void connecterActions();
    void remplirTableEmployes();
    void remplirTableSalles();
    void dessinerStatistiquesSalles();
    void dessinerStatistiques(); // ← NOUVEAU

    // --- Oracle : employés / salles (integration_db.cpp) ---
    void chargerEmployes();
    void afficherEmployes();
    void modifierEmploye(const QString &id);
    void supprimerEmploye(const QString &id);
    void chargerSalles();
    void afficherSalles();
    void modifierSalle(const QString &id);
    void supprimerSalle(const QString &id);

    QList<QVariantMap> m_emp, m_salles;
    bool m_dbEmp = false, m_dbSalle = false;
    QString m_empIdEdition, m_salleIdEdition;
    QString m_empBtnTexte, m_salleBtnTexte;
    int m_empTri = 0, m_salleTri = 0;   // 0 = aucun, 1 = A→Z, 2 = Z→A
};

#endif // INTEGRATION_H