# Test Exo 2 : Un cube et rien d'autre

## Commande utilisée

```powershell
Get-Content entree.txt | .\Build\Bin\Debug-Windows\MaFenetre\MaFenetre.exe
```

## Entrée

```text
5
blanc 18 1000 1000 1000 2000 1 150 50
ombre 16 1000 1000 1000 2000 1 150 50
plat 18 1000 0 1000 2000 1 150 50
mur 18 4000 2500 4000 1500 1 150 50
noir 18 1000 1000 1000 2000 0 0 50
```

## Résultat obtenu

```text
blanc VISIBLE
ombre RENDER3D ETEINT
plat ECHELLE NULLE
mur CAMERA DANS LE CUBE
noir PAS DE LUMIERE
VISIBLES 1
EN PANNE 4
```

## Vérification

Le résultat obtenu correspond au résultat attendu.

* `blanc` → visible
* `ombre` → rendu 3D désactivé
* `plat` → échelle nulle
* `mur` → caméra dans le cube
* `noir` → aucune lumière
* 1 cube visible
* 4 cubes en panne
