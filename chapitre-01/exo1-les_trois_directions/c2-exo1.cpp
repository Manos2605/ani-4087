#include <iomanip>
#include <iostream>

// Vecteur 3D
struct Vecteur {
    double x;
    double y;
    double z;
};

// axe z negatif
Vecteur Avant() {
    return {0.0, 0.0, -1.0};
}

// axe y positif
Vecteur Haut() {
    return {0.0, 1.0, 0.0};
}

// axe x positif
Vecteur Droite() {
    return {1.0, 0.0, 0.0};
}

// Produit scalaire
double produitScalaire(Vecteur gauche, Vecteur droite) {
    return gauche.x * droite.x + gauche.y * droite.y + gauche.z * droite.z;
}

int main() {
    Vecteur point{};

    // composantes du point depuis l'entree
    std::cin >> point.x >> point.y >> point.z;

    // composantes dans les directions Avant, Haut et Droite.
    std::cout << std::fixed << std::setprecision(4)
              << produitScalaire(point, Avant()) << '\n'
              << produitScalaire(point, Haut()) << '\n'
              << produitScalaire(point, Droite()) << '\n';
}
