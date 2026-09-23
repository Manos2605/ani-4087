# Exercice 8

## Enoncé

Écrivez les filtres pour Windows et pour Linux dans votre projet, chacun avec ses bibliothèques. Faites vérifier par un camarade sur l'autre système que le projet se construit chez lui sans modification. Si vous n'avez accès qu'à un système, écrivez quand même les deux et dites ce que vous n'avez pas pu vérifier.

## Solution

Dans cet exercice, j'ai ajouté les filtres Windows et Linux directement dans mon fichier [salle.jenga](salle.jenga).

### Fichier `salle.jenga`
J'ai ajouté un filtre pour chaque système avec les bibliothèques correspondantes :
```python
with filter("system:Windows"):
    links(["user32", "gdi32", "opengl32", "dinput8", "dxguid", "winmm"])

with filter("system:Linux"):
    links(["pthread", "X11", "Xext", "GL"])
```

Le filtre Windows ajoute les bibliothèques nécessaires pour Windows et le filtre Linux ajoute celles nécessaires pour Linux.

### Fichier [main.cpp](main.cpp)
Dans mon code C++, j'ai gardé les conditions de préprocesseur pour adapter le code au système utilisé :

```cpp
#ifdef _WIN32
    #include <windows.h>
#elif defined(__linux__)
    #include <unistd.h>
#endif
```

Et aussi j'affiche le système détecté :
```cpp
#ifdef _WIN32
    std::cout << "Système : Windows" << std::endl;
#elif defined(__linux__)
    std::cout << "System : Linux" << std::endl;
#else
    std::cout << "System : Autre que Windows et Linux" << std::endl;
#endif
```

Ici, `_WIN32` est utilisé pour Windows et `__linux__` pour Linux.

### Vérification sous Linux
Je n'ai pas pu fournir une sortie de construction Sous Linux pour cet exercice, car je n'ai pas pu avoir accès à un système Linux.

Je ne considère donc pas la construction Windows comme vérifiée.

### Vérification sous Windows

J'ai construit le projet sous Windows avec :
```
jenga build
```
Les informations importantes de la sortie sont :
```
Configuration: Debug
Target:        Windows x86_64
Toolchain:     mingw

✓   [1/1] Compiled: main.cpp
✓ Built: Build\Bin\Debug-Windows\MaSalle\MaSalle.exe
```

J'ai ensuite verifier en exécutant sous windows  :
```
jenga run
```
Le programme affiche :
```
Système : Windows
```
## Conclusion
Les **filtres du fichier `.jenga`** et les `#ifdef` du code C++ ne font pas la même chose.

Le `#ifdef` permet de choisir les parties du code C++ à compiler selon le système, tandis que le filtre `system:Windows` ou `system:Linux` permet de choisir les bibliothèques à donner à l'éditeur de liens.

La construction Windows a été vérifiée, mais la construction Windows reste à vérifier.
