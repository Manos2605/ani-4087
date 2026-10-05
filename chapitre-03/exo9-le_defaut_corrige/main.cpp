#include <NKWindow/NKWindow.h>
#include <NKEvent/NkEvent.h>
#include <NKEvent/NkWindowEvent.h>
#include <NKEvent/NkKeyboardEvent.h>
#include <NKEvent/NkMouseEvent.h>

#include <iostream>

int main(){
    // Configuration de la fenêtre
    nkentseu::NkWindowConfig config;

    config.title = "Ma salle";
    config.width = 1280;
    config.height = 720;

    // Création de la fenêtre
    nkentseu::NkWindow fenetre(config);

    if (!fenetre.IsValid()){
        return 1;
    }

    bool running = true;

    int rawDeltaXAccumule = 0;
    int deltaXPrecedent = 0;

    // Fermer la fenêtre
    nkentseu::NkEvents().AddEventCallback<nkentseu::NkWindowCloseEvent>(
        [&](nkentseu::NkWindowCloseEvent* event){
            running = false;
        });

    // Échap pour quitter
    nkentseu::NkEvents().AddEventCallback<nkentseu::NkKeyPressEvent>(
        [&](nkentseu::NkKeyPressEvent* event){
            if (event->GetKey() == nkentseu::NkKey::NK_ESCAPE){
                running = false;
            }
        });

    // Accumulation du déplacement brut
    nkentseu::NkEvents().AddEventCallback<nkentseu::NkMouseRawEvent>(
        [&](nkentseu::NkMouseRawEvent* event){
            rawDeltaXAccumule += event->GetDeltaX();
        });

    // Mesure de l'exercice précédent
    nkentseu::NkEvents().AddEventCallback<nkentseu::NkMouseMoveEvent>(
        [&](nkentseu::NkMouseMoveEvent* event){
            deltaXPrecedent = event->GetDeltaX();
        });

    while (running){
        nkentseu::NkEvents().PollEvents();

        std::cout
            << "accumule = " << rawDeltaXAccumule
            << " | precedent = " << deltaXPrecedent
            << std::endl;

        rawDeltaXAccumule = 0;
    }

    return 0;
}