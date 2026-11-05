#include <iostream>
#include <string>
#include <cstdint>

int main() {
    std::uint64_t n;
    std::cin >> n;

    std::uint64_t poignee = 0;
    std::uint64_t prochaine = 101;
    std::uint64_t index = 0;

    int affichees = 0;
    int noires = 0;
    int perimees = 0;
    int numero = 0;

    for (int i = 0; i < n; i++) {
        std::string cmd;
        std::cin >> cmd;

        if (cmd == "cible") {
            int w, h;
            std::cin >> w >> h;

            poignee = prochaine;
            prochaine++;

            std::cout << "CIBLE " << poignee << " " << w << " " << h << '\n';
        }

        else if (cmd == "chaine") {
            index = 0;
        }

        else if (cmd == "enregistre") {
            index = poignee;
        }

        else if (cmd == "image") {
            numero++;

            if (index == 0) {
                std::cout << "IMAGE " << numero << " NOIR\n";
                noires++;
            }
            else if (index == poignee) {
                std::cout << "IMAGE " << numero << " AFFICHE " << index << '\n';
                affichees++;
            }
            else {
                std::cout << "IMAGE " << numero << " PERIMEE " << index << '\n';
                perimees++;
            }
        }
    }

    std::cout << "AFFICHEES " << affichees << '\n';
    std::cout << "NOIRES " << noires << '\n';
    std::cout << "PERIMEES " << perimees << '\n';

    return 0;
}