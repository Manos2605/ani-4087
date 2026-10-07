# Test Exo 5 : L'ordre inversé

## Commande utilisée

```powershell
Get-Content entree.txt | .\Build\Bin\Debug-Windows\MaFenetre\MaFenetre.exe
```

## Entrée

```text
4
fond 0 1250 -2000 4000 2500 100
gauche -2000 1250 0 100 2500 4000
repere -1100 500 1200 1000 1000 1000
porte -700 1000 -1980 900 2000 40
```

## Résultat obtenu

```text
fond 0 3125 -200 1875
gauche -200 3125 0 1875
repere -1100 500 1200 0
porte -630 2000 -79 1901
DEPLACES 3
PIRE 1901
```

## Vérification

Le résultat obtenu correspond exactement au résultat attendu.

* Les positions obtenues avec le mauvais ordre sont correctes.
* Les divisions entières sont correctement tronquées vers zéro.
* `repere` reste à la même position avec un écart de `0`.
* `DEPLACES 3` est correct.
* Le plus grand écart est `1901`, donc `PIRE 1901`.
* Le test est réussi.
