#include <cstdint>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // Lecture des paramètres
    std::uint64_t n = 0;
    std::uint64_t t = 0;
    long long x0 = 0;
    long long v = 0;

    if (!(std::cin >> n >> t >> x0 >> v)) {
        return 0;
    }

    // Vérification des valeurs reçues
    if (n == 0 || n > 4 || t == 0) {
        std::cout << "SAUTS AUCUN\n";
        return 0;
    }

    std::string mot;
    if (!(std::cin >> mot) || mot.size() != t) {
        std::cout << "SAUTS AUCUN\n";
        return 0;
    }

    // Stockage des trames dans les différents emplacements
    std::vector<std::uint64_t> emplacement(static_cast<std::size_t>(n), 0);
    std::uint64_t prochaine = 0;

    std::int64_t dernier_affichee = std::numeric_limits<std::int64_t>::min();
    std::int64_t min_saut = std::numeric_limits<std::int64_t>::max();
    std::int64_t max_saut = std::numeric_limits<std::int64_t>::min();
    bool aucun_saut = true;

    for (std::uint64_t k = 0; k < t; ++k) {
        std::uint64_t trame = k + 1;
        char mode = mot[k];

        // Écriture de la nouvelle trame dans l'emplacement disponible
        std::uint64_t index_emprunte = prochaine;
        emplacement[static_cast<std::size_t>(index_emprunte)] = trame;

        std::uint64_t index_soumis = 0;
        if (mode == 'P') {
            index_soumis = (prochaine + n - 1u) % n;
        }

        // Récupération de la trame qui sera affichée
        std::uint64_t f = emplacement[static_cast<std::size_t>(index_soumis)];
        long long position_affichee = (f == 0) ? 0 : (x0 + static_cast<long long>(v) * static_cast<long long>(f));

        if (mode == 'D') {
            index_soumis = (prochaine + n - 1u) % n;
        } else if (mode == 'A') {
            index_soumis = index_emprunte;
        }

        // Affichage et calcul des sauts entre les positions
        if (f == 0) {
            std::cout << trame << " NOIR -\n";
            dernier_affichee = std::numeric_limits<std::int64_t>::min();
            aucun_saut = false;
        } else {
            if (trame == 1) {
                std::cout << trame << ' ' << position_affichee << " - -\n";
                dernier_affichee = static_cast<std::int64_t>(position_affichee);
                aucun_saut = false;
            } else {
                std::int64_t saut = static_cast<std::int64_t>(position_affichee) - dernier_affichee;
                std::cout << trame << ' ' << position_affichee << " " << saut << " -\n";
                dernier_affichee = static_cast<std::int64_t>(position_affichee);

                if (saut < min_saut) min_saut = saut;
                if (saut > max_saut) max_saut = saut;

                aucun_saut = false;
            }
        }

        // Passage à l'emplacement suivant
        prochaine = (prochaine + 1u) % n;
    }

    // bilan final
    if (aucun_saut) {
        std::cout << "SAUTS AUCUN\n";
    } else {
        std::cout << "SAUT MIN " << min_saut << '\n';
        std::cout << "SAUT MAX " << max_saut << '\n';
        std::cout << ((min_saut != max_saut) ? "TREMBLE OUI" : "TREMBLE NON") << '\n';
    }

    return 0;
}