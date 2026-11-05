#include <iostream>
#include <string>
#include <vector>

namespace{
    struct Cible{
        int largeur;
        int hauteur;
    };
}

int main(){
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int largeurFenetre = 1;
    int hauteurFenetre = 1;
    int nombreTampons = 1;
    int nombreOrdres = 0;

    if (!(std::cin >> largeurFenetre >> hauteurFenetre >> nombreTampons >> nombreOrdres)){
        return 0;
    }

    bool offscreenAllume = false;
    std::vector<Cible> cibles;
    int destination = 0; // 0 : la fenêtre, sinon le numéro de la cible
    int zoneLargeur = largeurFenetre;
    int zoneHauteur = hauteurFenetre;

    std::vector<bool> tamponPris(static_cast<unsigned long>(nombreTampons), false);

    int imagesAffichees = 0;
    int imagesNumerotees = 0;
    bool bloque = false;

    for (int i = 0; i < nombreOrdres; ++i){
        std::string ordre;

        if (!(std::cin >> ordre)){
            break;
        }

        if (ordre == "allume"){
            std::string nom;
            std::cin >> nom;

            if (nom == "OFFSCREEN"){
                offscreenAllume = true;
            }
        } else if (ordre == "cible"){
            int largeur = 0;
            int hauteur = 0;
            std::cin >> largeur >> hauteur;

            if (!offscreenAllume){
                std::cout << "CIBLE NULLE\n";
            } else{
                cibles.push_back(Cible{largeur, hauteur});
                std::cout << "CIBLE " << cibles.size() << ' ' << largeur << ' ' << hauteur << '\n';
            }
        } else if (ordre == "vers"){
            int numero = 0;
            std::cin >> numero;

            if (numero == 0){
                destination = 0;
            } else if (numero > 0 && static_cast<std::size_t>(numero) <= cibles.size()){
                destination = numero;
            } else{
                std::cout << "VERS REFUSE\n";
            }
        } else if (ordre == "taille"){
            std::cin >> zoneLargeur >> zoneHauteur;
        } else if (ordre == "image"){
            ++imagesNumerotees;
            const int tampon = (imagesNumerotees - 1) % nombreTampons;

            if (tamponPris[static_cast<std::size_t>(tampon)]){
                std::cout << "BLOQUE " << imagesNumerotees << '\n';
                bloque = true;
                break;
            }

            if (destination == 0){
                std::cout << "IMAGE " << imagesNumerotees << " FENETRE\n";
            } else{
                std::cout << "IMAGE " << imagesNumerotees << " CIBLE " << destination << '\n';
                const Cible &cible = cibles[static_cast<std::size_t>(destination - 1)];

                if (zoneLargeur > cible.largeur || zoneHauteur > cible.hauteur){
                    // Le travail ne se termine jamais : le tampon reste pris.
                    tamponPris[static_cast<std::size_t>(tampon)] = true;
                }
            }
            ++imagesAffichees;
        }
    }

    std::cout << "IMAGES " << imagesAffichees << '\n';
    std::cout << "ETAT " << (bloque ? "BLOQUE" : "LIBRE") << '\n';

    return 0;
}
