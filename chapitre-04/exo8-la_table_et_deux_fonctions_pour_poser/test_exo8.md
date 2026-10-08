# Exo 8: La table et deux fonctions pour poser

## Test

### Commande

```powershell
jenga build
```

Puis :

```powershell
Get-Content entree.txt | .\Build\Bin\Debug-Windows\MaFenetre\MaFenetre.exe
```

### Entrée utilisée

```text
1200 800 750 60 60 600 700
4
tabouret 400 450 400 -1200 900 SOL
repere 1000 1000 1000 -1100 1200 SOL
lampe 200 400 200 400 600 TABLE
livre 240 40 170 800 800 TABLE
```

### Sortie obtenue

```text
PLATEAU 600 720 700
PIED 60 345 360
PIED 1140 345 360
PIED 60 345 1040
PIED 1140 345 1040
tabouret -1200 225 900
repere -1100 500 1200
lampe 400 950 600
livre 800 770 800
```

## Vérification

* Le plateau est centré en `(600, 720, 700)`.
* Les quatre pieds sont placés dans l'ordre demandé.
* Les pieds sont posés au sol avec leur centre à `345 mm`.
* `tabouret` et `repere` sont posés au sol avec `PoserAuSol`.
* `lampe` et `livre` sont posés sur la table avec `PoserSurTable`.
* Les deux fonctions de pose sont définies et utilisées dans le programme.
* Le résultat correspond à l'entrée imposée par l'exercice.

## Résultat

Le programme se compile et produit la sortie attendue. L'exercice 8 est donc fonctionnel.
