#include <cstdint>
#include <iostream>
#include <string>

int main(){
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int nombreImages = 1;
    std::uint32_t valeurDepart = 0;
    int nombreOrdres = 0;

    if (!(std::cin >> nombreImages >> valeurDepart >> nombreOrdres)){
        return 0;
    }

    // Une variable par lettre de a à z, toutes initialisées à v
    std::uint32_t variables[26];

    for (std::uint32_t &variable : variables){
        variable = valeurDepart;
    }

    int prochaine = 0;
    bool empruntee = false;

    for (int i = 0; i < nombreOrdres; ++i){
        std::string ordre;

        if (!(std::cin >> ordre)){
            break;
        }

        if (ordre == "acquire"){
            char lettre = 'a';
            std::cin >> lettre;
            const int indice = lettre - 'a';

            if (empruntee){
                // la variable n'est pas touchée donc l'appel rend faux
                std::cout << "ACQUIRE FAUX " << lettre << ' ' << variables[indice] << '\n';
            } else{
                variables[indice] = static_cast<std::uint32_t>(prochaine);
                empruntee = true;
                std::cout << "ACQUIRE VRAI " << lettre << ' ' << variables[indice] << '\n';
            }
        } else if (ordre == "release"){
            if (!empruntee){
                std::cout << "RELEASE FAUX\n";
            } else{
                prochaine = (prochaine + 1) % nombreImages;
                empruntee = false;
                std::cout << "RELEASE VRAI\n";
            }
        } else if (ordre == "last"){
            std::cout << "LAST " << (prochaine + nombreImages - 1) % nombreImages << '\n';
        }
    }

    std::cout << "EMPRUNTEE " << (empruntee ? "OUI" : "NON") << '\n';
    std::cout << "PROCHAINE " << prochaine << '\n';

    return 0;
}
