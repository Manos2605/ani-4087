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

    // Déplacement de la souris
    nkentseu::NkEvents().AddEventCallback<nkentseu::NkMouseMoveEvent>(
        [&](nkentseu::NkMouseMoveEvent* event){
            std::cout
                << "rawDeltaX = " << event->GetDeltaX()
                << std::endl;
        });

    while (running){
        nkentseu::NkEvents().PollEvents();
    }

    return 0;
}