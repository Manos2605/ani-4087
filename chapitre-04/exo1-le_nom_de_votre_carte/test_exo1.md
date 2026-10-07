# Test de l'exercice 1

## Entrée

```text
5
bureau WINDOWS 3 OPENGL DX11 VULKAN
portable WINDOWS 2 OPENGL DX11
mac MACOS 2 OPENGL METAL
serveur LINUX 0
telephone ANDROID 2 OPENGLES VULKAN
```

## Sortie obtenue

```text
bureau Vulkan
portable DirectX 11
mac Metal
serveur Software
telephone Vulkan
IGNOREES 1
LOGICIEL 1
DIFFERENTES 4
```

## Vérification

La sortie obtenue correspond exactement à la sortie attendue dans l'énoncé.

* `bureau` → **Vulkan**
* `portable` → **DirectX 11**
* `mac` → **Metal**
* `serveur` → **Software**
* `telephone` → **Vulkan**
* `IGNOREES` → **1**
* `LOGICIEL` → **1**
* `DIFFERENTES` → **4**

Le test est donc réussi.
