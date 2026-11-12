# Intégration — Gestion Employés + Gestion Salles + Gestion Évaluateurs

Une seule application Qt (`QStackedWidget`) :

| Index | Page |
|-------|------|
| 0 | Login |
| 1 | Gestion des employés |
| 2 | Gestion des salles |
| 3 | Gestion des évaluateurs (module `GestionEvaluateurs`) |

## Navigation
- Bouton **ÉVALUATEURS** (barre des pages Employés et Salles) → page Évaluateurs.
- Dans la page Évaluateurs : « Gestion des employés » / « Tableau de bord » → Employés, « Gestion des salles » → Salles, « Déconnexion sécurisée » → confirmation puis Login.

## Fichiers
- Ajoutés : `gestion_evaluateurs.cpp/.h`, `gestion_evaluateurs.ui`
- Modifiés : `integration.cpp/.h` (page 3, slots, thème), `hacknova.pro`
- `main.cpp`, `integration.ui` inchangés.

## Notes techniques
- `GestionEvaluateurs` émet `demandeEmployes()`, `demandeSalles()`, `demandeDeconnexion()` ; `integration` les relie à ses slots.
- Le thème sombre n'est plus posé sur toute la fenêtre mais sur `pageLogin`, `pageEmployes`, `pageSalles`, sinon il écrasait le style clair du module Évaluateurs. Un thème minimal reste sur la fenêtre pour les boîtes de dialogue.

## Compilation
Ouvrir `hacknova.pro` dans Qt Creator → Run qmake → Rebuild All → Run.

## Si erreur « No rule to make target 'integration.ui' »
1. Fermer Qt Creator, supprimer `hacknova.pro.user` et tout dossier `build-*`.
2. Vérifier que `integration.ui` et `gestion_evaluateurs.ui` sont dans le même dossier que `hacknova.pro`.
3. Rouvrir `hacknova.pro` → Compiler → Nettoyer tout → Exécuter qmake → Reconstruire tout.

## Oracle
Voir `GUIDE_ORACLE.md`. Nouveaux fichiers : `oracletable.cpp/.h`, `evaluateur.cpp/.h`, `integration_db.cpp`.
