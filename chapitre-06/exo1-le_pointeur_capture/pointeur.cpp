#include "pointeur.hpp"

#include "NKEvent/NkKeyboardEvent.h"
#include "NKEvent/NkWindowEvent.h"

namespace nkentseu
{
    // Permet de savoir si le pointeur est deja cache
    int cachesEnCours = 0;
    bool pointeurConfine = false;

    // Cache le pointeur sans multiplier les appels
    void Cacher(NkWindow &fenetre)
    {
        if (cachesEnCours == 0)
        {
            fenetre.ShowMouse(false);
            cachesEnCours++;
        }
    }

    // Rend le pointeur visible
    void Montrer(NkWindow &fenetre)
    {
        while (cachesEnCours > 0)
        {
            fenetre.ShowMouse(true);
            cachesEnCours--;
        }
    }

    // Empeche le pointeur de sortir de la fenetre
    void Confiner(NkWindow &fenetre)
    {
        fenetre.ClipMouseToClient(true);
        pointeurConfine = true;
    }

    // Capture le pointeur et le cache
    void PointeurPrendre(NkWindow &fenetre)
    {
        Cacher(fenetre);
        fenetre.CaptureMouse(true);
        Confiner(fenetre);
    }

    // Libere le pointeur et le rend visible
    void PointeurRendre(NkWindow &fenetre)
    {
        Montrer(fenetre);
        fenetre.CaptureMouse(false);

        if (pointeurConfine)
        {
            fenetre.ClipMouseToClient(false);
            pointeurConfine = false;
        }
    }

    // Libere tout avant de terminer le programme
    void PointeurTerminer(NkWindow &fenetre)
    {
        PointeurRendre(fenetre);
    }

    // Gere les touches et les evenements de la fenetre
    void PointeurTraiterEvenement(NkWindow &fenetre, NkEvent *ev)
    {
        if (ev == nullptr)
            return;

        // Echap libere le pointeur et Entree le reprend
        if (NkKeyPressEvent *touche = ev->As<NkKeyPressEvent>())
        {
            if (touche->GetKey() == NkKey::NK_ESCAPE)
                PointeurRendre(fenetre);
            else if (touche->GetKey() == NkKey::NK_ENTER)
                PointeurPrendre(fenetre);

            return;
        }

        // Libere le pointeur quand on ferme la fenetre
        if (ev->As<NkWindowCloseEvent>() != nullptr)
        {
            PointeurRendre(fenetre);
            return;
        }

        // Replace la limite apres un deplacement ou un redimensionnement
        if (pointeurConfine &&
            (ev->As<NkWindowFocusGainedEvent>() != nullptr ||
             ev->As<NkWindowMoveEvent>() != nullptr ||
             ev->As<NkWindowResizeEvent>() != nullptr))
        {
            fenetre.ClipMouseToClient(true);
        }
    }
}