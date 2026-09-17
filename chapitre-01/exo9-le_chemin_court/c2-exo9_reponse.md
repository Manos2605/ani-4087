 # Exercice 9 : le chemin court

 ## Énoncé

 Écrivez la vitesse angulaire moyenne entre deux orientations séparées de dt, avec le forçage du chemin court.

Puis retirez le forçage et trouvez deux quaternions pour lesquels le résultat devient absurde. Rendez les deux valeurs, avec et sans.

 Le programme complet se trouve dans [c2-exo9.cpp](./c2-exo9.cpp).

 ## Solution

 Je calcule d'abord la rotation qui permet de passer de l'orientation de
 départ à l'orientation d'arrivée :

 ```text
 difference = conjugue(depart) * arrivee
 ```

 Si la partie vectorielle de `difference` est `(x, y, z)` et que sa longueur
 vaut `s`, l'angle de rotation est :

 ```text
 angle = 2 * atan2(s, difference.w)
 ```

 Ensuite, la vitesse angulaire moyenne est l'axe de rotation multiplié par
 `angle / dt`. Dans le code, cela donne directement :

 ```text
 vitesse = (x, y, z) * angle / (dt * s)
 ```

 Pour forcer le chemin court, je calcule le produit scalaire entre les deux
 quaternions. Si ce produit est négatif, je change le signe du quaternion
 d'arrivée :

 ```text
 si dot(depart, arrivee) < 0 :
	 arrivee = -arrivee
 ```

 Un quaternion et son opposé représentent exactement la même orientation.
 Changer le signe ne change donc pas la pose, mais cela permet de choisir la
 rotation la plus courte entre les deux orientations.

 ## Test utilisé

 J'ai choisi une pose de départ qui ne tourne pas et une arrivée représentant
 une rotation de 10 degrés autour de `z`. Mais j'ai écrit le quaternion
 d'arrivée avec le signe opposé :

 ```text
 depart  = (1, 0, 0, 0)
 arrivee = (-cos(5 degres), 0, 0, -sin(5 degres))
 dt      = 1 seconde
 ```

 Les deux quaternions `q` et `-q` représentent la même rotation. Donc, en
 réalité, le mouvement entre les deux orientations est seulement de 10 degrés.

 ## Résultats

 Avec le forçage du chemin court, le programme affiche :

 ```text
 Vitesse avec chemin court : -0.000000 0.000000 0.174533
 ```

 La vitesse est donc environ `0,174533 rad/s` autour de `z`. C'est logique,
 car 10 degrés correspondent à environ `0,174533` radians et `dt` vaut une
 seconde.

 Sans le forçage du chemin court, on obtient :

 ```text
 Vitesse sans chemin court : 0.000000 0.000000 -6.108652
 ```

 Cette valeur est absurde pour ce cas : le programme croit que l'orientation
 doit tourner de presque 350 degrés dans l'autre sens, au lieu de faire les
 10 degrés les plus courts. La vitesse est donc beaucoup trop grande et le
 signe de la rotation est inversé.

 ## Conclusion

 Le chemin court est important parce que deux quaternions opposés décrivent la
 même orientation. Si on ne regarde pas leur produit scalaire, le programme
 peut choisir un grand tour alors qu'un petit mouvement suffit. Dans un casque
 de réalité virtuelle, cela peut créer une rotation bizarre et provoquer une
 mauvaise sensation.

 Donc, avant de calculer l'angle et la vitesse, je vérifie le signe du produit
 scalaire. Si les quaternions sont du mauvais côté, je change le signe de celui
 d'arrivée. Comme ça, le calcul prend le petit chemin et la vitesse obtenue
 reste cohérente avec le mouvement réel.
