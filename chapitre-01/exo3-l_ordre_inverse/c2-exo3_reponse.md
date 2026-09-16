# Exercice 3 : l'ordre inverse

## Énoncé

Reprenez l'exercice précédent et écrivez une seconde fonction qui applique la translation d'abord et la rotation ensuite.

Affichez les deux résultats pour le même point. Puis trouvez une pose et un point pour lesquels les deux coïncident, et dites pourquoi.

## Code

J'ai gardé la convention `q = (w, x, y, z)` de l'exercice précédent.
Dans la première fonction, on fait la rotation avant d'ajouter la translation.
Dans la deuxième, on ajoute d'abord la translation et on fait la rotation
après. Le programme complet est dans [c2-exo3.cpp](./c2-exo3.cpp).

## Exemple

Pour tester, j'ai choisi la pose et le point suivants :
```text
pose.position = (0, 0, 2)
pose.rotation = (sqrt(2)/2, 0, 0, sqrt(2)/2)
point = (1, 0, 0)
```
Le quaternion représente une rotation de 90 degrés autour de l'axe `z`.

Quand je fais la rotation d'abord, puis la translation :
```text
R(point) + t = (0, 1, 0) + (0, 0, 2) = (0, 1, 2)
```
Quand je fais la translation d'abord, puis la rotation :

```text
R(point + t) = R(1, 0, 2) = (0, 1, 2)
```

On trouve donc `(0, 1, 2)` dans les deux cas. Les résultats coïncident bien.

## Conclusion

Normalement, l'ordre des opérations change le résultat. Mais ici, ça tombe
bien parce que la translation `(0, 0, 2)` est parallèle à l'axe de rotation
`z`. Quand on tourne ce vecteur autour de cet axe, il ne bouge pas : `R(t) = t`.

Plus généralement :
```text
R(point) + t = R(point + t) = R(point) + R(t)
```
Donc, si `R(t) = t`, les deux façons de faire donnent le même résultat.
Dans ce cas précis, le point choisi n'a pas vraiment d'importance : on aura
le même résultat pour tous les points avec cette pose.
