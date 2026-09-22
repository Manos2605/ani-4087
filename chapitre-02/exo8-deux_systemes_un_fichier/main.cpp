#include <iostream>

#ifdef _WIN32
    #include <windows.h>
#elif defined(__linux__)
    #include <unistd.h>
#endif

int main(){
#ifdef _WIN32
    std::cout << "Système : Windows" << std::endl;
#elif defined(__linux__)
    std::cout << "System : Linux" << std::endl;
#else
    std::cout << "System : Autre que Windows et Linux" << std::endl;
#endif

    return 0;
}