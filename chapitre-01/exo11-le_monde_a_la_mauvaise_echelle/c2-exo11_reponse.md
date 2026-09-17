 # Exercice 11 : le monde à la mauvaise échelle

 ## Énoncé

Écrivez un programme qui affiche les dimensions d'une salle et de son mobilier, multipliées par un facteur lu à l'entrée.

Faites décrire la salle à trois personnes pour trois facteurs différents, sans leur dire lequel, et notez leurs mots. Rendez les trois descriptions.

# Solution
 J'ai choisi une salle de `4 m x 3 m x 2,5 m`, avec une table, une chaise, une
 porte, une fenêtre et un meuble. Le programme complet se trouve dans
 [c2-exo11.cpp](./c2-exo11.cpp).

 ## Dimensions de départ

 ```text
 Salle   : 4,00 m x 3,00 m x 2,50 m
 Table   : 1,20 m x 0,75 m x 0,75 m
 Chaise  : 0,45 m x 0,45 m x 0,90 m
 Porte   : 0,90 m x 0,10 m x 2,00 m
 Fenêtre : 1,20 m x 0,10 m x 1,00 m
 Meuble  : 1,00 m x 0,45 m x 1,80 m
 ```

 ## Test avec trois personnes

 J'ai fait essayer trois versions de la salle à trois personnes différentes.
 Je ne leur ai pas dit le facteur utilisé. Je leur ai seulement demandé de
 décrire ce qu'elles ressentaient en regardant la salle et le mobilier.

 ### Chance

 Chance a dit :

 > La salle-ci est un peu serrée hein, mais on peut encore circuler. La table et la chaise sont petites, et la porte même paraît basse. On dirait que la pièce a été faite pour quelqu'un qui est plus petit que moi.

 ### Mesmine

 Mesmine a dit :

 > Ici les dimensions sont normales, je ne sens pas vraiment le problème. La table est à la bonne hauteur, la chaise aussi, et la porte ressemble à une porte normale.

 ### Kevin

 Kevin a dit :

 > La salle est grande hein, le plafond est vraiment haut. La table est large et la chaise aussi semble grande.

 ## Facteurs utilisés après le test

 Après avoir recueilli leurs paroles, j'ai regardé les facteurs utilisés :

 - Chance avait la salle avec le facteur `0,8` ;
 - Mesmine avait la salle avec le facteur `1,0` ;
 - Kevin avait la salle avec le facteur `1,2`.

 Les sorties du programme étaient les suivantes.

 ### Facteur 0,8

 ```text
 Salle : 3.20 m x 2.40 m x 2.00 m
 Table : 0.96 m x 0.60 m x 0.60 m
 Chaise : 0.36 m x 0.36 m x 0.72 m
 Porte : 0.72 m x 0.08 m x 1.60 m
 Fenetre : 0.96 m x 0.08 m x 0.80 m
 Meuble : 0.80 m x 0.36 m x 1.44 m
 ```

 ### Facteur 1,0

 ```text
 Salle : 4.00 m x 3.00 m x 2.50 m
 Table : 1.20 m x 0.75 m x 0.75 m
 Chaise : 0.45 m x 0.45 m x 0.90 m
 Porte : 0.90 m x 0.10 m x 2.00 m
 Fenetre : 1.20 m x 0.10 m x 1.00 m
 Meuble : 1.00 m x 0.45 m x 1.80 m
 ```

 ### Facteur 1,2

 ```text
 Salle : 4.80 m x 3.60 m x 3.00 m
 Table : 1.44 m x 0.90 m x 0.90 m
 Chaise : 0.54 m x 0.54 m x 1.08 m
 Porte : 1.08 m x 0.12 m x 2.40 m
 Fenetre : 1.44 m x 0.12 m x 1.20 m
 Meuble : 1.20 m x 0.54 m x 2.16 m
 ```

 ## Conclusion

 Ce test montre que même un petit facteur change beaucoup la sensation dans
 la pièce. Avec `0,8`, la personne peut avoir l'impression d'être grande et
 de voir des meubles trop bas. Avec `1,2`, elle peut se sentir petite parce
 que le plafond, la table et le meuble deviennent trop grands.

 Le facteur `1,0` paraît normal parce que les dimensions correspondent aux
 vraies dimensions choisies au départ. Le problème est que le programme peut
 fonctionner sans afficher d'erreur, même si le monde est mal dimensionné.
