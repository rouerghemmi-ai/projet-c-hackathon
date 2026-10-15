#ifndef GESTION_EVALUATEURS_H
#define GESTION_EVALUATEURS_H

#include <QMainWindow>
#include <QStandardItemModel>

QT_BEGIN_NAMESPACE
namespace Ui { class GestionEvaluateurs; }
QT_END_NAMESPACE

class GestionEvaluateurs : public QMainWindow
{
    Q_OBJECT
public:
    explicit GestionEvaluateurs(QWidget *parent = nullptr);
    ~GestionEvaluateurs();

signals:
    void demandeEmployes();
    void demandeSalles();
    void demandeDeconnexion();

private slots:
    void ajouterEvaluateur();
    void modifierEvaluateur();
    void supprimerEvaluateur();
    void annulerFormulaire();
    void rechercherEvaluateur();
    void trierEvaluateurs();
    void exporterEvaluateurs();
    void selectionnerLigne();
    void lancerTechChallenge();
    void lancerAnalyseProjet();

private:
    Ui::GestionEvaluateurs *ui;
    QStandardItemModel *model;
    int ligneEnEdition;
    void mettreAJourStatistiques();
    bool formulaireValide() const;
    void afficherMessage(const QString &titre, const QString &message);
    void reconstruireBoutonsAction();
};

#endif
