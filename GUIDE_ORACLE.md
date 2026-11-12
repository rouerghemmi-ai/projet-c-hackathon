# Connexion Oracle ↔ Qt (hacknova)

Chaîne de connexion : `Qt (QSqlDatabase/QODBC)` → `DSN ODBC source_projet_2a` → `Oracle`.

## Où est quoi
| Fichier | Rôle |
|---------|------|
| `connection.cpp/.h` | Ouvre la connexion (DSN, utilisateur, mot de passe). Une seule fois, au démarrage (`main.cpp`). |
| `oracletable.cpp/.h` | Envoie les requêtes SQL (SELECT / INSERT / UPDATE / DELETE) sur une table. **Détecte automatiquement les colonnes** de ta table. |
| `evaluateur.cpp/.h` | Table `EVALUATEUR` → page Évaluateurs |
| `integration_db.cpp` | Tables `EMPLOYES` et `SALLE` → pages Employés et Salles |

## À vérifier avant de lancer
1. **Utilisateur Oracle** : `connection.cpp` utilise `chahd / chahd2005`.
   Dans SQL Developer, ta connexion s'appelle `projet`. Si les tables `EMPLOYES`, `EVALUATEUR`, `SALLE`
   appartiennent à un autre utilisateur que `chahd`, mets dans `connection.cpp` l'utilisateur et le mot de passe
   de la connexion `projet` (ou donne les droits SELECT/INSERT/UPDATE/DELETE à `chahd`).
2. **DSN** `source_projet_2a` : le test de connexion ODBC doit réussir (64 bits si Qt 64 bits).

## Correspondance des colonnes (automatique)
| Table | Champs reconnus (par le début du nom de colonne) |
|-------|--------------------------------------------------|
| EMPLOYES | `ID…`, `NOM`, `PRENOM`, `MOT_DE_PASSE`/`MDP`, `ROLE`, `STATUT`/`ETAT` |
| SALLE | `ID…`, `NOM`/`LIBELLE`, `CAPACIT…`, `TYPE…`, `DISPO…`/`STATUT`/`ETAT` |
| EVALUATEUR | `ID…`, `NOM`, `PRENOM`, `STATUT`/`ETAT`, `PROJET` |

- Un champ sans colonne correspondante est ignoré (ex. pas de colonne STATUT → affiché « Actif »).
- L'identifiant est généré automatiquement (MAX + 1, en gardant le format existant, ex. `EMP-004`, ou un nombre si la colonne est NUMBER).
- Pour changer une correspondance : modifier les motifs en haut de `integration_db.cpp` / `evaluateur.cpp`.
- Les colonnes détectées s'affichent dans la console Qt Creator (`Table EMPLOYES → ...`).

## Comportement
- Au démarrage : les 3 pages chargent leurs données depuis Oracle.
- Ajouter / Modifier / Supprimer / Rechercher / Trier / Exporter CSV fonctionnent sur la base.
- Si Oracle est injoignable : message d'erreur au démarrage, données de démonstration, modifications bloquées
  (page Évaluateurs : le titre indique **[Oracle connecté]** ou **[hors ligne]**).
- La page de connexion (login) n'est pas encore reliée à la table EMPLOYES.

## Erreurs fréquentes
| Message | Solution |
|---------|----------|
| `QODBC driver not available` | Plugin `qsqlodbc` absent de l'installation Qt. |
| `Data source name not found…` | DSN absent ou ODBC 32/64 bits ≠ kit Qt. |
| `ORA-01017` | Mauvais utilisateur / mot de passe. |
| `La table X est introuvable` | Les tables appartiennent à un autre utilisateur (voir point 1). |
| `ORA-01400` | La table a une colonne obligatoire que l'application ne remplit pas (ex. e-mail) → à me signaler. |
| `ORA-02292` | Ligne utilisée dans une autre table (EVALUER, REALISER…), supprimer d'abord les liens. |
