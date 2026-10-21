#include "../include/pointeur.hpp"
#include "NKEvent/NkKeyboardEvent.h"
#include "NKEvent/NkWindowEvent.h"

namespace nkentseu
{
    bool pointeurPris = false;

    void PointeurPrendre(NkWindow &fenetre)
    {
        if (!pointeurPris)
        {
            fenetre.ShowMouse(false);
            fenetre.CaptureMouse(true);
            fenetre.ClipMouseToClient(true);
            pointeurPris = true;
        }
    }

    void PointeurRendre(NkWindow &fenetre)
    {
        if (pointeurPris)
        {
            fenetre.ShowMouse(true);
            fenetre.CaptureMouse(false);
            fenetre.ClipMouseToClient(false);
            pointeurPris = false;
        }
    }

    void PointeurTerminer(NkWindow &fenetre)
    {
        PointeurRendre(fenetre);
    }

    void PointeurTraiterEvenement(NkWindow &fenetre, NkEvent *ev)
    {
        if (ev == nullptr)
            return;

        if (auto touche = ev->As<NkKeyPressEvent>())
        {
            if (touche->GetKey() == NkKey::NK_ESCAPE)
                PointeurRendre(fenetre);
            else if (touche->GetKey() == NkKey::NK_ENTER)
                PointeurPrendre(fenetre);
        }

        if (ev->As<NkWindowCloseEvent>())
            PointeurTerminer(fenetre);
    }
}