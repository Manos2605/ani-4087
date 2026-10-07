#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <unordered_map>

int main() {
    int n;
    std::cin >> n;

    std::unordered_map<std::string, std::string> lisible = {
        {"VULKAN", "Vulkan"},
        {"DX12", "DirectX 12"},
        {"DX11", "DirectX 11"},
        {"OPENGL", "OpenGL"},
        {"METAL", "Metal"},
        {"SOFTWARE", "Software"}
    };

    int ignorees = 0;
    int logiciel = 0;
    std::set<std::string> differentes;

    for (int i = 0; i < n; ++i) {
        std::string nom;
        std::string plateforme;
        int k;

        std::cin >> nom >> plateforme >> k;

        std::set<std::string> apis;

        for (int j = 0; j < k; ++j) {
            std::string api;
            std::cin >> api;
            apis.insert(api);
        }

        std::vector<std::string> ordre;

        if (plateforme == "WINDOWS") {
            ordre = {"VULKAN", "DX12", "DX11", "OPENGL"};
        }
        else if (plateforme == "MACOS") {
            ordre = {"METAL", "OPENGL"};
        }
        else if (plateforme == "IOS") {
            ordre = {"METAL"};
        }
        else {
            ordre = {"VULKAN", "OPENGL"};
        }

        // Compter les interfaces présentes mais jamais essayées.
        for (const std::string& api : apis) {
            bool dansOrdre = false;

            for (const std::string& candidate : ordre) {
                if (candidate == api) {
                    dansOrdre = true;
                    break;
                }
            }

            if (!dansOrdre) {
                ignorees++;
            }
        }

        // Choisir la première interface disponible.
        std::string choix = "SOFTWARE";

        for (const std::string& candidate : ordre) {
            if (apis.count(candidate)) {
                choix = candidate;
                break;
            }
        }

        std::string nomLisible = lisible[choix];

        std::cout << nom << " " << nomLisible << "\n";

        if (choix == "SOFTWARE") {
            logiciel++;
        }

        differentes.insert(nomLisible);
    }

    std::cout << "IGNOREES " << ignorees << "\n";
    std::cout << "LOGICIEL " << logiciel << "\n";
    std::cout << "DIFFERENTES " << differentes.size() << "\n";

    return 0;
}