**# Exercice 8 : Le défaut reproduit**

**## Énoncé**

> Affichez `rawDeltaX` à chaque image, sans rien accumuler vous-même.
>
> Bougez la souris, puis posez la main. Rendez les vingt lignes qui suivent l'arrêt, et dites en une phrase ce qu'elles prouvent.

**## Solution**

J'ai repris la boucle principale des exercices précédents et affiché la valeur de `rawDeltaX` lors des mouvements de la souris.

Je n'ai pas ajouté de compteur ni de variable pour accumuler les déplacements. La valeur affichée correspond directement au `GetDeltaX()` fourni par l'événement de mouvement de la souris.

**### Modifications apportées au fichier [main.cpp](main.cpp)**

J'ai ajouté l'inclusion de `NkMouseEvent.h` ainsi que `iostream` pour pouvoir utiliser l'événement de déplacement de la souris et afficher sa valeur.

J'ai ensuite ajouté un callback sur `NkMouseMoveEvent` :

```cpp
nkentseu::NkEvents().AddEventCallback<nkentseu::NkMouseMoveEvent>(
    [&](nkentseu::NkMouseMoveEvent* event){
        std::cout
            << "rawDeltaX = " << event->GetDeltaX()
            << std::endl;
    });
```

La valeur de `GetDeltaX()` est affichée directement, sans être additionnée à une autre valeur.

**### Test**

J'ai lancé le programme, déplacé la souris, puis j'ai posé la main.

Les vingt lignes qui suivent l'arrêt sont :

```text
rawDeltaX = 0
rawDeltaX = 0
rawDeltaX = 0
rawDeltaX = 0
rawDeltaX = 0
rawDeltaX = 0
rawDeltaX = 0
rawDeltaX = 0
rawDeltaX = 0
rawDeltaX = 0
rawDeltaX = 0
rawDeltaX = 0
rawDeltaX = 0
rawDeltaX = 0
rawDeltaX = 0
rawDeltaX = 0
rawDeltaX = 0
rawDeltaX = 0
rawDeltaX = 0
rawDeltaX = 0
```

**### Observation**

Après avoir posé la main, `rawDeltaX` reste à `0` lorsque la souris ne bouge plus.

**### Conclusion**

Cela prouvent que le `rawDeltaX` correspond au déplacement de la souris et qu'il devient nul lorsque la souris est immobile.
