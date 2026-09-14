# Exercice 6 : La pire image

# Enoncer
Prenez n'importe quel programme à vous qui dessine en boucle, ou écrivez-en un qui ne fait qu'effacer l'écran. Mesurez, sur mille images, non pas la cadence moyenne mais la durée de la plus longue image, et le nombre d'images qui dépassent onze millisecondes.

Rendez les deux chiffres, et dites si votre programme tiendrait dans un casque.

## Mesure
J'ai utilisé un programme qui efface l'écran et le redessine en boucle. J'ai mesuré la durée de chaque image pendant 1 000 images. Le programme utilisé est disponible ici : [exo6_mesure.cpp](./exo6_mesure.cpp).

- Durée de la plus longue image : **41,6356 ms**
- Nombre d'images qui dépassent 11 ms : **5 sur 1 000**

## Conclusion

La cadence moyenne ne suffit pas pour savoir si le programme peut fonctionner dans un casque. Il faut surtout regarder la plus longue image et compter les images qui dépassent la limite de 11 ms.

Avec mes mesures, le programme **ne pourrait pas** tenir correctement dans un casque. La plus longue image dure **41,6356 ms**, ce qui est beaucoup plus que la limite de 11 ms. De plus, **5 images sur 1 000** dépassent cette limite. Ces images risquent de rater leur échéance d'affichage et de provoquer une gêne. Cela montre qu'en réalité virtuelle, il faut que les images soient rapides de manière régulière, et pas seulement que la moyenne soit bonne.
