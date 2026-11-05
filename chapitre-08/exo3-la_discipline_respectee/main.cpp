#include <iostream>
#include <string>

int main(){
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int nombreImages = 1;
    int nombreOrdres = 0;

    if (!(std::cin >> nombreImages >> nombreOrdres)){
        return 0;
    }

    if (nombreImages <= 0){
        // Sans aucune image, le modulo de l'index serait une division par zéro.
        return 0;
    }

    int prochaine = 0; // prochaine image à emprunter
    bool empruntee = false; // une image est-elle empruntée ?
    int imageEmpruntee = 0; // laquelle
    bool dessinee = false; // l'image empruntée a-t-elle reçu un draw ?

    bool rendueDepuisSoumission = false; // une image a-t-elle été rendue depuis le dernier envoi ?
    int derniereRendue = 0; // j de la dernière image relâchée
    bool derniereDessinee = false; // cette image avait-elle été dessinée ?

    int soumises = 0;
    int refus = 0;
    int horsEmprunt = 0;

    for (int i = 0; i < nombreOrdres; ++i){
        std::string ordre;

        if (!(std::cin >> ordre)){
            break;
        }

        if (ordre == "acquire"){
            if (empruntee){
                std::cout << "ACQUIRE REFUSE\n";
                ++refus;
            } else{
                empruntee = true;
                imageEmpruntee = prochaine;
                dessinee = false;
                std::cout << "ACQUIRE " << imageEmpruntee << '\n';
            }
        } else if (ordre == "draw"){
            if (empruntee){
                dessinee = true;
                std::cout << "DRAW " << imageEmpruntee << '\n';
            } else{
                std::cout << "DRAW HORS EMPRUNT\n";
                ++horsEmprunt;
            }
        } else if (ordre == "release"){
            if (!empruntee){
                std::cout << "RELEASE REFUSE\n";
                ++refus;
            } else{
                derniereRendue = imageEmpruntee;
                derniereDessinee = dessinee;
                rendueDepuisSoumission = true;
                prochaine = (prochaine + 1) % nombreImages;
                empruntee = false;
                dessinee = false;
                std::cout << "RELEASE OK\n";
            }
        } else if (ordre == "submit"){
            if (empruntee){
                std::cout << "SUBMIT REFUSE EMPRUNTEE\n";
                ++refus;
            } else if (!rendueDepuisSoumission){
                std::cout << "SUBMIT REFUSE RIEN\n";
                ++refus;
            } else{
                std::cout << "SUBMIT " << derniereRendue;

                if (!derniereDessinee){
                    std::cout << " VIDE";
                }

                std::cout << '\n';
                rendueDepuisSoumission = false;
                ++soumises;
            }
        }
    }

    std::cout << "SOUMISES " << soumises << '\n';
    std::cout << "REFUS " << refus << '\n';
    std::cout << "HORS EMPRUNT " << horsEmprunt << '\n';

    return 0;
}
