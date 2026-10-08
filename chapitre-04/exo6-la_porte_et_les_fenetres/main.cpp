#include <iostream>
#include <string>

int main() {
    long long W, H, seuil;
    std::cin >> W >> H >> seuil;

    int n;
    std::cin >> n;

    int ok = 0;
    int aReprendre = 0;

    for (int i = 0; i < n; i++) {
        std::string nom;
        long long u, y, l, h, e, d;

        std::cin >> nom >> u >> y >> l >> h >> e >> d;

        long long gauche = u - l / 2;
        long long droite = u + l / 2;
        long long bas = y - h / 2;
        long long haut = y + h / 2;

        long long saillie = d + e / 2;
        long long arriere = d - e / 2;

        std::string verdict;

        // Vérifier si le panneau sort du mur
        if (gauche < -W / 2 ||
            droite > W / 2 ||
            bas < 0 ||
            haut > H) {
            verdict = "DEBORDE";
        }
        else if (saillie <= 0) {
            verdict = "INVISIBLE";
        }
        else if (saillie < seuil) {
            verdict = "CLIGNOTE";
        }
        else if (arriere > seuil) {
            verdict = "DECOLLE";
        }
        else {
            verdict = "OK";
        }

        std::cout << nom << " "
                  << saillie << " "
                  << verdict << "\n";

        if (verdict == "OK") {
            ok++;
        }
        else {
            aReprendre++;
        }
    }

    std::cout << "OK " << ok << "\n";
    std::cout << "A REPRENDRE " << aReprendre << "\n";

    return 0;
}