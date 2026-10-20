#include "../include/derive.hpp"
#include "NKEvent/NkEventDispatcher.h"
#include "NKLogger/NkLog.h"

namespace nkentseu
{
    Derive::Derive()
        : lacet(0.0f),
          tangage(0.0f),
          sensibilite(0.002f),
          images(0),
          derniereSeconde(-1),
          dernierBrutX(0)
    {
    }

    void Derive::MettreAJour()
    {
        dernierBrutX = NkInput.MouseRawDeltaX();
        const int brutY = NkInput.MouseRawDeltaY();

        // Correction du sens des mouvements de la souris.
        lacet -= dernierBrutX * sensibilite;
        tangage -= brutY * sensibilite;

        ++images;
    }

    void Derive::AfficherMesure()
    {
        const float secondes =
            chrono.Elapsed().ToMilliseconds() / 1000.0f;

        const int seconde = static_cast<int>(secondes);

        if (seconde > derniereSeconde)
        {
            derniereSeconde = seconde;

            logger.Info(
                "t = {0:.1} s, images = {1}, brut = {2}, lacet = {3:.1} deg",
                secondes,
                images,
                dernierBrutX,
                lacet * 180.0f / 3.14159265f);
        }
    }

    float Derive::GetLacet() const
    {
        return lacet;
    }

    float Derive::GetTangage() const
    {
        return tangage;
    }
}