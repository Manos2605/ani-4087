# Exercice 4 : l'inverse d'une pose

## Énoncé

Ici, il fallait écrire `Inverser(pose)` à la main. Il fallait prendre le
conjugué du quaternion, puis calculer la position opposée tournée avec ce
conjugué.

Après ça, il fallait vérifier le résultat en appliquant la pose à un point,
puis la pose inverse au résultat. L'écart avec le point de départ devait être
nul, sauf les petites erreurs d'arrondi de la machine.
## Comment j'ai fait
Dans le cours, une pose contient seulement une position en mètres et une
orientation. Elle n'a pas d'échelle, parce qu'un casque ne redimensionne pas
la tête de la personne. La pose commence par tourner le point et ajoute
ensuite la translation :
```text
résultat = R(point) + position
```
Comme le quaternion est normalisé, l'inverse de la rotation est son conjugué.
Pour un quaternion `q = (w, x, y, z)`, le conjugué devient :
```text
q* = (w, -x, -y, -z)
```
Pour trouver la position inverse, je ne peux pas seulement changer les signes
de la position et c'est fini. Je prends d'abord la position opposée, puis je
la tourne avec le quaternion conjugué :
```text
positionInverse = tourner(q*, -position)
```
Le programme complet se trouve dans [c2-exo4.cpp](./c2-exo4.cpp).

Cette inversion est utile pour obtenir la vue de la caméra. La pose de l'oeil
dit où se trouve l'oeil dans le monde, mais pour dessiner la scène il faut
plutôt exprimer le monde par rapport à l'oeil. C'est pour cela qu'on utilise
l'inverse de la pose. Ici, je l'écris directement avec le conjugué, au lieu
de demander une inversion générale de matrice.
## Vérification

Pour tester, j'ai pris :

```text
position = (1, 2, 3)
Angle de rotation en degres : 90
Axe de rotation (x y z) : 0 0 1
donc on aura quaternion = (sqrt(2)/2, 0, 0, sqrt(2)/2)
point = (4, 5, 6)
```

Le quaternion correspond à une rotation de 90 degrés autour de l'axe `z`.
Je garde aussi la convention du module, la même que celle d'OpenXR : repère
à main droite, `+y` vers le haut, `+x` vers la droite et l'avant vers les `z`
négatifs.
Dans le programme, je fais les trois étapes suivantes :

1. j'applique la pose au point;
2. j'applique la pose inverse au résultat obtenu;
3. je compare le nouveau point avec le point de départ.

Le résultat du test donne ceci :

```text
Point transforme : -4 6 9
Apres application de l'inverse : 4 5 6
Ecart avec le point de depart : 2.66454e-15 2.66454e-15 0
Norme de l'ecart : 3.76822e-15
```

## Conclusion

Le test confirme que mon inverse fonctionne bien. Après avoir transformé le
point, j'applique l'inverse et je retrouve exactement `(4, 5, 6)`. La norme
de l'écart donne des valeurs très proche de `0`, donc le point revient à sa place, aux arrondis près.

En gros, l'inverse annule d'abord la translation avec la position opposée,
puis il annule la rotation avec le conjugué du quaternion. Il ne fallait donc
pas juste mettre un signe moins devant `x`, `y` et `z` de la position. Il
fallait encore tourner cette position avec le bon quaternion.

Ce que je retiens surtout du cours, c'est qu'une erreur de repère ne fait pas
forcément planter le programme. Si l'inverse est mal calculé, l'image peut
quand même s'afficher, mais la caméra ou le monde ne seront pas au bon endroit.
Le test aller-retour permet donc de vérifier concrètement que la pose inverse
fait bien son travail. Si l'écart devient grand, c'est qu'il y a un problème
dans l'ordre des opérations, dans le conjugué ou dans la convention du repère.