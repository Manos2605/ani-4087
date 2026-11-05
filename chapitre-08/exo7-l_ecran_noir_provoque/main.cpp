#include <cstdint>
#include <iostream>
#include <string>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // Lecture du nombre d'ordres
    std::uint64_t ordres = 0;
    if (!(std::cin >> ordres)) {
        return 0;
    }

    std::uint64_t poignee_courante = 0;
    std::uint64_t prochaine_poignee = 101;
    std::uint64_t index_0 = 0;

    std::uint64_t image_k = 0;
    int affichees = 0;
    int noires = 0;
    int petimees = 0;

    for (std::uint64_t o = 0; o < ordres; ++o) {
        std::string cmd;
        if (!(std::cin >> cmd)) {
            break;
        }

        if (cmd == "cible") {
            std::uint64_t w = 0;
            std::uint64_t h = 0;
            std::cin >> w >> h;

            // Création d'une nouvelle cible avec une poignée unique
            poignee_courante = prochaine_poignee++;
            std::cout << "CIBLE " << poignee_courante << ' ' << w << ' ' << h << '\n';

        } else if (cmd == "chaine") {
            index_0 = 0;

        } else if (cmd == "enregistre") {
            // Conservation de la poignée de la cible actuelle
            index_0 = poignee_courante;

        } else if (cmd == "image") {
            ++image_k;

            // Vérification de l'état de la cible au moment de l'affichage
            if (index_0 == 0) {
                std::cout << "IMAGE " << image_k << " NOIR\n";
                ++noires;

            } else if (index_0 == poignee_courante) {
                std::cout << "IMAGE " << image_k << " AFFICHE " << index_0 << '\n';
                ++affichees;

            } else {
                std::cout << "IMAGE " << image_k << " PERIMEE " << index_0 << '\n';
                ++petimees;
            }
        }
    }

    // bilan final
    std::cout << "AFFICHEES " << affichees << '\n';
    std::cout << "NOIRES " << noires << '\n';
    std::cout << "PERIMEES " << petimees << '\n';

    return 0;
}