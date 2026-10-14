#include "NKImage/Core/NkImage.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <cstdint>

using namespace nkentseu;
namespace fs = std::filesystem;

// Informations sur chaque format d'image
struct FormatImage {
    std::string nom;
    std::string chemin;
    bool (NkImage::*sauvegarder)(const char *) const noexcept;
};

int main() {
    NkImage photo;

    // Chargement de l'image originale
    if (!photo.Load("photo.png")) {
        std::cerr << "Erreur : impossible de charger photo.png.\n";
        return 1;
    }

    // Dimensions de l'image de départ
    const int largeurOriginale = photo.Width();
    const int hauteurOriginale = photo.Height();

    // Les cinq formats à tester
    FormatImage formats[] = {
        {"PNG", "photo_sortie.png", &NkImage::SavePNG},
        {"JPEG", "photo_sortie.jpg", nullptr},
        {"BMP", "photo.bmp", &NkImage::SaveBMP},
        {"TGA", "photo.tga", &NkImage::SaveTGA},
        {"QOI", "photo.qoi", &NkImage::SaveQOI}
    };

    // Fichier qui contiendra les résultats
    std::ofstream resultat("formats.txt");

    if (!resultat) {
        std::cerr << "Erreur : impossible de creer formats.txt.\n";
        return 1;
    }

    for (const auto &format : formats) {
        bool ecriture = false;

        // Sauvegarde de l'image dans le format choisi
        if (format.nom == "JPEG") {
            ecriture = photo.SaveJPEG(format.chemin.c_str(), 90);
        } else {
            ecriture = (photo.*format.sauvegarder)(format.chemin.c_str());
        }

        // Récupération de la taille du fichier
        std::uintmax_t taille = 0;
        std::error_code erreur;

        bool fichierExiste =
            fs::is_regular_file(format.chemin, erreur) && !erreur;

        if (fichierExiste) {
            taille = fs::file_size(format.chemin, erreur);

            if (erreur) {
                fichierExiste = false;
                taille = 0;
            }
        }

        // On recharge l'image pour vérifier qu'elle est lisible
        NkImage imageRelue;
        bool lecture = false;

        if (ecriture && fichierExiste) {
            lecture = imageRelue.Load(format.chemin.c_str());
        }

        if (lecture) {
            // Écriture des résultats dans le fichier
            resultat << format.nom << " : " << taille
                     << " octets, " << imageRelue.Width()
                     << " x " << imageRelue.Height() << '\n';

            // Vérification des dimensions après rechargement
            if (imageRelue.Width() != largeurOriginale ||
                imageRelue.Height() != hauteurOriginale) {
                std::cerr << "Attention : les dimensions de "
                          << format.nom
                          << " different de celles de la photo originale.\n";
            }

            std::cout << format.nom << " : " << taille
                      << " octets, " << imageRelue.Width()
                      << " x " << imageRelue.Height() << '\n';
        } else {
            // Le fichier n'a pas pu être relu
            resultat << format.nom << " : " << taille
                     << " octets, illisible\n";

            std::cout << format.nom << " : " << taille
                      << " octets, illisible\n";
        }
    }

    // Affichage des dimensions de l'image originale
    std::cout << "Dimensions originales : "
              << largeurOriginale << " x "
              << hauteurOriginale << '\n';

    return 0;
}