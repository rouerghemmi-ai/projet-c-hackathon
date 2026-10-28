#ifndef INTEGRATION_H
#define INTEGRATION_H

#include <QMainWindow>
#include <QPushButton>
#include <QLayout>

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

    void onConnexion();
    void onNotifications();
    void onProfil();
    void onDeconnexion();

    void onEnvoyerQuestion();
    void onSuggererSalle();
    void onValiderQR();

    // Module Salles — slots dédiés
    void onAjouterSalle();
    void onAnnulerSalle();
    void onRechercherSalle();
    void onTrierSalle();
    void onExporterSalle();

    // Slots publics pour les boutons MODIFIER / SUPPRIMER
    void ouvrirModificationSalle(qint64 idSalle);
    void ouvrirSuppressionSalle(qint64 idSalle, const QString &nomSalle);

private:
    Ui::integration *ui;

    // Installation
    void appliquerTheme();
    void connecterLogin();
    void connecterNavigation();
    void connecterChatbot();
    void connecterActions();

    // Chargement
    void remplirTableEmployes();
    void remplirTableSalles();

    // Statistiques
    void dessinerStatistiques();
    void dessinerStatistiquesSalles();

    // Helpers internes
    void rafraichirSallesEtStats();
    void viderFormulaireSalle();

    // Utilitaires ajoutés
    void viderTableSalles();
    QWidget *creerWidgetActions(qint64 idSalle, const QString &nomSalle);
    // NOTE : creerWidgetActionsRecherche supprimée (déclarée mais jamais définie)
};

#endif // INTEGRATION_H