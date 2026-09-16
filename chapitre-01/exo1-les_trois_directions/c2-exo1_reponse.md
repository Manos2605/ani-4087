# Exercice 1 : les trois directions

# Enoncer
Écrivez les trois fonctions qui fixent la convention une fois pour toutes : Avant(), Haut() et Droite(), qui rendent chacune un vecteur unitaire dans la convention du module.

Votre programme lit trois réels, les traite comme un point, et affiche son produit scalaire avec chacune des trois directions. Trois lignes, quatre décimales.

## Convention
Dans le cours il est dit que `x`vers la droite `y` vers le haut et `z` vers l'arrière. L'avant est donc la direction `-z`.

Les trois fonctions qui fixent cette convention sont dans le fichier
[c2-exo1.cpp](./c2-exo1.cpp).


Elles renvoient :
- `Avant()` : `(0, 0, -1)`;
- `Haut()` : `(0, 1, 0)` ;
- `Droite()` : `(1, 0, 0)`.

## Conclusion

Pour un point `(x, y, z)`, les trois lignes affichées sont respectivement :

```text
-z
y
x
```
