# Exercice 1

## Énoncé

Ouvrez le fichier de projet de la démonstration XR du moteur et lisez-le en entier, y compris les commentaires.

Rendez une page : ce qu'il construit, ce dont il dépend, ce qui change d'un système à l'autre, et les trois pièges qu'il documente. Pour chacun des trois, dites ce qui se passerait sans la ligne concernée.

## Solution

### 1. Recherche de l'exemple

Pour commencer, j'ai affiché la liste des exemples disponibles avec la commande :

```powershell
jenga examples list
```

Dans la liste, j'ai trouvé l'exemple :

```text
27_nk_window
```

Cet exemple concerne le système de fenêtres multiplateforme `NKWindow`.

J'ai ensuite copié l'exemple dans mon dossier de travail avec :

```powershell
jenga examples copy 27_nk_window .
```

Après cela, j'ai ouvert le dossier de l'exemple et j'ai recherché le fichier de projet Jenga. C'est ce fichier que j'ai lu en entier, en prenant aussi en compte les commentaires.

### 2. Ce que construit le projet

Le projet construit principalement une bibliothèque statique appelée **NKWindow**.

Le fichier indique que :

```text
NKWindow: Static library with platform abstraction
Sandbox: Demo application using NKWindow API
```

Donc `NKWindow` est une bibliothèque C++ qui permet de gérer les fenêtres et les événements de manière multiplateforme.

Le projet contient aussi trois applications de démonstration :

* `Sandbox`
* `SandboxCamera`
* `SandboxCameraFull`

Ces applications utilisent la bibliothèque `NKWindow`.

Le projet peut être construit en **Debug** ou en **Release** et utilise le C++17.

### 3. De quoi dépend le projet ?

Le projet utilise notamment les répertoires :

```text
src
../Externals
```

Les applications de démonstration utilisent également `NKWindow` comme dépendance :

```text
links(["NKWindow"])
dependson(["NKWindow"])
```

Il y a aussi des bibliothèques qui dépendent du système utilisé.

Par exemple, sous Windows, on trouve :

```text
user32
gdi32
opengl32
dwmapi
shell32
xinput
```

Sous Linux :

```text
pthread
X11
```

Et sous Android :

```text
android
log
EGL
GLESv3
camera2ndk
mediandk
```

Donc les dépendances ne sont pas exactement les mêmes selon la plateforme.

### 4. Ce qui change d'un système à l'autre

Le projet est prévu pour plusieurs plateformes :

* Windows
* Linux
* macOS
* Android
* iOS
* Web
* HarmonyOS

Le fichier utilise des `filter("system:...")` pour appliquer une configuration différente selon le système.

**Windows :**
Le projet utilise le backend **Win32**, avec le toolchain `clang-mingw` et les bibliothèques propres à Windows.

**Linux :**
Il existe deux configurations. Une configuration `headless` utilise le backend **NOOP**, notamment lorsqu'il n'y a pas d'affichage. La configuration normale utilise **XLib/X11**.

**macOS :**
Le backend utilisé est **Cocoa**, avec des frameworks comme `Cocoa`, `QuartzCore`, `OpenGL` et `Metal`.

**Android :**
Le projet utilise le **NDK**, `NativeActivity` et `EGL`. Il configure aussi les versions du SDK ainsi que plusieurs architectures Android.

**iOS :**
Le projet utilise **UIKit**, `QuartzCore`, `OpenGLES` et `AVFoundation`.

**Web :**
Le projet utilise **Emscripten** et **WebAssembly**, avec un canvas et une mémoire initiale de 32 MB.

**HarmonyOS :**
Pour le moment, le projet utilise un backend **Noop** comme solution de remplacement. Le commentaire indique qu'un vrai backend HarmonyOS devra être ajouté plus tard.

### 5. Les trois pièges documentés

### Piège 1 : le mode Linux `headless`

Le fichier contient :

```text
with filter("system:Linux && options:headless"):
```

Cette configuration utilise le backend `NOOP`. Le commentaire précise que cela permet notamment de faire fonctionner le projet en CI ou avec WSL lorsqu'il n'y a pas d'affichage X11.

Les fichiers générés sont également placés dans des dossiers différents pour ne pas les mélanger avec les fichiers Linux normaux.

**Sans cette configuration**, le projet pourrait essayer d'utiliser le backend graphique X11 alors qu'aucun affichage n'est disponible.

### Piège 2 : `ASYNCIFY` pour le Web

Pour la plateforme Web, on trouve :

```text
emscriptenextraflags(["-s", "ASYNCIFY"])
```

Le commentaire précise que cette option est nécessaire pour les boucles synchrones de type desktop sur le Web.

**Sans cette ligne**, les boucles synchrones utilisées par l'application pourraient ne pas fonctionner correctement avec Emscripten.

#### Piège 3 : le fallback HarmonyOS

Pour HarmonyOS, le projet utilise :

```text
files([
    "src/NKWindow/Platform/Noop/**.cpp",
    "src/NKWindow/Platform/Noop/**.h",
])
```

Le commentaire indique qu'il s'agit d'un **fallback si le backend n'est pas encore implémenté**.

Le fichier indique également qu'un vrai backend HarmonyOS pourra être ajouté plus tard.

**Sans ce fallback**, il n'y aurait pas les fichiers d'implémentation utilisés actuellement pour cette configuration HarmonyOS, ce qui pourrait empêcher la compilation de cette cible.

## Conclusion

Après avoir lu le fichier de projet, j'ai compris que `27_nk_window` est un exemple de **framework de fenêtrage multiplateforme** basé sur `NKWindow`.

Le fichier Jenga permet de définir ce qui doit être construit, les dépendances, les plateformes supportées et les configurations particulières de chaque système.
