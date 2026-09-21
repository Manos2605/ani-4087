# Exercice 2
## Enoncer
Lancez jenga info sur votre projet et lisez sa sortie en entier. Rendez-la, et dites ce qu'elle vous apprend que le fichier de projet ne disait pas explicitement.

## Solution
J'ai gardé le même projet qu'a l'exercice 1 et je lance :
```text
jenga info
```

et la sortie obtenue est :
![Terminal](image.png)

## Conclusion et remarque

La commande `jenga info` confirme que le workspace s'appelle `salle` et que Jenga utilise bien le fichier `salle.jenga`. Elle indique également que deux configurations sont disponibles : `Debug` et `Release`.

La sortie apporte surtout des informations qui n'étaient pas écrites directement dans le fichier de projet :

* trois toolchains sont disponibles : `host-gcc`, `msvc` et `mingw` ;
* la cible `MaSalle` est de type `ConsoleApp` ;
* `MaSalle` n'est pas une suite de tests ni un projet externe ;
* le daemon Jenga n'est pas lancé.

mais aussi les deux lignes :

```text
Platforms: Windows
Target OSes: Windows, Linux, macOS
```

`Target OSes` vient du fichier `salle.jenga`, car les systèmes Windows, Linux et macOS y sont déclarés avec `targetoses([TargetOS.WINDOWS, TargetOS.LINUX, TargetOS.MACOS]])`.

En revanche, `Platforms: Windows` correspond à la plateforme de la machine sur laquelle Jenga est actuellement exécuté. Elle indique donc que mon environnement actuel est Windows.

Ainsi, `jenga info` ne fait pas seulement afficher le contenu du fichier de projet : il donne aussi des informations sur l'environnement détecté par Jenga, notamment les toolchains disponibles, la plateforme actuelle et l'état du daemon.

