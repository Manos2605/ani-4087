# Exercice 11

## Enoncer
Ouvrez le fichier de projet de la démonstration XR du moteur et lisez-le en entier, y compris les commentaires.

Rendez une page : ce qu'il construit, ce dont il dépend, ce qui change d'un système à l'autre, et les trois pièges qu'il documente. Pour chacun des trois, dites ce qui se passerait sans la ligne concernée.

## Solution
## 1. Ce qu'il construit

Le projet s'appelle **NKXRDemo** et il est déclaré avec `windowedapp()`, donc il construit une application graphique avec une fenêtre.

C'est une démonstration XR du moteur qui permet de simuler un rendu stéréo sur ordinateur, sans utiliser directement un casque XR. La scène est rendue pour les deux yeux avec deux renderers 3D offscreen, puis la frame finale est composée avec un renderer 2D.

Le projet permet donc de tester la partie XR sur un environnement desktop.

## 2. Ce dont il dépend

Le projet dépend de plusieurs modules du moteur **Nkentseu**.

On retrouve notamment :

* `NKCore`
* `NKMemory`
* `NKPlatform`
* `NKRenderer`
* `NKRHI`
* `NKSL`
* `NKGLSlang`
* `NKSPIRVCross`
* `NKSerialization`
* `NKReflection`
* `NKFileSystem`
* `NKFont`
* `NKImage`
* `NKGlad`
* `NKEvent`
* `NKWindow`
* `NKMath`
* `NKTime`
* `NKLogger`
* `NKStream`
* `NKContainers`
* `NKThreading`

Le projet utilise aussi les headers de **Vulkan**, notamment pour la partie `NkVulkanDevice.h` utilisée avec OpenXR.

Les dépendances vont donc des modules de base du moteur jusqu'aux modules de rendu, de shaders et de gestion des fenêtres.

## 3. Ce qui change selon le système

Le fichier possède des configurations différentes selon le système d'exploitation.

### Windows

Sous Windows, le projet utilise `TC_WINDOWS`.

Il ajoute notamment les defines :

```text
WIN32_LEAN_AND_MEAN
_UNICODE
UNICODE
```

Et plusieurs bibliothèques Windows comme :

```text
user32
gdi32
opengl32
d3d11
d3d12
dxgi
advapi32
```

### Linux

Sous Linux, le projet utilise `clang-native`.

Il ajoute notamment :

```text
NKENTSEU_FORCE_WINDOWING_XLIB_ONLY
```

Et utilise :

```text
pthread
X11
Xext
GL
```

### macOS

Sous macOS, le projet utilise également `clang-native`.

Il utilise notamment les frameworks :

```text
Cocoa
QuartzCore
OpenGL
```

Il y a aussi les configurations `Debug` et `Release`, qui permettent de changer les paramètres de compilation et d'optimisation.

## 4. Les trois pièges documentés

### Piège 1 : `NKGLSlang` et `NKSPIRVCross`

Le fichier ajoute explicitement `NKGLSlang` et `NKSPIRVCross` dans `nkentseudependson`.

Ces deux modules sont nécessaires parce que les composants de rendu et de shaders les utilisent.

Le problème est que ces dépendances ne sont pas récupérées automatiquement par transitivité jusqu'à l'exécutable final.

**Si on enlève ces deux dépendances :**

* les sources peuvent quand même être compilées ;
* mais au moment de l'édition de liens, certaines fonctions utilisées par `NKSL` ou `NKRHI` ne seront pas trouvées ;
* on obtient donc des erreurs de symboles non définis.

Le problème arrive donc à l'étape de **l'édition de liens**.

### Piège 2 : `NK_RHI_VK_ENABLED`

Le projet contient :

```text
defines(["NK_RHI_VK_ENABLED"])
```

Ce define permet d'activer la vraie implémentation Vulkan dans `NkVulkanDevice.h`.

Sans ce define, le header utilise une version **STUB** de la classe.

**Si on enlève cette ligne :**

* le header peut toujours être inclus ;
* mais la vraie classe Vulkan n'est plus disponible ;
* le code OpenXR qui utilise les fonctionnalités Vulkan nécessaires ne peut alors plus compiler.

Le problème apparaît donc lors de la **compilation**.

### Piège 3 : `advapi32`

Sous Windows, le projet ajoute :

```text
advapi32
```

Cette bibliothèque est utilisée pour accéder à certaines fonctions du registre Windows.

Dans le cas de la démo XR, elle permet notamment d'utiliser `RegGetValueA` pour rechercher le runtime OpenXR actif.

**Si on enlève `advapi32` :**

* le code source peut être compilé ;
* mais lors de l'édition de liens, `RegGetValueA` n'est pas trouvé ;
* le linker signale alors un symbole non défini.

Ce problème apparaît donc à l'**édition de liens** et uniquement sous Windows.

## 5. Résumé

| Piège | Ce qui manque | Conséquence | Étape |
| - | - | - | - |
| `NKGLSlang` / `NKSPIRVCross` | Dépendances explicites | Symboles non trouvés | Édition de liens |
| `NK_RHI_VK_ENABLED` | Define Vulkan | Utilisation du STUB et erreur avec le code Vulkan | Compilation      |
| `advapi32` | Bibliothèque Windows | `RegGetValueA` non trouvé | Édition de liens |

En résumé, le fichier `.jenga` ne sert pas seulement à compiler le projet. Il permet aussi de gérer ses dépendances et les différences entre Windows, Linux et macOS. Les trois pièges montrent surtout que certaines dépendances ou définitions doivent être ajoutées explicitement pour que le projet puisse aller jusqu'à l'édition de liens.
