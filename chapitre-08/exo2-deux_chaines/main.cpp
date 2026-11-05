#include <iostream>
#include <string>

namespace{
    const int EmplacementsParChaine = 4;
    const int ImagesMaximum = EmplacementsParChaine;
}

int main(){
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int nombreChaines = 0;

    if (!(std::cin >> nombreChaines)){
        return 0;
    }

    int acceptees = 0;
    int occupes = 0;
    int gauche = 0;
    int droit = 0;
    int sansNom = 0;

    for (int i = 0; i < nombreChaines; ++i){
        int largeur = 0;
        int hauteur = 0;
        int images = 0;
        int oeil = 0;
        std::string nom;

        if (!(std::cin >> largeur >> hauteur >> images >> oeil >> nom)){
            break;
        }

        const bool aUnNom = (nom != "-");
        const std::string nomAffiche = aUnNom ? nom : "SANS-NOM";

        if (largeur == 0 || hauteur == 0){
            std::cout << "REFUSEE " << nomAffiche << " TAILLE\n";
        } else if (images == 0 || images > ImagesMaximum){
            std::cout << "REFUSEE " << nomAffiche << " IMAGES\n";
        } else{
            std::cout << "CHAINE " << nomAffiche << ' ' << largeur << ' ' << hauteur << ' '
                      << images << ' ' << (oeil == 0 ? "GAUCHE" : "DROIT") << '\n';
            ++acceptees;
            occupes += images;

            if (oeil == 0){
                ++gauche;
            } else{
                ++droit;
            }

            if (!aUnNom){
                ++sansNom;
            }
        }
    }

    std::cout << "ACCEPTEES " << acceptees << '\n';
    std::cout << "EMPLACEMENTS " << acceptees * EmplacementsParChaine << '\n';
    std::cout << "OCCUPES " << occupes << '\n';
    std::cout << "GAUCHE " << gauche << '\n';
    std::cout << "DROIT " << droit << '\n';
    std::cout << "SANS NOM " << sansNom << '\n';

    return 0;
}
