Smart Hackathon Management — Métier 2 refondu

Contenu du projet :
- SmartHackathon_GestionEvaluateurs.pro : projet Qt/qmake
- main.cpp : point d'entrée
- gestion_evaluateurs.cpp / gestion_evaluateurs.h : logique de l'application
- GestionEvaluateurs.ui : interface principale du Métier 1

Métier 2 a été refondu :
- Interface simplifiée, avec sélection d'un projet.
- La spécialité attendue est liée au projet sélectionné.
- Le tableau des évaluateurs montre ID, nom, statut et spécialités modifiables.
- Un seul évaluateur actif est proposé par projet.
- La correspondance de spécialité est exacte (insensible à la casse et aux espaces superflus) : un profil Développement n'est pas compatible avec Réseaux.
- Le résultat explique clairement si aucun évaluateur actif n'a la spécialité demandée.
- Export CSV du dernier résultat.

Compilation :
1. Extraire le ZIP dans un nouveau dossier.
2. Ouvrir SmartHackathon_GestionEvaluateurs.pro dans Qt Creator.
3. Choisir un kit Desktop Qt compatible.
4. Exécuter Run qmake, puis Rebuild All et Run.

Note : l'archive contient le code source complet. La compilation n'a pas pu être exécutée dans l'environnement de création faute de kit Qt/qmake disponible ; compilez dans Qt Creator pour valider avec votre installation.
