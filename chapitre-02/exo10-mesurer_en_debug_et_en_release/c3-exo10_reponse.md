# Exercice 10

## Enoncer

Écrivez une boucle qui fait un calcul lourd et le chronomètre. Construisez en Debug puis en Release et mesurez.

Rendez le rapport. Puis dites, sachant qu'une image de casque dure onze millisecondes, laquelle des deux mesures vous aurait fait prendre une mauvaise décision.

## Solution

Pour faire le test, j'ai écrit une boucle dans le fichier `main.cpp` qui effectue beaucoup de calculs et j'ai utilisé `std::chrono` pour mesurer le temps d'exécution.

Mon `main.cpp` est le suivant :

J'ai ensuite construit le projet une première fois en **Debug** :

```bash
jenga build --config Debug
```

Puis j'ai fait la même chose en **Release** :

```bash
jenga build --config Release
```

J'ai exécuté le programme dans les deux configurations et j'ai relevé les temps d'exécution.

### Résultats Temps d'exécution

| Configuration |    Essai 1 |    Essai 2 |    Essai 3 |        Moyenne |
|-|-|-|-|-|
| Debug         | 254.449 ms | 255.994 ms | 254.885 ms | **255.109 ms** |
| Release       | 254.416 ms | 254.713 ms | 262.416 ms | **257.182 ms** |

Dans mes mesures, la version **Release n'est pas plus rapide que la version Debug**. Elle est même légèrement plus lente sur ces trois essais. La différence reste cependant assez faible, donc elle peut être liée aux variations du temps d'exécution lors des différents essais.

## Comparaison avec les 11 ms

Une image de casque dure **11 ms**. Le cours précise qu'il ne faut pas mesurer les performances en **Debug**, car cette configuration peut donner une mauvaise idée des performances réelles du programme. L'écart entre Debug et Release peut être très important pour un calcul lourd.

Dans mes essais, j'obtiens :

* **Debug : 255.109 ms en moyenne**
* **Release : 257.182 ms en moyenne**

Dans mon cas, les résultats sont très proches et la Release est même légèrement plus lente sur les trois essais. Cela peut venir des variations lors de l'exécution et ne représente pas forcément le comportement général des deux configurations.

Cependant, selon le cours, **la mesure Debug est celle qui pourrait me faire prendre une mauvaise décision**, car elle n'est pas représentative des performances finales du programme. Pour juger si un calcul peut tenir dans un budget de **11 ms**, il faut donc se baser sur la configuration **Release**.

## Conclusion

Cela me montre qu'il ne faut pas utiliser le temps obtenu en **Debug** pour juger les performances d'un calcul lourd. Même si mes mesures sont ici très proches, le cours précise que l'écart entre Debug et Release peut être très important.

Pour une application de casque où le budget est de **11 ms par image**, je dois donc mesurer et prendre mes décisions à partir de la **version Release**, et non de la version Debug.