#include <NKWindow/NKWindow.h>
#include <NKEvent/NkEvent.h>
#include <NKEvent/NkWindowEvent.h>
#include <NKEvent/NkKeyboardEvent.h>

int main(){
    // Configuration de la fenêtre
    nkentseu::NkWindowConfig config;

    config.title = "Ma salle";
    config.width = 1280;
    config.height = 720;

    // Création de la fenêtre avec la configuration définie
    nkentseu::NkWindow fenetre(config);

    // Vérification que la fenêtre a bien été créer
    if (!fenetre.IsValid()){
        return 1;
    }

    bool running = true;

    // Rappel appelé lorsque l'utilisateur ferme la fenêtre
    // bouton X
    nkentseu::NkEvents().AddEventCallback<nkentseu::NkWindowCloseEvent>(
        [&](nkentseu::NkWindowCloseEvent* event){
            running = false;
        });

    // Rappel appelé lorsqu'une touche du clavier est pressée, on verifie si c'est la touche Echap
    // touche Échap
    nkentseu::NkEvents().AddEventCallback<nkentseu::NkKeyPressEvent>(
        [&](nkentseu::NkKeyPressEvent* event){
            if (event->GetKey() == nkentseu::NkKey::NK_ESCAPE){
                running = false;
            }
        });
        

    while (running){
        nkentseu::NkEvents().PollEvents();
    }

    return 0;
}