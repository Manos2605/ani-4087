#include <iostream>
#include <string>
#include <cstdlib>
#include <algorithm>

int main() {
    int n;
    std::cin >> n;

    int deplaces = 0;
    long long pire = 0;

    for (int i = 0; i < n; i++) {
        std::string nom;
        long long tx, ty, tz;
        long long sx, sy, sz;

        std::cin >> nom >> tx >> ty >> tz >> sx >> sy >> sz;

        // Mauvais ordre : l'échelle agit aussi sur la translation
        long long x = sx * tx / 1000;
        long long y = sy * ty / 1000;
        long long z = sz * tz / 1000;

        long long ecartX = std::llabs(tx - x);
        long long ecartY = std::llabs(ty - y);
        long long ecartZ = std::llabs(tz - z);

        long long ecart = std::max({ecartX, ecartY, ecartZ});

        std::cout << nom << " "
                  << x << " "
                  << y << " "
                  << z << " "
                  << ecart << "\n";

        if (ecart != 0) {
            deplaces++;
        }

        if (ecart > pire) {
            pire = ecart;
        }
    }

    std::cout << "DEPLACES " << deplaces << "\n";
    std::cout << "PIRE " << pire << "\n";

    return 0;
}