#include <iostream>
#include <chrono>

int main() {
    const long long N = 1000000;
    volatile double resultat = 0.0;

    auto debut = std::chrono::high_resolution_clock::now();

    for (long long i = 0; i < N; i++) {
        resultat += i * 0.000001;
    }

    auto fin = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> duree = fin - debut;

    std::cout << "Resultat : " << resultat << std::endl;
    std::cout << "Temps d'excution : " << duree.count()
              << " ms" << std::endl;

    return 0;
}