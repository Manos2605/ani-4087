# Exercice 7

## Énoncé
Prenez la liste des vingt-trois dépendances de la démonstration du moteur et classez-la en trois groupes : celles dont le nom suffit à deviner le rôle, celles dont vous avez une idée sans certitude, celles dont vous ne savez rien.

Pour ce troisième groupe, ouvrez l'en-tête principal de chaque module et rendez une phrase par module.

## Solution

La liste des dépendances vient de la démonstration `NKXRDemo`, dans le fichier `Applications/NKXRDemo/NKXRDemo.jenga`.

On peut retrouver cette liste avec :

```bash
nkentseudependson(
    ["NKXR", "NKRenderer", "NKRHI", "NKSL", "NKGLSlang", "NKSPIRVCross",
     "NKSerialization", "NKReflection", "NKFileSystem", "NKFont", "NKImage", "NKGlad",
     "NKEvent", "NKWindow", "NKMath", "NKTime", "NKLogger", "NKStream",
     "NKContainers", "NKMemory", "NKCore", "NKPlatform", "NKThreading"]
)
```

### 1. Noms dont je peux facilement deviner le rôle

* **NKXR** : réalité virtuelle / XR
* **NKRenderer** : rendu graphique
* **NKSerialization** : sérialisation des données
* **NKReflection** : réflexion
* **NKFileSystem** : système de fichiers
* **NKFont** : gestion des polices
* **NKImage** : gestion des images
* **NKEvent** : gestion des événements
* **NKWindow** : gestion des fenêtres
* **NKMath** : mathématiques
* **NKTime** : gestion du temps
* **NKLogger** : gestion des logs
* **NKStream** : gestion des flux
* **NKContainers** : conteneurs de données
* **NKMemory** : gestion de la mémoire
* **NKCore** : fonctionnalités principales / de base
* **NKPlatform** : gestion de la plateforme
* **NKThreading** : gestion des threads

### 2. Noms dont j'ai une idée mais sans certitude

* **NKRHI** : probablement une interface avec le matériel ou le système de rendu graphique.
* **NKSL** : probablement lié aux shaders ou à leur gestion.
* **NKGlad** : probablement lié à OpenGL et au chargement de ses fonctions.

### 3. Noms dont je ne connaissais pas précisément le rôle
Après avoir regardé les en-têtes principaux de ces modules :
* **NKGLSlang** : ce module fournit les éléments nécessaires pour travailler avec le langage de shaders GLSL.
* **NKSPIRVCross** : ce module permet de travailler avec SPIR-V et de faire des conversions entre différents formats ou langages de shaders.

## Conclusion

En regardant uniquement les noms, une grande partie des modules est assez facile à comprendre, comme `NKWindow`, `NKImage`, `NKFont` ou `NKMemory`. Par contre, des noms comme `NKGLSlang` et `NKSPIRVCross` sont beaucoup moins intuitifs et nécessitent de regarder leur code pour comprendre leur rôle.
