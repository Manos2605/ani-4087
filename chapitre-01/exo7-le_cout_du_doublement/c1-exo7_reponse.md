# Exercice 7 : Le coût du doublement

## Mesure

J'ai repris le programme de l'exercice 6. Le code de mesure est disponible ici : [exo7_mesure.cpp](./exo7_mesure.cpp). Le temps mesuré correspond uniquement à la boucle qui écrit les pixels, donc au rendu simulé.

Cinq exécutions donnent les durées maximales suivantes :

- **26,4140 ms**
- **8,8268 ms**
- **7,8911 ms**
- **29,1949 ms**
- **7,3335 ms**

La mesure est donc instable sur cette machine. Le meilleur cas observé est **7,3335 ms** et le pire cas observé est **29,1949 ms**.

## Estimation d'un double rendu
En prenant le meilleur cas, faire le rendu deux fois coûterait environ :

**2 × 7,3335 = 14,6670 ms**

Le budget d'une image étant de **11 ms**, il ne resterait pas de temps pour la logique : le double rendu dépasserait déjà le budget de **14,6670 - 11,0 = 3,6670 ms**.

Dans le pire cas mesuré :
**2 × 29,1949 = 58,3898 ms**

Le dépassement atteindrait alors **58,3898 - 11,0 = 47,3898 ms**.

## Conclusion
Le rendu est déjà trop coûteux pour être effectué deux fois dans une image de 11 ms. Il faudrait donc réduire fortement le coût du rendu, par exemple en diminuant le nombre de pixels traités.
