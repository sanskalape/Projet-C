# Document de définition de projet

## Nom du projet
Blind Test de musiques de jeux vidéo

## Objectif du projet
Créer une application simple en C avec SDL2 qui permet aux utilisateurs de tester leurs connaissances sur les musiques de jeux vidéo à travers un quiz interactif.

## Description du projet
Ce projet consiste à concevoir un jeu où les utilisateurs doivent reconnaître des morceaux de musique tirés de jeux vidéo célèbres. Le jeu commence par une page d’accueil où le joueur entre son nom et son prénom. Ensuite, il peut cliquer sur "Jouer" pour lancer une partie. Pendant la partie, des extraits de musiques sont joués, et le joueur doit sélectionner la bonne réponse parmi plusieurs propositions. À la fin, un écran affiche le score final et propose de rejouer ou de quitter.

## Les principales étapes du jeu

### Accueil
- L’utilisateur saisit son nom et son prénom.
- Un bouton "Jouer" permet de commencer la partie.

### Phase de jeu
- Entre 5 et 10 questions sont posées.
- Chaque question diffuse un extrait sonore.
- L’utilisateur doit saisir la réponse.
- Une fois la réponse validée, le jeu passe à la question suivante.

### Fin de partie
- Un écran affiche le score final (par exemple : "Vous avez obtenu 7/10").
- Deux choix sont proposés : Rejouer ou Quitter.
