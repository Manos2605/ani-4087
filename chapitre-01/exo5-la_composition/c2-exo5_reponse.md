 # Exercice 5 : la composition de deux poses

## Énoncé
Écrivez la composition de deux poses, puis vérifiez sur un cas que composer puis appliquer donne le même résultat qu'appliquer l'une après l'autre.

Affichez les deux points obtenus et leur écart.

 ## Solution
 Une pose applique d'abord la rotation et ensuite la translation :

 ```text
 appliquer(pose, point) = tourner(rotation, point) + position
 ```

 J'ai appelé `parent` la pose extérieure et `enfant` la pose intérieure.
 Pour composer les deux, j'applique donc d'abord la pose enfant, puis la pose
 parent :

 ```text
 composee = parent * enfant
 ```

 La rotation composée est le produit des deux quaternions, dans cet ordre :

 ```text
 rotationComposee = rotationParent * rotationEnfant
 ```

 Pour la position, je tourne d'abord la position de l'enfant avec la rotation
 du parent, puis j'ajoute la position du parent :

 ```text
positionComposee = tourner(rotationParent positionEnfant) + positionParent
 ```

 Le programme complet se trouve dans [c2-exo5.cpp](./c2-exo5.cpp).

 ## Test
 Pour faire le test, j'ai choisi :

 ```text
 parent : position (1, 2, 0), rotation de 90 degrés autour de z
 enfant : position (2, 0, 0), rotation de 90 degrés autour de z
 point  : (1, 0, 0)
 ```

 Je calcule le premier résultat en appliquant directement la pose composée.
 Ensuite, je calcule le deuxième en appliquant d'abord la pose enfant, puis la
 pose parent.

 Le programme affiche :

 ```text
Point avec la pose composee : -1.33227e-15 4 0
Point avec les poses enchainees : -8.88178e-16 4 0
Ecart : -4.44089e-16 0 0
Norme de l'ecart : 4.44089e-16
 ```

 ## Conclusion

 On voit que les deux points obtenus sont pareils. La pose composée donne le même résultat que les deux poses appliquées l'une après l'autre. L'écart est presque nul, à part les petites erreurs d'arrondi possibles avec les nombres réels.
