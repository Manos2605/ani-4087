# Test Exo 6: La porte et les fenêtres

## Commande utilisée

```powershell
Get-Content entree.txt | .\Build\Bin\Debug-Windows\MaFenetre\MaFenetre.exe
```

## Entrée

```text
4000 2500 5
5
porte -700 1000 900 2000 40 20
fenetre -900 1400 1200 1000 40 20
colle 900 1400 1200 1000 2 0
haute 0 2200 1200 1000 40 20
flottante 1200 1000 600 600 20 300
```

## Résultat obtenu

```text
porte 40 OK
fenetre 40 OK
colle 1 CLIGNOTE
haute 40 DEBORDE
flottante 310 DECOLLE
OK 2
A REPRENDRE 3
```

## Vérification

Le résultat obtenu correspond exactement au résultat attendu.

* `porte` est correctement détectée comme `OK`.
* `fenetre` est correctement détectée comme `OK`.
* `colle` est détectée comme `CLIGNOTE`.
* `haute` est détectée comme `DEBORDE`.
* `flottante` est détectée comme `DECOLLE`.
* `OK 2` est correct.
* `A REPRENDRE 3` est correct.
* Le test est réussi.
