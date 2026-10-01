#include <NKWindow/NKWindow.h>
#include <NKEvent/NkEvent.h>
#include <NKEvent/NkWindowEvent.h>
#include <NKEvent/NkKeyboardEvent.h>
#include <NKEvent/NkEventDispatcher.h>
#include <NKTime/NkClock.h>

int main(){
    // Configuration de la fenêtre
    nkentseu::NkWindowConfig config;

    config.title = "Ma salle";
    config.width = 1280;
    config.height = 720;

    nkentseu::NkInitialise();

    // Création de la fenêtre avec la configuration définie
    nkentseu::NkWindow fenetre(config);

    // Vérification que la fenêtre a bien été créer
    if (!fenetre.IsValid()){
        return 1;
    }

    bool running = true;
    int compteurEtat = 0;
    int compteurEvenement = 0;

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

            // Compteur pour la touche espace
            if (event->GetKey() == nkentseu::NkKey::NK_SPACE){
                compteurEvenement++;
            }
        });

    while (running){
        nkentseu::NkEvents().PollEvents();

        if (nkentseu::NkInput.IsKeyDown(nkentseu::NkKey::NK_SPACE)){
            compteurEtat++;
        }

        nkentseu::NkClock::Sleep((nkentseu::int64)10);
    }

    std::cout << "Compteur avec IsKeyDown : "<< compteurEtat << std::endl;
    std::cout << "Compteur avec NkKeyPressEvent : "<< compteurEvenement << std::endl;

    return 0;
}