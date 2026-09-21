# Exercice 3
## Enoncer
Construisez votre projet en Debug puis en Release. Comparez la taille des deux exécutables et le temps de construction.

Rendez les quatre nombres.

## Solution
J'ai gardé le même projet qu'a l'exercice 1 et je lance et étant donné que par défaut la configuration est en `debug` j'ai lancé:
### Debug
```text
jenga build --config Debug
```
### Release
```text
jenga build --config Release
```
## Commande utilisée pour afficher la taille des exécutables
Pour obtenir la taille exacte des exécutables en octets, j'ai utilisé la commande PowerShell suivante :
```powershell
Get-ChildItem -Recurse -Filter *.exe | ForEach-Object { Write-Host $_.FullName Write-Host "Taille :" $_.Length "octets"}
```

## tableau temps d'éxécution
| Configuration | Essai 1 | Essai 2 | Essai 3 | Moyenne |
|-|-|-|-|-|
| Debug | 0.94 s | 0.94 s | 0.95 s | `0,943 s` |
| Release | 0.96 s | 0.91 s | 1.04 s | `0,97 s` |
## tableau taille de l'exécutable en octets
| Configuration | Essai 1 | Essai 2 | Essai 3 | Moyenne |
|-|-|-|-|-|
| Debug | 54542  | 54542 | 54542 | `54542 octets` |
| Release | 54542 | 54542 | 54542 | `54542 octets` |

## Conclusion et remarque
## Commande utilisée pour afficher la taille des exécutables

Pour obtenir la taille exacte des exécutables en octets, j'ai utilisé la commande PowerShell suivante :

```powershell
Get-ChildItem -Recurse -Filter *.exe | Select-Object FullName, Length
```

La colonne `Length` permet d'obtenir la taille exacte de chaque fichier `.exe`.

## Conclusion et remarque
## Conclusion et remarque
Pour le temps de construction, les trois mesures donnent une moyenne de **0,943 s** en Debug et de **0,97 s** en Release. Les temps sont donc très proches, avec des variations d'une exécution à l'autre.

On constate que les deux exécutables ont exactement la même taille, soit **54 542 octets** pour Debug et Release.

J'ai également utilisé la commande :
```powershell
Get-ChildItem -Recurse -Filter *.exe | Get-FileHash
```

Les deux exécutables ont des hash SHA256 différents. Le fichier Debug et le fichier Release ne sont donc pas identiques, même s'ils ont exactement la même taille.

En regardant le fichier salle.jenga, on voit que les configurations Debug et Release sont seulement déclarées avec :
`configurations(['Debug', 'Release'])`

Aucune option de compilation spécifique à Debug ou à Release n'est définie dans le fichier. On ne peut donc pas identifier, à partir du fichier salle.jenga seul, une différence précise de réglage entre les deux configurations.

Pour ce petit projet, les deux configurations produisent ainsi des exécutables de même taille, avec des temps de construction très proches, mais avec des contenus binaires différents.

