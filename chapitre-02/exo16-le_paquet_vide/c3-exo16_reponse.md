# Exercice 16

## Énoncé

Empaquetez votre programme, même s'il n'affiche encore qu'une couleur. Signez-le.

Rendez la taille du paquet obtenu et la liste de ce qu'il contient. La plupart des outils d'archive savent l'ouvrir : un `apk` est une archive ordinaire.

## Solution

J'ai d'abord empaqueté le projet Android en Release avec la commande :

```powershell
jenga package --platform android --config Release --project MaSalle --type apk
```

Le paquet obtenu est : [MaSalle.apk](MaSalle.apk)

Ensuite, j'ai signé l'APK avec ma clé de signature :

```powershell
apksigner sign --ks .\monapp.keystore --ks-key-alias monapp-key --ks-pass pass:android --key-pass pass:Mon_mot_de_passe .\dist\MaSalle.apk
```

Le mot de passe réel n'est pas indiqué dans ce rapport.

La vérification de la signature donne :

```text
Verifies
Verified using v2 scheme (APK Signature Scheme v2): true
Verified using v3 scheme (APK Signature Scheme v3): true
Number of signers: 1
```

L'APK est donc bien signé.

### Taille du paquet

J'ai utilisé la commande :

```powershell
Get-Item .\dist\MaSalle.apk | Select-Object Name,Length,@{Name="Taille_Ko";Expression={[math]::Round($_.Length / 1KB, 2)}}
```

La taille obtenue est :

```text
28,41 Ko
```

### Contenu du paquet

J'ai utilisé la commande :

```powershell
tar -tf .\dist\MaSalle.apk
```

Le paquet contient :

```text
AndroidManifest.xml
resources.arsc
lib/x86_64/libMaSalle.so
META-INF/MONAPP-K.SF
META-INF/MONAPP-K.RSA
META-INF/MANIFEST.MF
```

Les principaux fichiers utilisés sont :

* [main.cpp](main.cpp)

* [MaSalle.apk](MaSalle.apk)

Le fichier `monapp.keystore` reste local et est ignoré par Git grâce au `.gitignore`.

## Conclusion

J'ai réussi à construire le programme Android, à générer le paquet `MaSalle.apk` et à le signer. Le contenu de l'APK a également été vérifié avec `tar`.

La signature finale est valide avec les schémas APK V2 et V3.
