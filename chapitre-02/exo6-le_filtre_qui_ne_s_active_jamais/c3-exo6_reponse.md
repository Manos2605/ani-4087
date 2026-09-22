# Exercice 6
## Énoncé

Écrivez un filtre dont la condition est fausse sur votre machine, avec une définition à l'intérieur, et prouvez qu'elle n'est pas appliquée. Puis rendez la condition vraie et prouvez qu'elle l'est.

Ne comptez pas sur jenga info, et c'est le premier enseignement de l'exercice. Sa sortie est rigoureusement identique, au caractère près, que la condition soit vraie ou fausse : il n'affiche aucune définition. Vérifié dans l'image de correction, Jenga 2.8.0. jenga build --verbose n'en montre pas davantage.

Quand l'outil refuse de répondre, on fait répondre le programme. Arrangez-vous pour que votre main.cpp affiche lui-même s'il a reçu la définition ou non, puis lancez-le dans les deux cas.

## Solution
### Fichier [salle.jenga](salle.jenga)
Pour tester le filtre, j'ai utilisé deux conditions. Ma machine étant sous Windows, la condition `system:Linux` est fausse et la condition `system:Windows` est vraie.

### Fichier [main.jenga](main.cpp)
```cpp
    #ifdef FILTRE_SYSTEM
        std::cout << "Systeme Windows." << std::endl;
    #else
        std::cout << "Systeme Windows non trouvé." << std::endl;
    #endif
```
Le programme vérifie directement si la définition `FILTRE_SYSTEM` a été reçue.

## 1. Condition fausse
Avec le filtre :
```python
with filter("system:Linux"):
    defines(["FILTRE_SYSTEM"])
```

la condition est fausse car ma machine utilise Windows.

### Sortie du programme
```text
Systeme Windows non trouvé.
```

Cela prouve que `FILTRE_SYSTEM` n'a pas été défini.

## 2. Condition vraie

Avec le filtre :

```python
with filter("system:Windows"):
    defines(["FILTRE_SYSTEM"])
```

la condition est vraie sur ma machine.

### Sortie du programme

```text
Systeme Windows.
```

Cela prouve que `FILTRE_SYSTEM` a bien été défini.

## Remarque

`jenga info` ne permet pas de voir si la définition du filtre est appliquée ou non. La vérification doit donc être faite directement dans le programme en testant `FILTRE_SYSTEM`.

## Conclusion
cela permet de comprendre qu'un filtre peut activer une définition seulement lorsque sa condition est vraie. Dans mon cas, le filtre `system:Linux` n'est pas appliqué sur Windows, tandis que le filtre `system:Windows` est appliqué.