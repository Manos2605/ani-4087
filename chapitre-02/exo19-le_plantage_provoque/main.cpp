#include <iostream>

int main() {
    std::cout << "Demarrage du programme" << std::endl;

    int* pointeur = nullptr;

    std::cout << "Avant le plantage" << std::endl;

    // pointeur nul
    *pointeur = 42;

    return 0;
}