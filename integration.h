#ifndef INTEGRATION_H
#define INTEGRATION_H

#include <QMainWindow>
#include <QRandomGenerator>
#include <QPushButton>

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
};

#endif // INTEGRATION_H