# Test Exo 4 : Le sol et les quatre murs

## Commande utilisée

```powershell
Get-Content entree.txt | .\Build\Bin\Debug-Windows\MaFenetre\MaFenetre.exe
```

## Entrée

```text
4000 100
4
fond 0 -2050 4000 100
entree 0 2050 4000 100
gauche -2050 0 100 4000
droit 2050 0 100 4000
```

## Résultat obtenu

```text
fond -2000 2000 -2100 -2000
entree -2000 2000 2000 2100
gauche -2100 -2000 -2000 2000
droit 2000 2100 -2000 2000
FOND_GAUCHE TROU
FOND_DROIT TROU
ENTREE_GAUCHE TROU
ENTREE_DROIT TROU
TROUS 4
```

## Vérification

Le résultat obtenu correspond au résultat attendu.

* Les quatre emprises des murs sont correctement calculées.
* Les quatre angles sont détectés comme des `TROU`.
* Le nombre total de trous est correctement calculé : `TROUS 4`.
* Le test est réussi.
