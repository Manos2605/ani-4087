#include "../include/accumulation.hpp"

#include "NKEvent/NkMouseEvent.h"
#include "NKLogger/NkLog.h"

namespace nkentseu
{
    /*
    Symptôme : le lacet peut continuer à évoluer après l'arrêt de la souris.
    Cause : les événements bruts arrivent indépendamment du rythme des images.
    Preuve : sans vidage, le déplacement accumulé est réutilisé à chaque image.
    Correction : accumuler les événements, lire le total une fois par image, puis le vider.
    */

    Accumulation::Accumulation()
        : sensibilite(0.002f),
          lacetLu(0.0f),
          lacetVide(0.0f),
          lacetSansVidage(0.0f),
          accumulationVideX(0),
          accumulationVideY(0),
          accumulationSansVidageX(0),
          accumulationSansVidageY(0),
          images(0),
          derniereSeconde(-1)
    {
    }

    void Accumulation::TraiterEvenement(NkEvent *ev)
    {
        if (ev == nullptr)
            return;

        if (ev->As<NkMouseRawEvent>())
        {
            NkMouseRawEvent *souris = ev->As<NkMouseRawEvent>();

            const int dx = souris->GetDeltaX();
            const int dy = souris->GetDeltaY();

            // Les deux accumulateurs reçoivent les mêmes événements.
            accumulationVideX += dx;
            accumulationVideY += dy;

            accumulationSansVidageX += dx;
            accumulationSansVidageY += dy;
        }
    }

    void Accumulation::MettreAJour()
    {
        // Version 1 : lecture directe du module d'entrées.
        const int brutX = NkInput.MouseRawDeltaX();
        lacetLu -= brutX * sensibilite;

        // Version 2 : lire l'accumulation une fois par image.
        const int deltaVideX = accumulationVideX;
        lacetVide -= deltaVideX * sensibilite;

        // Version 3 : réutiliser l'accumulation sans jamais la vider.
        lacetSansVidage -= accumulationSansVidageX * sensibilite;

        // Vider uniquement l'accumulateur de la version 2.
        accumulationVideX = 0;
        accumulationVideY = 0;

        ++images;
    }

    void Accumulation::AfficherMesure()
    {
        const float secondes =
            chrono.Elapsed().ToMilliseconds() / 1000.0f;

        const int seconde = static_cast<int>(secondes);

        if (seconde > derniereSeconde)
        {
            derniereSeconde = seconde;

            logger.Info(
                "t = {0:.1} s, lu = {1:.1}, vide = {2:.1}, sans vidage = {3:.1}",
                secondes,
                lacetLu * 180.0f / 3.14159265f,
                lacetVide * 180.0f / 3.14159265f,
                lacetSansVidage * 180.0f / 3.14159265f);
        }
    }

    float Accumulation::GetLacetLu() const
    {
        return lacetLu;
    }

    float Accumulation::GetLacetVide() const
    {
        return lacetVide;
    }

    float Accumulation::GetLacetSansVidage() const
    {
        return lacetSansVidage;
    }

}
