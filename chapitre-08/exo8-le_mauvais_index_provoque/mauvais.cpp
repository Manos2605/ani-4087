#include <iostream>
#include <string>
#include <vector>

int main() {
    int n, T;
    long long x0, v;

    std::cin >> n >> T >> x0 >> v;

    std::string modes;
    std::cin >> modes;

    std::vector<int> images(n, 0);

    int prochaine = 0;
    long long derniere_position = 0;
    bool premiere = true;

    long long min_saut = 0;
    long long max_saut = 0;
    bool premier_saut = true;

    for (int t = 1; t <= T; t++) {

        // On peint la prochaine image
        int empruntee = prochaine;
        images[empruntee] = t;

        int index;

        if (modes[t - 1] == 'P') {
            index = (prochaine + n - 1) % n;
        }

        // On libère l'image et on passe à la suivante
        prochaine = (prochaine + 1) % n;

        if (modes[t - 1] == 'D') {
            index = (prochaine + n - 1) % n;
        }
        else if (modes[t - 1] == 'A') {
            index = empruntee;
        }

        int f = images[index];

        if (f == 0) {
            std::cout << t << " NOIR -\n";
            premiere = true;
        }
        else {
            long long position = x0 + v * f;

            if (premiere) {
                std::cout << t << " " << position << " -\n";
                premiere = false;
            }
            else {
                long long saut = position - derniere_position;

                std::cout << t << " " << position << " " << saut << '\n';

                if (premier_saut) {
                    min_saut = saut;
                    max_saut = saut;
                    premier_saut = false;
                }
                else {
                    if (saut < min_saut) {
                        min_saut = saut;
                    }

                    if (saut > max_saut) {
                        max_saut = saut;
                    }
                }
            }

            derniere_position = position;
        }
    }

    if (premier_saut) {
        std::cout << "SAUTS AUCUN\n";
    }
    else {
        std::cout << "SAUT MIN " << min_saut << '\n';
        std::cout << "SAUT MAX " << max_saut << '\n';

        if (min_saut != max_saut) {
            std::cout << "TREMBLE OUI\n";
        }
        else {
            std::cout << "TREMBLE NON\n";
        }
    }

    return 0;
}