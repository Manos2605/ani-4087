 # Exercice 6 : le bras en poses

 ## Énoncé

 Construisez un bras articulé à trois segments : une épaule à l'origine, un coude à un bras de distance, une main à un avant-bras du coude.

Chaque articulation porte sa pose dans le repère de la précédente, et la pose d'une articulation dans le monde s'obtient en composant celle de son parent avec la sienne.

Affichez la position du coude et celle de la main dans l'espace du monde. Faites tourner l'épaule et vérifiez que la main suit.


 ## solution

 J'ai pris des longueurs plus proches des dimensions d'un bras humain pris sur internet :

 ```text
 épaule : position (0, 0, 0)
 bras : 0,30 mètre
 avant-bras : 0,30 mètre
 ```

 Au départ, les trois articulations ne sont pas tournées et le bras est aligné
 avec l'axe `x`. La pose du coude est donc `(0,30, 0)` dans le repère de
 l'épaule. La pose de la main est `(0,30, 0)` dans le repère du coude.

 Le programme complet se trouve dans [c2-exo6.cpp](./c2-exo6.cpp).

 ## Composition dans le monde

 Pour obtenir la pose du coude dans le monde, je compose l'épaule avec le
 coude :

 ```text
 coudeMonde = composer(epaule, coude)
 ```

 Ensuite, pour obtenir la pose de la main, je compose la pose du coude dans le
 monde avec la pose locale de la main :

 ```text
 mainMonde = composer(coudeMonde, main)
 ```

 Au départ, le programme affiche :

 ```text
Coude dans le monde : 0.3000 0.0000 0.0000
Main avant rotation de l'epaule : 0.6000 0.0000 0.0000
 ```

 La main se trouve bien à `0,30 + 0,30 = 0,60` mètre de l'origine.

 ## Rotation de l'épaule

 Ensuite, j'ai tourné l'épaule de 90 degrés autour de l'axe `z`. Je recalcule
 alors le coude et la main en composant les nouvelles poses :

 ```text
Main apres rotation de l'epaule : -0.0000 0.6000 0.0000
 ```

Le bras, qui était dirigé vers `+x`, est maintenant
 dirigé vers `+y`.

 ## Conclusion

 On voit que la main suit bien l'épaule. Au départ, elle est en `(0,60, 0, 0)`
 et après la rotation de l'épaule elle arrive en `(0, 0,60, 0)`. Le coude a
 aussi bougé avec l'épaule, même si sa pose locale n'a pas changé.
