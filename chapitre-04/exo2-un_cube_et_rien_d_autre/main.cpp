#include <iostream>
#include <string>

int main() {
    int n;
    std::cin >> n;

    int visibles = 0;
    int enPanne = 0;

    for (int i = 0; i < n; i++) {
        std::string nom;
        long long drapeaux;
        long long sx, sy, sz;
        long long distance;
        int lumieres;
        int ambiante;
        long long proche;

        std::cin >> nom >> drapeaux >> sx >> sy >> sz >> distance >> lumieres >> ambiante >> proche;

        std::string verdict;

        // 1. Vérifier si le rendu 3D est activé
        if ((drapeaux & 2) == 0) {
            verdict = "RENDER3D ETEINT";
        }

        // 2. Vérifier si une dimension du cube est nulle
        else if (sx == 0 || sy == 0 || sz == 0) {
            verdict = "ECHELLE NULLE";
        }

        else {
            // Position de la face avant du cube
            long long faceAvant = distance - sz / 2;

            // 3. Vérifier si la caméra est dans le cube
            if (faceAvant <= 0) {
                verdict = "CAMERA DANS LE CUBE";
            }

            // 4. Vérifier le plan rapproché
            else if (faceAvant < proche) {
                verdict = "COUPE PAR LE PLAN PROCHE";
            }

            // 5. Vérifier la présence de lumière
            else if (lumieres == 0 && ambiante == 0) {
                verdict = "PAS DE LUMIERE";
            }

            // 6. Aucun problème
            else {
                verdict = "VISIBLE";
            }
        }

        std::cout << nom << " " << verdict << "\n";

        if (verdict == "VISIBLE") {
            visibles++;
        }
        else {
            enPanne++;
        }
    }

    std::cout << "VISIBLES " << visibles << "\n";
    std::cout << "EN PANNE " << enPanne << "\n";

    return 0;
}