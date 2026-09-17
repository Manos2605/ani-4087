# Exercice 8 : l'extrapolation d'une pose

## Énoncé
Écrivez la fonction qui avance une pose de dt secondes à vitesses constantes, linéaire et angulaire.

Votre programme lit une pose, ses deux vitesses et une durée, et affiche la pose extrapolée. Traitez le cas d'une vitesse angulaire nulle sans diviser par zéro.

Le programme complet se trouve dans [c2-exo8.cpp](./c2-exo8.cpp).

## Comment j'ai fait

Pour la position, le calcul est direct :

 ```text
 nouvellePosition = position + vitesseLineaire * dt
 ```

La vitesse angulaire est donnée comme un vecteur en radians par seconde. Sa
norme donne la vitesse de rotation et sa direction donne l'axe :

 ```text
 vitesseRotation = longueur(vitesseAngulaire)
 axe = vitesseAngulaire / vitesseRotation
 angle = vitesseRotation * dt
 ```

Avec cet angle et cet axe, je construis un quaternion de rotation
supplémentaire. Je le compose ensuite avec l'orientation actuelle de la pose :

 ```text
 nouvelleRotation = rotation * rotationSupplementaire
 ```

Dans le programme, l'angle de la pose initiale est saisi en degrés et converti
en radians pour construire son quaternion. La vitesse angulaire, elle, est
saisie directement en radians par seconde.

## Le cas où ça ne tourne pas

Il ne faut pas diviser par la norme de la vitesse angulaire si cette norme
vaut zéro. Dans ce cas, je laisse simplement la rotation comme elle était :

 ```text
 si vitesseRotation == 0 :
	 nouvelleRotation = rotation
```

La position continue quand même d'avancer avec la vitesse linéaire. Comme ça,
le programme ne fait pas de division par zéro et il traite correctement le cas
où la pose se déplace sans tourner.

## Résultats

J'ai repris les valeurs de l'exemple et je les ai entrées dans le programme.
Pour le premier test, j'ai saisi :

```text
Position de la pose (x y z) : -1 0 2
Quaternion unitaire de la pose (w x y z) : 0.1221 0.4517 -0.4945 -0.7325
Vitesse lineaire (m/s, x y z) : 1 1 2
Vitesse angulaire (rad/s, x y z) : 0 0.2 0.56
Duree dt en secondes : 2
```

Le programme donne réellement :

```text
Position extrapolee : 1.0000 2.0000 6.0000
Quaternion extrapole : 0.5808 0.2513 -0.6249 -0.4572
```

La position est bien calculée avec `position + vitesse * dt`, donc on obtient
`(-1, 0, 2) + (1, 1, 2) * 2 = (1, 2, 6)`.

Avec une vitesse angulaire nulle, j'ai saisi :

```text
Position de la pose (x y z) : -1 0 2
Quaternion unitaire de la pose (w x y z) : 0.1221 0.4517 -0.4945 -0.7325
Vitesse lineaire (m/s, x y z) : 0 0.2 0.56
Vitesse angulaire (rad/s, x y z) : 0 0 0
Duree dt en secondes : 2
```

La sortie réelle est :

```text
Position extrapolee : -1.0000 0.4000 3.1200
Quaternion extrapole : 0.1221 0.4517 -0.4945 -0.7325
```

Ici, le quaternion reste exactement pareil. Donc on voit bien que le
programme conserve l'orientation quand la vitesse angulaire vaut zéro, sans
faire de division par zéro.

Enfin, j'ai testé une durée plus longue, `dt = 20` secondes :

```text
Position de la pose (x y z) : -1 0 2
Quaternion unitaire de la pose (w x y z) : 0.1221 0.4517 -0.4945 -0.7325
Vitesse lineaire (m/s, x y z) : 0 0.2 0.56
Vitesse angulaire (rad/s, x y z) : 1 1 2
Duree dt en secondes : 20
```

Le programme donne :

```text
Position extrapolee : -1.0000 4.0000 13.2000
Quaternion extrapole : -0.2684 0.3956 -0.0293 -0.8778
```

La position continue d'avancer selon la vitesse constante, mais il est peu
réaliste de supposer que le corps garde exactement le même mouvement pendant
20 secondes. À ce moment-là, le modèle commence à raconter n'importe quoi
pour un vrai casque.

## Conclusion

Pour moi, l'extrapolation sert à deviner où la pose sera quelques instants
plus tard, en supposant que les deux vitesses restent les mêmes. Ça peut aider
en réalité virtuelle parce que l'image arrive après la mesure de la pose.

Mais le test avec `dt = 20` montre aussi la limite de la méthode. Le calcul
continue sans problème, pourtant une personne ne va probablement pas garder
exactement la même vitesse pendant vingt secondes. C'est pour ça qu'une
extrapolation longue devient moins fiable : le modèle mathématique continue,
mais le mouvement réel a déjà changé.