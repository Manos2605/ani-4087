#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

struct Face {
    std::string nom;
    double x, y, z;
};

struct Soleil {
    std::string nom;
    double dx, dy, dz;
    int intensite;
};

int main() {
    int ambiante;
    int N;

    std::cin >> ambiante;
    std::cin >> N;

    std::vector<Soleil> soleils(N);

    for (auto& soleil : soleils) {
        std::cin >> soleil.nom
                 >> soleil.dx
                 >> soleil.dy
                 >> soleil.dz
                 >> soleil.intensite;
    }

    std::vector<Face> faces = {
        {"SOL", 0, 1, 0},
        {"FOND", 0, 0, 1},
        {"ENTREE", 0, 0, -1},
        {"GAUCHE", 1, 0, 0},
        {"DROIT", -1, 0, 0}
    };

    for (const auto& soleil : soleils) {
        double longueur = std::sqrt(
            soleil.dx * soleil.dx +
            soleil.dy * soleil.dy +
            soleil.dz * soleil.dz
        );

        std::vector<int> lumieres;

        for (const auto& face : faces) {
            double produit =
                face.x * soleil.dx +
                face.y * soleil.dy +
                face.z * soleil.dz;

            double c = -produit / longueur;

            int lumiere = static_cast<int>(
                std::round(
                    ambiante +
                    soleil.intensite * std::max(0.0, c)
                )
            );

            lumieres.push_back(lumiere);

            std::cout << soleil.nom << " "
                      << face.nom << " "
                      << lumiere << "\n";
        }

        int minimum = *std::min_element(lumieres.begin(), lumieres.end());
        int maximum = *std::max_element(lumieres.begin(), lumieres.end());

        std::cout << soleil.nom << " CONTRASTE "
                  << maximum - minimum << "\n";
    }

    return 0;
}