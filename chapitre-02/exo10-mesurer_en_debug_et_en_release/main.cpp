#include <chrono>
#include <cmath>
#include <iostream>

int main() {

#ifdef NDEBUG
    std::cout << "Configuration : Release" << std::endl;
#else
    std::cout << "Configuration : Debug" << std::endl;
#endif

    const int nbImages = 20;
    const int nbCalculs = 500000;

    double total = 0.0;
    double resultat = 0.0;

    for (int image = 0; image < nbImages; image++) {

        auto debut = std::chrono::steady_clock::now();

        for (int i = 0; i < nbCalculs; i++) {
            double x = i * 0.00001;
            resultat += std::sin(x) * std::cos(x);
        }

        auto fin = std::chrono::steady_clock::now();

        double temps = std::chrono::duration<double, std::milli>(
            fin - debut
        ).count();

        total += temps;
    }

    double moyenne = total / nbImages;

    std::cout << "Calculs par image : " << nbCalculs << std::endl;
    std::cout << "Nombre d'images   : " << nbImages << std::endl;
    std::cout << "Temps moyen       : " << moyenne << " ms" << std::endl;
    std::cout << "Resultat temoin   : " << resultat << std::endl;

    return 0;
}