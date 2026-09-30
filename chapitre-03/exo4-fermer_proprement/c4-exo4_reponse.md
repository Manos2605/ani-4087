# Exercice 4 : Fermer proprement

## Énoncé

> Ajoutez un rappel sur `NkWindowCloseEvent` qui met un booléen à faux, et faites porter la boucle sur ce booléen plutôt que sur `IsOpen()`.
>
> Ajoutez ensuite un rappel sur `NkKeyPressEvent` qui fait la même chose sur la touche Échap.
>
> Rendez le code et expliquez pourquoi les deux chemins de sortie doivent aboutir au même endroit.

## Solution

Pour cet exercice, j'ai modifié la boucle principale afin qu'elle ne dépende plus directement de `fenetre.IsOpen()`.

J'ai créé un booléen `running` qui indique si le programme doit continuer à fonctionner. Au départ, sa valeur est `true`.

Un rappel sur `NkWindowCloseEvent` permet de mettre `running` à `false` lorsque l'utilisateur ferme la fenêtre.

J'ai également ajouté un rappel sur `NkKeyPressEvent` qui met le même booléen à `false` lorsque la touche **Échap** est pressée.

### Programme [`main.cpp`](main.cpp)

```cpp
bool running = true;

// Rappel appelé lorsque l'utilisateur ferme la fenêtre
nkentseu::NkEvents().AddEventCallback<nkentseu::NkWindowCloseEvent>(
    [&](nkentseu::NkWindowCloseEvent* event){
        running = false;
    });

// Rappel appelé lorsqu'une touche du clavier est pressée, on verifie si c'est la touche Echap
nkentseu::NkEvents().AddEventCallback<nkentseu::NkKeyPressEvent>(
    [&](nkentseu::NkKeyPressEvent* event){
        if (event->GetKey() == nkentseu::NkKey::NK_ESCAPE){
            running = false;
        }
    });
```

### Fonctionnement

La boucle fonctionne avec le booléen `running`, lorsque l'utilisateur ferme la fenêtre, `running` devient `false`.

De la même manière, lorsqu'il appuie sur la touche **Échap**, `running` devient également `false`.

## Pourquoi les deux chemins doivent aboutir au même endroit ?

Les deux actions ont le même objectif : **arrêter proprement le programme**.

Que l'utilisateur ferme la fenêtre avec le bouton de fermeture ou qu'il appuie sur **Échap**, le résultat doit donc être le même : le booléen `running` passe à `false`, la boucle s'arrête et le programme arrive ensuite au même `return 0`.

Cela évite d'avoir deux façons différentes de quitter le programme et permet de gérer les deux événements avec une seule condition de sortie.
