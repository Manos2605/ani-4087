**# Exercice 7 : Le pointeur caché**

**## Énoncé**

> Cachez le curseur et confinez-le. Affichez à chaque image la position `x`, `y` et le `rawDelta`.
>
> Bougez la souris jusqu'à ce qu'elle atteigne le bord. Rendez les deux séries et dites laquelle continue de bouger, et pourquoi c'est celle-là qu'il faut.

**## Solution**

J'ai repris la boucle principale des exercices précédents. J'ai ajouté la gestion de la souris afin de cacher le curseur, de le confiner dans la fenêtre et d'afficher sa position ainsi que son déplacement.

J'ai également utilisé `GetDeltaX()` et `GetDeltaY()` pour récupérer le déplacement relatif de la souris.

**### Modifications apportées au fichier [main.cpp](main.cpp)**

Pour utiliser les événements de la souris, j'ai ajouté l'inclusion de `NkMouseEvent.h` :

```cpp
#include <NKEvent/NkMouseEvent.h>
```

J'ai également ajouté :

```cpp
fenetre.HideCursor();
fenetre.ConfineCursor(true);
```

La première instruction cache le curseur et la deuxième le confine à la fenêtre.

J'ai ensuite ajouté un callback sur `NkMouseMoveEvent` pour afficher la position et le déplacement de la souris :

```cpp
nkentseu::NkEvents().AddEventCallback<nkentseu::NkMouseMoveEvent>(
    [&](nkentseu::NkMouseMoveEvent* event){
        std::cout
            << "x = " << event->GetX()
            << " | y = " << event->GetY()
            << " | rawDeltaX = " << event->GetDeltaX()
            << " | rawDeltaY = " << event->GetDeltaY()
            << std::endl;
    });
```

`GetX()` et `GetY()` permettent d'obtenir la position de la souris dans la fenêtre.

`GetDeltaX()` et `GetDeltaY()` permettent d'obtenir le déplacement de la souris.

**### Test**

J'ai lancé le programme puis j'ai déplacé la souris jusqu'au bord de la fenêtre.

Les valeurs affichées ressemblent à ceci :

```text
x = 1275 | y = 363 | rawDeltaX = 3 | rawDeltaY = 0
x = 1276 | y = 363 | rawDeltaX = 1 | rawDeltaY = 0
x = 1277 | y = 704 | rawDeltaX = 1 | rawDeltaY = 341
x = 1277 | y = 705 | rawDeltaX = 0 | rawDeltaY = 1
x = 1276 | y = 705 | rawDeltaX = -1 | rawDeltaY = 0
x = 1275 | y = 706 | rawDeltaX = -1 | rawDeltaY = 1
x = 1275 | y = 707 | rawDeltaX = 0 | rawDeltaY = 1
x = 1274 | y = 707 | rawDeltaX = -1 | rawDeltaY = 0
x = 1273 | y = 707 | rawDeltaX = -1 | rawDeltaY = 0
x = 1272 | y = 707 | rawDeltaX = -1 | rawDeltaY = 0
x = 1272 | y = 706 | rawDeltaX = 0 | rawDeltaY = -1
x = 1271 | y = 706 | rawDeltaX = -1 | rawDeltaY = 0
```

Lorsque la souris arrive au bord, la position `x` ou `y` ne peut plus continuer dans la même direction.

**### Observation**

J'ai constaté que la position `x` et `y` est limitée par les dimensions de la fenêtre.

Lorsque le curseur atteint le bord, sa position ne peut donc plus augmenter dans cette direction.

En revanche, le `rawDelta`, qui correspond au déplacement de la souris, continue de donner une valeur lorsque je continue à déplacer physiquement la souris.

Les deux séries ne représentent donc pas la même information :

* `x` et `y` représentent la position de la souris dans la fenêtre ;
* `rawDeltaX` et `rawDeltaY` représentent le déplacement de la souris.

**### Conclusion**

C'est la série du **`rawDelta`** qu'il faut utiliser pour détecter le mouvement de la souris lorsque le curseur est caché et confiné.

La position `x/y` atteint une limite lorsque la souris arrive au bord de la fenêtre. Le `rawDelta`, lui, permet de récupérer le mouvement relatif de la souris même lorsque sa position ne peut plus continuer dans cette direction.

D'ou le `rawDelta` est adapté pour contrôler par exemple une caméra ou effectuer un déplacement continu avec une souris confinée.
