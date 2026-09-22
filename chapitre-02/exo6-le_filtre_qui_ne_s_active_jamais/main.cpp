#include <iostream>

int main(){
    #ifdef FILTRE_SYSTEM
        std::cout << "Systeme Windows." << std::endl;
    #else
        std::cout << "Systeme Windows non trouvé." << std::endl;
    #endif

    return 0;
}