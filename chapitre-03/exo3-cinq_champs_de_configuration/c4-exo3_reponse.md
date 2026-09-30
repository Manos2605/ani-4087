# Exercice 3 : Cinq champs de configuration

## Énoncé

> Modifiez cinq champs de `NkWindowConfig` que le chapitre n'a pas montrés, choisis dans `NkWindowConfig.h`.
>
> Pour chacun, rendez la ligne, ce que vous attendiez, et ce que vous avez observé. Un champ qui n'a rien changé est une réponse valable, à condition de dire pourquoi vous le pensez.

## Solution
Pour cet exercice, j'ai choisi cinq champs différents de `NkWindowConfig` qui se trouve dans `KitNkentseu\include\NKWindow\Core` afin d'observer leur influence sur la fenêtre. J'ai testé chaque champ séparément.

### 1. Position : `centered`

```cpp
bool centered = false;
```

**Attendu :** Je m'attends à ce que la fenêtre ne soit pas automatiquement placée au centre de l'écran.

**Observé :** La fenêtre ne s'est plus affichée exactement au centre de l'écran. Elle a été positionnée différemment par rapport à sa position habituelle.

### 2. Position horizontale : `x`

```cpp
int32 x = 500;
```

**Attendu :** Je m'attends à ce que la fenêtre soit positionnée plus à droite sur l'écran.

****Observé :** Au début, j'ai essayé plusieurs valeurs pour `x`, même des valeurs beaucoup plus grandes, mais la fenêtre ne changeait pas de position. J'ai d'abord pensé qu'il y avait un problème avec le champ `x`. Ensuite, en relançant le programme, j'ai oublié de remettre `bool centered = true` et j'ai remarqué que la fenêtre avait changé de position. J'ai donc compris que lorsque `centered` est à `true`, la fenêtre est centrée automatiquement et la valeur de `x` n'est pas prise en compte pour son positionnement initial. En mettant `bool centered = false`, la valeur de `x` est bien prise en compte et la fenêtre se déplace.


### 3. Redimensionnement : `resizable`

```cpp
bool resizable = true;
```

**Attendu :** Je m'attends à ce que la taille de la fenêtre ne puisse plus être modifiée manuellement.

**Observé :** Je ne pouvais plus redimensionner la fenêtre avec la souris. Sa taille restait fixe même en essayant de déplacer ses bordures.

### 4. Couleur de fond : `bgColor`

```cpp
uint32 bgColor = 0xFF0000FF;
```

**Attendu :** Je m'attends à ce que la couleur de fond de la fenêtre soit modifiée.

**Observé :** La couleur de fond de la fenêtre a changé par rapport à la couleur utilisée au départ. Le champ `bgColor` permet donc bien de modifier la couleur de la fenêtre.

### 5. Opacité : `opacity`

```cpp
float32 opacity = 0.5f;
```

**Attendu :** Je m'attends à ce que la fenêtre devienne partiellement transparente.

**Observé :** La fenêtre est devenue plus transparente qu'avant. On pouvait voir en partie ce qui se trouvait derrière la fenêtre c'est à dire mon editeur de code.

## Conclusion

Ces cinq tests m'ont permis de voir que les différents champs de `NkWindowConfig` permettent de modifier plusieurs caractéristiques de la fenêtre, comme sa position, sa taille et son apparence.

J'ai testé chaque champ séparément afin de voir plus facilement l'effet de chaque modification.
