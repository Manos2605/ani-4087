# Exercice 7 — Les vingt-deux triées

## Classement des projets

Prenez la liste des vingt-trois dépendances de la démonstration du moteur et classez-la en trois groupes : celles dont le nom suffit à deviner le rôle, celles dont vous avez une idée sans certitude, celles dont vous ne savez rien.

Pour ce troisième groupe, ouvrez l'en-tête principal de chaque module et rendez une phrase par module.
## Solution
Pour afficher la liste des projets d'exemple disponibles dans Jenga, j'ai utilisé la commande :
```
jenga examples list
```
Cette commande affiche la liste des projets d'exemple, leur nom, leur identifiant, leur description, les plateformes supportées et leur niveau de difficulté.

J'ai ensuite classé les projets selon ce que leur nom permet de comprendre.

### 1. Dépendances dont le nom suffit à comprendre le rôle

Pour cette partie, le nom du projet me permet déjà de comprendre plus ou moins ce à quoi il sert.

* **Hello Console** : c'est simplement une application qui affiche des informations dans la console.
* **Static Library** : permet de créer et d'utiliser une bibliothèque statique.
* **Shared Library** : permet de créer et d'utiliser une bibliothèque partagée.
* **Unit Tests** : sert à faire des tests unitaires sur le programme.
* **Android NDK** : permet de développer une application Android avec du code natif.
* **iOS Application** : permet de créer une application native pour iOS.
* **WebAssembly** : sert à compiler du code pour l'exécuter avec WebAssembly.
* **Custom Toolchain** : permet d'utiliser une toolchain personnalisée.
* **Multi-Project Workspace** : permet de gérer plusieurs projets dans un même workspace.
* **Benchmarking** : sert à mesurer les performances d'un programme.
* **External Includes** : permet d'utiliser des bibliothèques ou des fichiers d'en-tête externes.
* **Packaging** : sert à préparer un projet pour sa distribution.
* **Cross Compilation** : permet de compiler un programme pour une autre plateforme.
* **Win32 Window** : permet de créer une fenêtre Windows avec Win32.
* **X11 Window (Linux)** : permet de créer une fenêtre sous Linux avec X11.
* **Cocoa Window (macOS)** : permet de créer une fenêtre native sous macOS avec Cocoa.
* **Android Native Window** : permet de créer une fenêtre native Android.
* **Web Canvas** : permet d'utiliser un canvas sur le Web avec WebAssembly.
* **iOS UIKit Window** : permet de créer une fenêtre iOS avec UIKit.
* **Zig Cross Compilation** : permet de faire de la compilation croisée avec Zig.
* **Android SDL3 (ndk-build)** : permet d'utiliser SDL3 pour Android avec `ndk-build`.
* **All Platforms** : projet configuré pour fonctionner sur plusieurs plateformes.
* **OpenGL Triangle** : montre un exemple simple d'utilisation d'OpenGL/GLES avec un triangle.
* **Xbox Project Kinds** : présente différents types de projets pour Xbox.
* **Xbox UWP Dev Mode** : montre un projet Xbox utilisant le mode développeur UWP.

### 2. Le nom donne une idée, mais je ne suis pas totalement sûr

Pour certains projets, le nom me donne une idée de ce qu'ils font, mais je ne peux pas connaître exactement leur fonctionnement seulement avec le nom.

* **C++20 Modules** : je comprends que ça concerne les modules introduits avec C++20, mais je ne connais pas encore exactement comment ils sont utilisés dans Jenga.
* **Nuklear Multi-Platform Sandbox** : je comprends que c'est un projet graphique qui utilise Nuklear et qui fonctionne sur plusieurs plateformes. Par contre, le terme « Sandbox » ne me permet pas de savoir exactement ce que le projet fait.
* **NK Window Framework** : le nom me fait penser à un framework pour gérer les fenêtres sur plusieurs plateformes, mais je ne connais pas exactement le rôle de `NK`.

### 3. Ce que je ne connaissais pas

Pour les projets où je n'étais pas sûr, il faudrait regarder les fichiers sources et surtout les fichiers d'en-tête principaux en profondeur pour mieux comprendre leur fonctionnement.

* **Nuklear Multi-Platform Sandbox** : Nuklear est utilisé pour gérer l'interface graphique et le projet sert surtout à montrer son utilisation sur plusieurs plateformes.
* **NK Window Framework** : c'est un framework qui permet de gérer les fenêtres sur différentes plateformes et qui est utilisé dans certaines démonstrations.
* **C++20 Modules** : ce projet montre comment utiliser les modules de C++20 avec les compilateurs qui les supportent.

## Remarque

Avec la commande `jenga examples list`, j'obtiens **28 projets d'exemple**. Certains sont faciles à comprendre juste avec leur nom, tandis que pour d'autres il faut regarder un peu en profondeur le code pour savoir exactement à quoi ils servent.

Dans mon cas, j'ai donc utilisé les noms et les descriptions affichés par Jenga pour faire le classement.