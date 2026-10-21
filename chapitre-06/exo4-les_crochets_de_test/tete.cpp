#include "../include/tete.hpp"

#include <cmath>
#include <cstdio>

namespace {
    constexpr float PI = 3.14159265358979323846f;
    constexpr float TOLERANCE = 0.001f;

    bool Proche(float a, float b) {
        return std::fabs(a - b) <= TOLERANCE;
    }

    bool Tester(const char* nom, Tete& tete,
                float lacet, float tangage,
                const Vecteur3& position,
                const Vecteur3& attendu) {
        tete.PoserTete(lacet, tangage, position);
        const Vecteur3 obtenu = tete.Regard();

        const bool ok =
            Proche(obtenu.x, attendu.x) &&
            Proche(obtenu.y, attendu.y) &&
            Proche(obtenu.z, attendu.z);

        std::printf(
            "%s : attendu (%.3f, %.3f, %.3f), "
            "obtenu (%.3f, %.3f, %.3f), %s\n",
            nom,
            attendu.x, attendu.y, attendu.z,
            obtenu.x, obtenu.y, obtenu.z,
            ok ? "OK" : "ECHEC"
        );

        return ok;
    }
}

Tete::Tete()
    : lacet(0.0f),
      tangage(0.0f),
      position{0.0f, 0.0f, 0.0f}
{
}

void Tete::PoserTete(float lacetRad, float tangageRad,
                     const Vecteur3& nouvellePosition) {
    lacet = lacetRad;
    tangage = tangageRad;
    position = nouvellePosition;
}

Vecteur3 Tete::Regard() const {
    return {
        std::sin(lacet) * std::cos(tangage),
        std::sin(tangage),
        -std::cos(lacet) * std::cos(tangage)
    };
}

int ExecuterTestsTete() {
    Tete tete;
    const Vecteur3 position = {1.0f, 2.0f, 3.0f};
    int echecs = 0;

    // Attendu déduit de la convention avant = -Z, haut = +Y.
    if (!Tester("lacet 90, tangage 0",
                tete, PI / 2.0f, 0.0f, position,
                {-1.0f, 0.0f, 0.0f}))
        ++echecs;

    if (!Tester("lacet 0, tangage 90",
                tete, 0.0f, PI / 2.0f, position,
                {0.0f, 1.0f, 0.0f}))
        ++echecs;

    if (!Tester("lacet 45, tangage 30",
                tete, PI / 4.0f, PI / 6.0f, position,
                {-0.612372f, 0.500000f, -0.612372f}))
        ++echecs;

    std::printf("%s\n",
                echecs == 0 ? "TOUS LES TESTS REUSSIS" : "ECHEC");

    return echecs == 0 ? 0 : 1;
}