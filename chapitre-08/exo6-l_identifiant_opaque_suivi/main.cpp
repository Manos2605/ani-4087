#include <cstdint>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // Lecture du nombre de cases et du nombre d'ordres
    std::uint64_t n = 0;
    std::uint64_t ordres = 0;

    if (!(std::cin >> n >> ordres)) {
        return 0;
    }

    // Tableau contenant les handles
    std::vector<std::uint64_t> chaine(static_cast<std::size_t>(n), 0);

    // Ensemble des handles actuellement existants
    std::unordered_set<std::uint64_t> existantes;

    int mortes = 0;
    int larges = 0;

    for (std::uint64_t o = 0; o < ordres; ++o) {
        std::string cmd;

        if (!(std::cin >> cmd)) {
            break;
        }

        if (cmd == "cree") {
            std::uint64_t h = 0;
            std::cin >> h;
            existantes.insert(h);

        } else if (cmd == "detruit") {
            std::uint64_t h = 0;
            std::cin >> h;
            existantes.erase(h);

        } else if (cmd == "set") {
            std::uint64_t i = 0;
            std::uint64_t h = 0;
            std::cin >> i >> h;

            // Vérification de l'indice avant de modifier le tableau
            if (i >= n) {
                std::cout << "SET IGNORE " << i << '\n';
            } else{
                chaine[static_cast<std::size_t>(i)] = h;

                // Comptage des handles qui dépassent 32 bits
                if (h > 4294967295u) {
                    ++larges;
                }
            }

        } else if (cmd == "get") {
            std::uint64_t i = 0;
            std::cin >> i;
            std::uint64_t h = 0;

            // Récupération de la valeur à l'indice demandé
            if (i >= n) {
                h = 0;
            } else{
                h = chaine[static_cast<std::size_t>(i)];
            }

            if (h == 0) {
                std::cout << "GET " << i << " 0 VIDE\n";

            } else if (existantes.count(h)) {
                std::cout << "GET " << i << ' ' << h << " VIVANTE\n";

            } else{
                std::cout << "GET " << i << ' ' << h << " MORTE\n";
                ++mortes;
            }
        }
    }

    // Bilan final
    std::cout << "MORTES " << mortes << '\n';
    std::cout << "LARGES " << larges << '\n';

    return 0;
}