#include <algorithm>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

uint32_t drapeau(const std::string& nom) {
    if (nom == "RENDER2D") return 1;
    if (nom == "RENDER3D") return 2;
    if (nom == "TEXT") return 4;
    if (nom == "UI") return 8;
    if (nom == "SHADOW") return 16;
    if (nom == "POST_PROCESS") return 32;
    if (nom == "ALL") return 4294967295u;

    return 0;
}

long long mediane(std::vector<long long> temps) {
    std::sort(temps.begin(), temps.end());

    return (temps[4] + temps[5]) / 2;
}

long long moyenne(const std::vector<long long>& temps) {
    long long somme = 0;

    for (long long tempsMs : temps) {
        somme += tempsMs;
    }

    return somme / 10;
}

int main() {
    uint32_t valeurs[2];
    long long medianes[2];
    long long moyennes[2];

    for (int configuration = 0; configuration < 2; configuration++) {
        std::string nom;
        int k;

        std::cin >> nom >> k;

        uint32_t valeur = 0;

        for (int i = 0; i < k; i++) {
            std::string nomDrapeau;
            std::cin >> nomDrapeau;

            valeur |= drapeau(nomDrapeau);
        }

        std::vector<long long> temps(10);

        for (long long& tempsMs : temps) {
            std::cin >> tempsMs;
        }

        valeurs[configuration] = valeur;
        medianes[configuration] = mediane(temps);
        moyennes[configuration] = moyenne(temps);

        std::cout << nom << " VALEUR " << valeur << "\n";
        std::cout << nom << " MEDIANE " << medianes[configuration] << "\n";
        std::cout << nom << " MOYENNE " << moyennes[configuration] << "\n";
    }

    std::cout << "ECART MEDIANES "
              << medianes[0] - medianes[1] << "\n";

    std::cout << "ECART MOYENNES "
              << moyennes[0] - moyennes[1] << "\n";

    return 0;
}