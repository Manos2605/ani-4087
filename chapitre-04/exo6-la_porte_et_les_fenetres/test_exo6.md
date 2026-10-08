# Test Exo 7: Les drapeaux qu'on nomme

## Commande utilisée

```powershell
Get-Content entree.txt | .\Build\Bin\Debug-Windows\MaFenetre\MaFenetre.exe
```

## Entrée

```text
4
RENDER3D
SHADOW
TEXT
RENDU3D
```

## Résultat obtenu

```text
INCONNU RENDU3D
VALEUR 22
HEXA 0x00000016
MANQUE TEXT RENDER2D
ALLUMES 3
ETEINTS 10
```

## Vérification

Le résultat obtenu correspond exactement au résultat attendu.

* `RENDU3D` est correctement détecté comme nom inconnu.
* La valeur finale est `22`.
* La représentation hexadécimale est `0x00000016`.
* La dépendance manquante `TEXT RENDER2D` est correctement signalée.
* `ALLUMES 3` est correct.
* `ETEINTS 10` est correct.
* Le test est réussi.
