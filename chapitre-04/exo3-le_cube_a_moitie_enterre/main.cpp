#include <iostream>
#include <string>
#include <cstdlib>

int main() {
    int n;
    std::cin >> n;

    int aCorriger = 0;
    long long pire = 0;

    for (int i = 0; i < n; i++) {
        std::string nom;
        long long e, y;

        std::cin >> nom >> e >> y;

        long long demiHauteur = e / 2;
        long long bas = y - demiHauteur;
        long long haut = y + demiHauteur;

        std::string verdict;

        // L'ordre des tests est important
        if (haut <= 0) {
            verdict = "SOUS LE SOL";
        }
        else if (bas < 0) {
            verdict = "ENTERRE";
        }
        else if (bas == 0) {
            verdict = "POSE";
        }
        else {
            verdict = "FLOTTE";
        }

        std::cout << nom << " "
                  << bas << " "
                  << haut << " "
                  << verdict << " "
                  << demiHauteur << "\n";

        if (verdict != "POSE") {
            aCorriger++;
        }

        // Distance entre le bas du cube et le sol
        long long distance = std::llabs(bas);

        if (distance > pire) {
            pire = distance;
        }
    }

    std::cout << "A CORRIGER " << aCorriger << "\n";
    std::cout << "PIRE " << pire << "\n";

    return 0;
}