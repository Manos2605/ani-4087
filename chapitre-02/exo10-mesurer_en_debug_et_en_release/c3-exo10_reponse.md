**# Exercice 10**

**## Énoncé**

Écrivez une boucle qui fait un calcul lourd et le chronomètre. Construisez en Debug puis en Release et mesurez.

Rendez le rapport. Puis dites, sachant qu'une image de casque dure onze millisecondes, laquelle des deux mesures vous aurait fait prendre une mauvaise décision.

**## Le calcul mesuré**

Pour faire le test, j'ai écrit une boucle qui effectue plusieurs calculs avec `sin` et `cos`. Le calcul est répété sur plusieurs images et j'utilise `std::chrono` pour mesurer le temps nécessaire à chaque calcul.

**### Fichier [main.cpp](main.cpp)**

Les parties importantes du code sont la boucle de calcul et la mesure du temps :

```cpp
const int nbImages = 20;
const int nbCalculs = 500000;

double total = 0.0;
double resultat = 0.0;

for (int image = 0; image < nbImages; image++) {

    auto debut = std::chrono::steady_clock::now();

    for (int i = 0; i < nbCalculs; i++) {
        double x = i * 0.00001;
        resultat += std::sin(x) * std::cos(x);
    }

    auto fin = std::chrono::steady_clock::now();

    double temps = std::chrono::duration<double, std::milli>(
        fin - debut
    ).count();

    total += temps;
}

double moyenne = total / nbImages;
```

Le résultat du calcul est également affiché à la fin afin de conserver le calcul effectué :

```cpp
std::cout << "Temps moyen : " << moyenne << " ms" << std::endl;
std::cout << "Resultat : " << resultat << std::endl;
```

**## Conditions de mesure**

J'ai effectué les mesures sous Windows en utilisant Jenga avec les deux configurations.

J'ai d'abord construit et exécuté le programme en Debug :

```bash
jenga clean
jenga build --config Debug
jenga run --config Debug
```

Puis j'ai fait la même chose en Release :

```bash
jenga build --config Release
jenga run --config Release
```

Cela permet de mesurer le programme avec les deux configurations.

**## Résultats**

J'ai effectué trois mesures pour chaque configuration afin d'avoir une moyenne.

| Configuration | Essai 1    | Essai 2    | Essai 3    | Moyenne |
| - | - | - | - | - |
| Debug | 11,5265 ms | 11,7278 ms | 11,5265 ms | **11,5936 ms** |
| Release | 11,0831 ms | 10,8887 ms | 11,2260 ms | **11,0659 ms** |

Pour comparer les deux configurations, j'utilise le pourcentage d'écart :

```text
Écart = |Debug - Release| / Debug × 100
```

L'écart obtenu entre les deux moyennes est donc d'environ **4,55 %**.

**## Comparaison avec les 11 ms**

Une image de casque dure **11 ms**.

Ma mesure moyenne en Debug est de :

```text
11,5936 ms
```

Ma mesure moyenne en Release est de :

```text
11,0659 ms
```

Je compare donc directement ces deux valeurs au budget de 11 ms :

* Debug : **11,5936 ms** au-dessus de 11 ms.
* Release : **11,0659 ms** légèrement au-dessus de 11 ms.

Les deux mesures sont donc légèrement au-dessus de 11 ms. Cependant, la mesure Release est beaucoup plus proche de la limite et l'écart entre les deux configurations est d'environ **4,55 %**.

**## Quelle mesure m'aurait fait prendre une mauvaise décision ?**

Le cours indique qu'il ne faut pas prendre de décision de performance à partir d'une mesure en Debug.

Dans mes mesures, le Debug donne **11,5936 ms**, tandis que le Release donne **11,0659 ms**. La mesure Debug donne donc une vision plus défavorable du temps d'exécution.

Avec une limite de **11 ms**, quelques dixièmes de milliseconde peuvent être importants. Une décision prise uniquement à partir de la mesure Debug pourrait donc être différente de celle prise avec la mesure Release.