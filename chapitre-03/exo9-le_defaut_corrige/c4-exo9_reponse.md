# Exercice 9 : Le défaut corrigé

## Énoncé

> Écrivez l'accumulateur : un rappel sur `NkMouseRawEvent` qui ajoute, et une consommation par image qui prend le total et remet à zéro.
>
> Reprenez la mesure de l'exercice précédent. Rendez les deux séries côte à côte.

## Solution

J'ai repris le programme de l'exercice 8 et ajouté un accumulateur pour `NkMouseRawEvent`.

* `rawDeltaXAccumule` additionne les valeurs de `GetDeltaX()` reçues par `NkMouseRawEvent`.
* À chaque image, la valeur accumulée est affichée puis remise à `0`.
* `deltaXPrecedent` reprend la mesure de l'exercice précédent avec `NkMouseMoveEvent`.

### Modifications apportées au fichier [`main.cpp`](main.cpp)

```cpp
int rawDeltaXAccumule = 0;
int deltaXPrecedent = 0;
```

L'accumulation est faite dans le rappel :

```cpp
nkentseu::NkEvents().AddEventCallback<nkentseu::NkMouseRawEvent>(
    [&](nkentseu::NkMouseRawEvent* event){
        rawDeltaXAccumule += event->GetDeltaX();
    });
```

Puis la valeur est consommée dans la boucle principale :

```cpp
std::cout
    << "accumule = " << rawDeltaXAccumule
    << " | precedent = " << deltaXPrecedent
    << std::endl;

rawDeltaXAccumule = 0;
```

La mesure de l'exercice précédent est conservée avec :

```cpp
nkentseu::NkEvents().AddEventCallback<nkentseu::NkMouseMoveEvent>(
    [&](nkentseu::NkMouseMoveEvent* event){
        deltaXPrecedent = event->GetDeltaX();
    });
```

## Test

En bougeant la souris, j'obtiens par exemple :

```text
accumule = -10 | precedent = -12
accumule = 0 | precedent = -11
accumule = 0 | precedent = -11
...
accumule = -11 | precedent = -11
accumule = 0 | precedent = -12
...
accumule = -12 | precedent = -12
accumule = 0 | precedent = -12
...
accumule = -12 | precedent = -14
accumule = 0 | precedent = -13
...
```

## Observation

La série `accumule` reçoit les déplacements bruts et est remise à `0` après chaque image.

La série `precedent` conserve la dernière valeur reçue par `NkMouseMoveEvent` jusqu'à l'arrivée d'un nouvel événement.

Les deux valeurs peuvent donc être différentes.

## Conclusion

L'accumulateur permet de récupérer plusieurs `NkMouseRawEvent` et d'en faire un total par image. Après sa consommation, le total est remis à zéro, ce qui corrige le défaut de l'exercice précédent.
