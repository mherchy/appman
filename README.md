# APP-MAN

Multi-thread pacman clone in C

##Introduction

APP-MAN est un jeu vidéo inspiré de PAC-MAN. Pour le réaliser nous avons utilisé des outils étudiés en PRS ainsi qu&#39;en LPE.



##Principe du jeu

Tout d&#39;abord, nous allons voir le principe du jeu original : PAC-MAN.

PAC-MAN est un jeu d&#39;arcade créé par Toru Iwatani et sorti en 1980.

Le principe du jeu est de collecter toutes les &quot;pièces&quot;, en se déplaçant à travers un labyrinthe, pour finir le niveau. La difficulté du jeu réside dans le fait que quatre fantômes essayent de nous attraper pour se défendre, PAC-MAN peut manger des boules (les gros &quot;points&quot; au quatres coins du labyrinthe) et ce faisant il peut à son tour chasser les fantômes pendant une durée limitée afin de les neutraliser pendant un court instant. Le jeu possède un système de scoring et propose au joueur trois vies avant le &quot;Game Over&quot;. Le jeu possède aussi des règles de déplacements : le joueurs et les fantômes ne peuvent pas traverser les murs et les fantômes ne peuvent jamais faire demi-tour.

Pour notre version APP-MAN, nous n&#39;avons pas pu développer toutes les fonctionnalités du jeu de base car le projet était ambitieux et nous avons du faire un choix dans les fonctionnalités à implémenter. C&#39;est pour cette raison que notre jeu ne possède pas  de système de scoring, il ne propose qu&#39;une seule vie au joueur et il n&#39;y a pas les quatre boules qui permettent de chasser les fantômes (ce qui rend le jeu un peu plus difficile).



##Contraintes

Pour réaliser ce jeu, nous avons eu affaire  à trois grandes contraintes.

La première est la nécessité d&#39;avoir un affichage graphique car on ne pouvait pas avoir un affiche en mode prompt.

La deuxième contrainte est la nécessité d&#39;avoir une gestion du temps dans le jeu, en effet même si le joueur ne fait rien les IA doivent continuer à avancer. De plus cela permet de donner au jeu une fréquence de rafraîchissement pour l&#39;affiche du joueur et des ennemis.

 La troisième contrainte est la mise en place d&#39;une intelligence artificielle afin de gérer les déplacements des fantômes de façon à ce qu&#39;ils se dirige vers le joueur tout en respectant les règles de déplacement du labyrinthe.

##Architecture choisie

###Outils utilisés :

- _Forks : Le processus père produit 3 processus fils qui vont chacun gérer un traitement particulier : le système de jeu, l&#39;écoute clavier et l&#39;affichage. Le fork permet de faire fonctionner ces traitements en parallèle et de manière autonomes._
- _Threads : le processus Jeu créé 4 threads à chaque temporisation, qui correspondent aux IA. Les threads permettent de lancer rapidement des traitements parallèles qui dépendent d&#39;un thread principale._
- _Mémoire partagée : Afin de partager les informations entre les différents processus, on utilise des mémoires partagées. Il y  en a 3 : overview (positions de tous les joueurs et IA), map (Structure et état du labyrinthe), lastkey (dernière touche pressée par l&#39;utilisateur)._
- _Sémaphores : permettent de sécuriser l&#39;utilisation de la mémoire partagée. Ces sémaphores sont nommés._
- _Signaux :_
  - _USR1 : le processus principale envoi un signal SIGUSR1 au processus JEU et AFFICHAGE pour donner l&#39;ordre de rafraîchir les données._
  - _TERM : Le processus joueur envoi un signal SIGTERM au processus père en cas d&#39;appuie sur la touche F1. Le père s&#39;occupe ensuite de tuer ses processus fils_
- _Tubes (debug) : Afficher des informations sur un autre terminal, car le terminal principal est bloqué par ncurses_

###Chronologie :

1. main : initialisation
	* fifo
	* gestionnaire de signaux
	* shm et sémaphores attachés
2. fork : processus JEU
3. main : Initialisation de ncurses
4. fork : processus CLAVIER
	* Écoute constante du clavier : écriture dans la mémoire partagée
5. fork : processus AFFICHAGE
6. traitement principal (boucle) :
	* Courte attente (0.2s)
	* main : Envoi d&#39;un signal SIGUSR1 au processus JEU :
		* Réception du signal
		* Création des IA (threads)
		* Fin de traitement des IA (join) récupération de leur déplacements
		* Ecriture des nouvelles positions IA dans la mémoire partagée
		* Récupération et écriture des déplacements du joueurs
	* Courte attente (0.2s)
	* main : Envoi d&#39;un signal SIGUSR1 au processus AFFICHAGE :
		* Réception du signal
		* Récupération des données dans la mémoire partagée
		* Affichage à l&#39;écran
7. main : Réception d&#39;un SIGTERM
8. Kill des processus fils
	* Pour chaque processus fils, déclenchement du handler atexit
	* Détachement des shm et sémaphores
9. Fermetures :
	* ncurses
	* tube



##Difficultés rencontrés

- Le problème majeur rencontré est la difficulté de déboguer. Cette difficulté est liée à l&#39;utilisation de ncurses car ncurses &quot;bloque&quot; le terminal est empêche donc l&#39;utilisation de printf et l&#39;affichage de logs.
  - Pour répondre à ce problème d&#39;affichage pour les logs et le débogage nous avons dû utiliser les tubes  afin d&#39;envoyer les informations sur un autre terminal.

##Conclusion

APP-MAN nous a permis d&#39;utiliser la plupart des technologies étudiées en PRS (thread, fork, sémaphore, tube, mémoire partagée et signaux) et même plus encore avec l&#39;utilisation de ncurses pour l&#39;affichage. Grâce à ce jeu nous avons donc pu mieux comprendre et mieux maîtriser ces outils.


