# Exercice 5: La taille qui change

## Énoncé

> Écoutez `NkWindowResizeEvent` et affichez la nouvelle taille dans la console à chaque changement.
>
> Redimensionnez lentement, puis d'un coup. Rendez les deux séries de nombres, et dites ce que vous en concluez sur le nombre d'événements reçus.

## [`main.cpp`](main.cpp)

```cpp
// Rappel appelé à chaque changement de taille de la fenêtre
nkentseu::NkEvents().AddEventCallback<nkentseu::NkWindowResizeEvent>(
    [](nkentseu::NkWindowResizeEvent* event){
        std::cout << "Nouvelle taille : "
                  << event->GetWidth() << " x "
                  << event->GetHeight() << std::endl;
    });
```

## Redimensionnement lent

J'ai commencé par redimensionner la fenêtre doucement. En faisant glisser progressivement le bord de la fenêtre, plusieurs changements de taille sont apparus dans la console.

J'ai remarqué que la taille changeait progressivement et que plusieurs événements étaient affichés pendant que je déplaçais le bord de la fenêtre.

## Redimensionnement d'un coup

Ensuite, j'ai redimensionné la fenêtre beaucoup plus rapidement. Cette fois, il y avait également plusieurs changements affichés, mais ils étaient beaucoup moins nombreux que lors du redimensionnement lent.

## Conclusion
Avec le redimensionnement lent, j'ai reçu beaucoup plus d'événements parce que la taille de la fenêtre changeait plusieurs fois pendant que je déplaçais doucement le bord.

Avec le redimensionnement rapide, j'ai aussi reçu plusieurs événements, mais moins nombreux.

Donc que `NkWindowResizeEvent` est envoyé à chaque changement de taille détecté par le programme. Le nombre d'événements reçus dépend donc de la manière dont la fenêtre est redimensionnée. Ce n'est pas forcément un seul événement pour une seule action de redimensionnement.
