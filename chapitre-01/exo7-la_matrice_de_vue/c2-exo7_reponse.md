 # Exercice 7 : la matrice de vue

 ## Énoncé

 Dans cet exercice, j'ai écrit deux façons de calculer l'inverse d'une pose :

 - une inversion générale de la matrice 4x4 ;
 - une inversion directe avec le conjugué du quaternion et la translation
	 opposée tournée par ce conjugué.

 Le programme complet se trouve dans [c2-exo7.cpp](./c2-exo7.cpp).

 ## Les deux méthodes

 Pour une pose de position `t` et de rotation `R`, transformer un point se
 fait comme ceci :

 ```text
 pointMonde = R * pointLocal + t
 ```

 La matrice homogène de cette pose est :

 ```text
  R  t 
  0  1 
 ```

 ### L'inverse direct
 Pour comprendre la formule, je pars de l'équation de la pose et je cherche le
 point local à partir du point monde :

 ```text
 pointMonde = R * pointLocal + t
 pointMonde - t = R * pointLocal
 R^-1 * (pointMonde - t) = pointLocal
 ```

 Comme `R` est une rotation, son inverse est sa transposée : `R^-1 = R^T`.
 On obtient donc :

 ```text
 pointLocal = R^T * pointMonde - R^T * t
 ```

 C'est pour cela que la matrice inverse directe est :

 ```text
  R^T  -R^T * t 
   0       1     
 ```

 Il faut faire attention à la translation inverse. Ce n'est pas seulement
 `-t`. Il faut d'abord prendre la position opposée, puis la tourner avec la
 rotation inverse :

 ```text
 positionInverse = tourner(rotationInverse, -position)
 ```

 Avec un quaternion unitaire `q = (w, x, y, z)`, la rotation inverse est
 donnée par le conjugué :

 ```text
 q* = (w, -x, -y, -z)
 ```

 Donc, dans le programme, l'inversion directe se fait en trois étapes :

 1. construire le conjugué du quaternion ;
 2. opposer la position de la pose ;
 3. tourner cette position opposée avec le conjugué.

 ## Comparaison

 J'ai testé une pose avec une translation `(1, 2, 3)` et une rotation de 35
 degrés autour de `z`. Les deux matrices inverses obtenues sont les mêmes :

 ```text 
Inverse generale :
            0.8192            0.5736            0.0000           -1.9663
           -0.5736            0.8192            0.0000           -1.0647
            0.0000            0.0000            1.0000           -3.0000
            0.0000            0.0000            0.0000            1.0000
            
Inverse directe :
            0.8192            0.5736            0.0000           -1.9663
           -0.5736            0.8192            0.0000           -1.0647
            0.0000            0.0000            1.0000           -3.0000
            0.0000            0.0000            0.0000            1.0000
 ```

 Le programme compare bien les 16 coefficients, et il affiche :

 ```text
Ecart maximum entre les 16 coefficients : 0.0000
 ```

 Donc, pour une pose normale, l'inversion générale et l'inversion directe
 donnent le même résultat.

 ## Test

 Pour le deuxième test, j'ai donné à l'inversion générale une matrice
 singulière : ses deux premières lignes sont identiques. Elle n'a donc pas
 d'inverse mathématique.

 Le programme affiche :

 ```text
Pose degeneree inversible : non
Resultat rendu pour la pose degeneree :
            1.0000            0.0000            0.0000            0.0000
            0.0000            1.0000            0.0000            0.0000
            0.0000            0.0000            1.0000            0.0000
            0.0000            0.0000            0.0000            1.0000
 ```

 ## Conclusion

 Les deux méthodes sont d'accord sur une pose valide, coefficient par
 coefficient. Par contre, l'inversion générale ne peut pas inverser une
 matrice dégénérée. Dans mon programme, elle indique que la matrice n'est pas
 inversible et rend l'identité comme valeur.
