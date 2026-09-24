# Exercice 19

## Énoncé

Faites planter volontairement votre programme sur l'appareil, par exemple en déréférençant un pointeur nul.

Retrouvez la trace dans le journal et rendez-la. Dites ce qu'elle vous apprend et ce qu'elle ne vous apprend pas.

## Solution

Pour cet exercice, j'ai provoqué volontairement un plantage en utilisant un pointeur nul. Le programme essaie d'écrire dans une adresse mémoire qui n'est pas valide.

### Code de [main.cpp](main.cpp)

```cpp
#include <iostream>

int main() {
    std::cout << "Demarrage du programme" << std::endl;

    int* pointeur = nullptr;

    std::cout << "Avant le plantage" << std::endl;

    // pointeur nul
    *pointeur = 42;

    return 0;
}
```

## Construction

* J'ai d'abord nettoyé l'ancien programme :

```bash
jenga clean
```

* Puis j'ai construit le projet :

```bash
jenga build
```

La construction s'est faite avec la configuration **Debug**, pour la cible **Linux x86_64**.


## Exécution

J'ai ensuite lancé le programme avec :

```bash
jenga run
```

Le programme affiche :

```text
Demarrage du programme
Avant le plantage
```

Puis il s'arrête avec le code :

```text
FIN D'EXECUTION — termine avec le code -11
```

Le code `-11` correspond à un signal `SIGSEGV`, c'est-à-dire une erreur de segmentation.

## Recherche dans le journal

Pour retrouver la trace du plantage, j'ai utilisé :

```bash
journalctl -b | grep -i "MaSalle"
```

Le journal contient notamment :

```text
Sep 24 21:07:04 SONWA kernel: MaSalle[949]: segfault at 0 ip 0000617344dd91d7 sp 00007ffdc179fdb0 error 6 in MaSalle[617344dd9000+1000] likely on CPU 14 (core 7, socket 0)

Sep 24 21:07:04 SONWA kernel: MaSalle: MaSalle: potentially unexpected fatal signal 11.

Sep 24 21:07:04 SONWA kernel: CPU: 14 PID: 949 Comm: MaSalle Not tainted 6.6.87.2-microsoft-standard-WSL2 #1

Sep 24 21:07:04 SONWA unknown: WSL (950 - CaptureCrash): Capturing crash for pid: 595, executable: !mnt!d!Users!sonwa!Documents!Art Numerique!AIA-4!Semestre 1!Coder pour la VR, l'AR et l'XR!projets!salle!Build!Bin!Debug-Linux!MaSalle!MaSalle, signal: 11, port: 50005
```

## Ce que la trace m'apprend

La trace permet de savoir que :

* le programme `MaSalle` a subi un **segmentation fault** ;
* le signal reçu est le **signal 11 (`SIGSEGV`)** ;
* le plantage s'est produit à l'adresse mémoire `0` (`segfault at 0`) ;
* le processus concerné avait le PID `949` ;
* le programme tournait sur **WSL2** ;
* le journal indique également le processeur sur lequel le processus était exécuté.

Dans notre cas, cela confirme que le déréférencement du pointeur nul est bien à l'origine du plantage.

## Ce que la trace ne m'apprend pas

La trace ne donne pas directement :

* la ligne exacte du fichier `main.cpp` qui a provoqué le plantage ;
* le nom de la variable `pointeur` ;
* la raison pour laquelle le pointeur était nul ;
* la pile d'appels complète permettant de remonter facilement jusqu'à la ligne fautive.

Pour obtenir ce type d'information, il faudrait utiliser un outil de débogage comme `gdb` et compiler avec les informations de débogage.

## Conclusion

Dans cet exercice, j'ai provoqué volontairement un plantage avec un pointeur nul. Le programme s'est terminé avec le code `-11`, correspondant à un `SIGSEGV`.

Le journal système permet de confirmer le plantage et donne des informations techniques comme le PID, le signal et l'adresse mémoire concernée. Cependant, il ne donne pas directement la ligne de code responsable du problème.
