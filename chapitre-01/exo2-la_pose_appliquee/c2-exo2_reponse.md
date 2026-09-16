# Exercice 2 : la pose appliquée

## Énoncé

Écrivez la structure Pose, avec une position et un quaternion, et la fonction qui applique une pose à un point : rotation puis translation.

Le quaternion est donné par ses quatre composantes, déjà normalisé. Votre programme lit une pose et un point, et affiche le point transformé.

## Solution

J'ai utilisé la convention `q = (w, x, y, z)` pour le quaternion. Pour
transformer le point, je commence par le tourner avec le quaternion. Après ça,
j'ajoute la position de la pose sur les trois coordonnées.

Donc l'ordre est bien : **rotation puis translation**


## Code
Le programme complet se trouve dans [c2-exo2.cpp](./c2-exo2.cpp).
