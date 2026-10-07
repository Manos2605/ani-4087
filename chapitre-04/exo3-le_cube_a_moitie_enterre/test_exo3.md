# Test Exo 3 : Le cube à moitié enterré

## Commande utilisée

```powershell
Get-Content entree.txt | .\Build\Bin\Debug-Windows\MaFenetre\MaFenetre.exe
```

## Entrée

```text
4
unite 1000 0
tabouret 700 350
lampe 700 700
cave 400 -300
```

## Résultat obtenu

```text
unite -500 500 ENTERRE 500
tabouret 0 700 POSE 350
lampe 350 1050 FLOTTE 350
cave -500 -100 SOUS LE SOL 200
A CORRIGER 3
PIRE 500
```

## Vérification

Le résultat obtenu correspond au résultat attendu.

* `unite` → cube enterré
* `tabouret` → cube posé sur le sol
* `lampe` → cube flottant
* `cave` → cube sous le sol
* 3 cubes à corriger
* Pire écart : 500 mm
