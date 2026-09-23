**# Exercice 10**

## Énoncé**

Écrivez une boucle qui fait un calcul lourd et le chronomètre. Construisez en Debug puis en Release et mesurez.

Rendez le rapport. Puis dites, sachant qu'une image de casque dure onze millisecondes, laquelle des deux mesures vous aurait fait prendre une mauvaise décision.

## Le calcul mesuré**

Pour faire le test, j'ai écrit une boucle qui effectue plusieurs calculs avec `sin` et `cos`. Le calcul est répété sur plusieurs images et j'utilise `std::chrono` pour mesurer le temps nécessaire à chaque calcul.

### Fichier [main.cpp](main.cpp)**

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

## Conditions de mesure**

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
## Vérification des exécutables

Comme les premières mesures donnaient des temps très proches entre Debug et Release, j'ai vérifié les exécutables produits par les deux configurations.

Avec PowerShell, j'ai utilisé :
```
Get-Item ".\Build\Bin\Debug-Windows\MaSalle\MaSalle.exe" |
    Select-Object Length, LastWriteTime
```
et :
```
Get-Item ".\Build\Bin\Release-Windows\MaSalle\MaSalle.exe" |
    Select-Object Length, LastWriteTime
```
Les résultats obtenus sont :
```
Configuration	Taille de l'exécutable	Dernière modification
Debug	80 577 octets	23/09/2026 à 10:07:55
Release	80 577 octets	23/09/2026 à 10:08:57
```
Les deux fichiers ont donc bien été reconstruits à des heures différentes. En revanche, ils ont exactement la même taille.

Cette vérification est importante car une mesure de performance n'a de sens que si le programme exécuté correspond réellement à la configuration que l'on veut mesurer.

## Résultats

J'ai effectué trois mesures pour chaque configuration afin d'avoir une moyenne.

| Configuration | Essai 1 | Essai 2 | Essai 3 | Moyenne |
|-|-|-|-|-|
| Debug | 11,5265 ms | 11,7278 ms | 11,5265 ms | **11,5936 ms** |
| Release | 11,0831 ms | 10,8887 ms | 11,2260 ms | **11,0659 ms** |

Pour comparer les deux configurations, j'utilise le pourcentage d'écart :

```text
Écart = |Debug - Release| / Debug × 100
```

L'écart obtenu entre les deux moyennes est donc d'environ **4,55 %**.

## Comparaison avec les 11 ms

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

## Quelle mesure m'aurait fait prendre une mauvaise décision ?

Une image de casque dispose d'un budget de **11 ms**.

Avec mes mesures :

```text
Debug   : 11,5936 ms, soit environ 0,5936 ms au-dessus du budget.

Release : 11,0659 ms, soit environ 0,0659 ms au-dessus du budget.
```

Les deux mesures indiquent donc que le calcul dépasse légèrement les **11 ms**. La mesure Debug donne toutefois un résultat plus défavorable que la mesure Release.

Avec mes résultats, je ne peux pas dire que l'une des deux mesures aurait conduit à elle seule à une décision différente concernant le respect du budget, puisque les deux dépassent la limite de 11 ms. La différence est surtout que le dépassement mesuré en Debug est plus important.

Enfin, j'ai vérifié les exécutables produits. Les fichiers `MaSalle.exe` de Debug et de Release ont été générés à des heures différentes, mais ils ont exactement la même taille : **80 577 octets**. Cette vérification montre qu'il est important de contrôler non seulement la configuration utilisée pour construire le programme, mais également celle utilisée pour lancer le programme, avant de tirer une conclusion définitive sur les performances.
