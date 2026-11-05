#include <iostream>
#include <vector>
#include <unordered_set>
#include <string>
#include <cstdint>

int main() {
    std::uint64_t n, ordres;
    std::cin >> n >> ordres;

    std::vector<std::uint64_t> chaine(n, 0);
    std::unordered_set<std::uint64_t> textures;

    int mortes = 0;
    int larges = 0;

    for (int o = 0; o < ordres; o++) {
        std::string cmd;
        std::cin >> cmd;

        if (cmd == "cree") {
            std::uint64_t h;
            std::cin >> h;
            textures.insert(h);
        }

        else if (cmd == "detruit") {
            std::uint64_t h;
            std::cin >> h;
            textures.erase(h);
        }

        else if (cmd == "set") {
            std::uint64_t i, h;
            std::cin >> i >> h;

            if (i >= n) {
                std::cout << "SET IGNORE " << i << '\n';
            }
            else {
                chaine[i] = h;

                if (h > 4294967295ULL) {
                    larges++;
                }
            }
        }

        else if (cmd == "get") {
            std::uint64_t i;
            std::cin >> i;

            std::uint64_t h = 0;

            if (i < n) {
                h = chaine[i];
            }

            if (h == 0) {
                std::cout << "GET " << i << " 0 VIDE\n";
            }
            else if (textures.count(h) > 0) {
                std::cout << "GET " << i << " " << h << " VIVANTE\n";
            }
            else {
                std::cout << "GET " << i << " " << h << " MORTE\n";
                mortes++;
            }
        }
    }

    std::cout << "MORTES " << mortes << '\n';
    std::cout << "LARGES " << larges << '\n';

    return 0;
}