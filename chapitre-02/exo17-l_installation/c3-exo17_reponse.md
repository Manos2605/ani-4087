# Exercice 17

## Énoncé

Installez votre paquet sur un appareil et lancez-le. Rendez une photographie de l'écran de l'appareil.

Si l'installation échoue, rendez le message exact : il est plus instructif qu'une réussite.

## Installation

J'ai connecté mon téléphone Android au PC après avoir activé le mode développeur et le débogage USB. J'ai ensuite vérifié qu'il était détecté avec :

```powershell
adb devices
```

Résultat :

```text
List of devices attached
ts6595d6amjraaqg    device
```

Le téléphone est donc bien détecté par ADB.

J'ai ensuite essayé d'installer l'APK avec :

```powershell
adb install -r .\dist\MaSalle.apk
```

L'installation a échoué avec le message suivant :

```text
Performing Incremental Install
Performing Streamed Install
adb.exe: failed to install .\dist\MaSalle.apk: Failure [INSTALL_FAILED_NO_MATCHING_ABIS: INSTALL_FAILED_NO_MATCHING_ABIS: Failed to extract native libraries, res=-113]
```

## Résultat

L'installation n'a pas pu être effectuée à cause d'une incompatibilité d'architecture entre l'APK et le téléphone.

L'APK généré contenait la bibliothèque :

```text
lib/x86_64/libMaSalle.so
```

alors que la configuration actuelle de mon fichier Jenga prévoit l'architecture Android `arm64-v8a`.

![salle.jenga](salle.jenga)

La ligne concernée dans le fichier de configuration est :

```python
androidabis(["arm64-v8a"])
```

Cette configuration indique que l'application Android doit être construite pour l'architecture ARM64.
