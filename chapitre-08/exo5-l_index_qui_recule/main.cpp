#include <cstdint>
#include <iostream>

int main(){
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int nombreEtats = 0;

    if (!(std::cin >> nombreEtats)){
        return 0;
    }

    int refusees = 0;
    int fauxSigne = 0;
    int fauxNonSigne = 0;

    for (int i = 0; i < nombreEtats; ++i){
        std::uint64_t nombreImages = 0;
        std::uint64_t liberations = 0;

        if (!(std::cin >> nombreImages >> liberations)){
            break;
        }

        if (nombreImages == 0 || nombreImages > 4){
            std::cout << nombreImages << ' ' << liberations << " REFUSEE\n";
            ++refusees;
            continue;
        }

        // On vérifie si r peut dépasser un int
        const std::uint64_t prochaine = liberations % nombreImages;
        const std::uint64_t juste = (prochaine + nombreImages - 1) % nombreImages;

        // -1 % 3 vaut -1
        const int signe = (static_cast<int>(prochaine) - 1) % static_cast<int>(nombreImages);

        // 0 - 1 vaut 4294967295
        const std::uint32_t nonsigne =
            (static_cast<std::uint32_t>(prochaine) - 1u) % static_cast<std::uint32_t>(nombreImages);

        std::cout << nombreImages << ' ' << liberations << ' ' << prochaine << ' ' << juste
                  << ' ' << signe << ' ' << nonsigne << '\n';

        if (signe != static_cast<int>(juste)){
            ++fauxSigne;
        }

        if (nonsigne != juste){
            ++fauxNonSigne;
        }
    }

    std::cout << "REFUSEES " << refusees << '\n';
    std::cout << "FAUX SIGNE " << fauxSigne << '\n';
    std::cout << "FAUX NON SIGNE " << fauxNonSigne << '\n';

    return 0;
}
