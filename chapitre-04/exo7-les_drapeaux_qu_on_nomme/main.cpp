#include <iostream>
#include <string>
#include <unordered_map>
#include <cstdint>
#include <iomanip>

int main() {
    int n;
    std::cin >> n;

    std::unordered_map<std::string, uint32_t> drapeaux = {
        {"RENDER2D", 1},
        {"RENDER3D", 2},
        {"TEXT", 4},
        {"UI", 8},
        {"SHADOW", 16},
        {"POST_PROCESS", 32},
        {"VFX", 64},
        {"ANIMATION", 128},
        {"OVERLAY", 256},
        {"SIMULATION", 512},
        {"OFFSCREEN", 1024},
        {"RAYTRACING", 2048},
        {"GPU_CULLING", 4096},
        {"NONE", 0},
        {"2D_ESSENTIALS", 1 | 4},
        {"3D_BASE", 2 | 16 | 32},
        {"DEBUG", 256 | 512},
        {"ALL", 4294967295u}
    };

    uint32_t valeur = 0;

    if (n == 0) {
        valeur = 4294967295u;
    }

    for (int i = 0; i < n; i++) {
        std::string nom;
        std::cin >> nom;

        auto it = drapeaux.find(nom);

        if (it == drapeaux.end()) {
            std::cout << "INCONNU " << nom << "\n";
        }
        else {
            valeur |= it->second;
        }
    }

    std::cout << "VALEUR " << valeur << "\n";

    std::cout << "HEXA 0x"
              << std::uppercase
              << std::hex
              << std::setfill('0')
              << std::setw(8)
              << valeur
              << std::dec << "\n";

    // Vérifier les dépendances
    if ((valeur & 4) && !(valeur & 1)) {
        std::cout << "MANQUE TEXT RENDER2D\n";
    }

    if (valeur & 8) {
        if (!(valeur & 1)) {
            std::cout << "MANQUE UI RENDER2D\n";
        }

        if (!(valeur & 4)) {
            std::cout << "MANQUE UI TEXT\n";
        }
    }

    if ((valeur & 16) && !(valeur & 2)) {
        std::cout << "MANQUE SHADOW RENDER3D\n";
    }

    if (valeur & 256) {
        if (!(valeur & 1)) {
            std::cout << "MANQUE OVERLAY RENDER2D\n";
        }

        if (!(valeur & 4)) {
            std::cout << "MANQUE OVERLAY TEXT\n";
        }
    }

    int allumes = 0;

    uint32_t simples[] = {
        1, 2, 4, 8, 16, 32, 64,
        128, 256, 512, 1024, 2048, 4096
    };

    for (uint32_t drapeau : simples) {
        if (valeur & drapeau) {
            allumes++;
        }
    }

    std::cout << "ALLUMES " << allumes << "\n";
    std::cout << "ETEINTS " << 13 - allumes << "\n";

    return 0;
}