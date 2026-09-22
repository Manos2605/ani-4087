# Exercice 8 
## Enoncer
Écrivez les filtres pour Windows et pour Linux dans votre projet, chacun avec ses bibliothèques. Faites vérifier par un camarade sur l'autre système que le projet se construit chez lui sans modification.

Si vous n'avez accès qu'à un système, écrivez quand même les deux et dites ce que vous n'avez pas pu vérifier.

## Solution

Dans mon fichier [main.cpp](main.cpp), j'ai utilisé des filtres pour différencier Windows et Linux.

J'ai utilisé les filtres dans le main() pour afficher le système sur lequel le programme est exécuté :

```text
#ifdef _WIN32
    std::cout << "Système : Windows" << std::endl;
#elif defined(__linux__)
    std::cout << "System : Linux" << std::endl;
#else
    std::cout << "System : Autre que Windows et Linux" << std::endl;
#endif
```

L'idée est que le compilateur prend automatiquement la partie correspondant au système utilisé. Ainsi, avec Windows, _WIN32 est défini, tandis que sous Linux, __linux__ est défini.

Le fichier `salle.jenga` contient les cibles :

* Windows
* Linux
* macOS

Pour cet exercice, les filtres permettent de sélectionner les éléments nécessaires selon le système utilisé.

## Vérification

J'ai vérifié le projet sur les deux systèmes.

- Sur **Windows**, le projet se construit correctement avec :

```text
jenga build
```

Sur **Ubuntu 24.04.3 LTS**, le même projet se construit également sans modification avec :

```text
jenga build
```

La compilation sous Ubuntu utilise la toolchain `host-gcc` pour la cible Linux x86_64.

Le projet a donc été vérifié sur Windows et Linux avec les mêmes fichiers.

## Remarque

Le fait d'utiliser des filtres permet de conserver un seul projet tout en utilisant les bibliothèques adaptées au système d'exploitation sur lequel le projet est compilé.

## Conclusion

Le projet fonctionne sur Windows et Linux sans avoir besoin de modifier les fichiers entre les deux systèmes.
