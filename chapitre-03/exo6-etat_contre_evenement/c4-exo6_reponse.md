# Exercice 6 : État contre événement

## Énoncé

> Écrivez deux compteurs. Le premier s'incrémente à chaque image où la touche Espace est tenue, lu par `NkInput.IsKeyDown`. Le second s'incrémente à chaque `NkKeyPressEvent` sur Espace.
>
> Appuyez une seconde, relâchez. Rendez les deux nombres et expliquez l'écart.

## Solution

J'ai repris la boucle principale des exercices précédents. J'ai ajouté deux compteurs :

* le premier compte le nombre de fois où la touche **Espace** est détectée comme étant maintenue ;
* le deuxième compte le nombre d'événements `NkKeyPressEvent` reçus pour la touche **Espace**.

### Modifications apportées au fichier [main.cpp](main.cpp)

Pour utiliser `NkInput.IsKeyDown`, j'ai ajouté l'inclusion de `NkEventDispatcher.h`.

J'ai également ajouté :

```cpp
int compteurEtat = 0;
int compteurEvenement = 0;
```

Dans le callback `NkKeyPressEvent`, j'ai ajouté le compteur des événements lorsque la touche Espace est pressée :

```cpp
if (event->GetKey() == nkentseu::NkKey::NK_SPACE){
    compteurEvenement++;
}
```

Dans la boucle principale, j'ai ajouté la vérification de l'état de la touche :

```cpp
if (nkentseu::NkInput.IsKeyDown(nkentseu::NkKey::NK_SPACE)){
    compteurEtat++;
}
```

J'ai aussi ajouté `NkInitialise()` avant la création de la fenêtre afin d'initialiser correctement le système d'événements utilisé par `NkInput`.

Enfin, j'ai ajouté une pause de `10 ms` à chaque passage de la boucle :

```cpp
nkentseu::NkClock::Sleep((nkentseu::int64)10);
```

Cela permet d'éviter que la boucle s'exécute continuellement sans pause.

### Test

J'ai lancé le programme puis j'ai maintenu la touche **Espace pendant environ une seconde** avant de la relâcher.

Résultat obtenu :

```text
Compteur avec IsKeyDown : 219
Compteur avec NkKeyPressEvent : 4
```

### Observation

J'ai constaté que les deux compteurs ne mesurent pas la même chose.

Le compteur basé sur `NkInput.IsKeyDown` augmente à chaque vérification où la touche Espace est maintenue. Il peut donc augmenter plusieurs fois pendant une seule pression maintenue.

Le compteur basé sur `NkKeyPressEvent` compte les événements de pression reçus. Il ne correspond donc pas directement au nombre de fois où la boucle vérifie que la touche est maintenue.

### Conclusion

Cela montre la différence entre **un état** et **un événement**.

`IsKeyDown` permet de savoir si la touche est actuellement maintenue. Comme le test est effectué régulièrement dans la boucle, le compteur peut augmenter de nombreuses fois pendant que la touche reste enfoncée.

À l'inverse, `NkKeyPressEvent` correspond à une pression détectée par le système d'événements. Les deux compteurs peuvent donc avoir des valeurs très différentes même lorsque la touche Espace est maintenue pendant environ une seconde.
