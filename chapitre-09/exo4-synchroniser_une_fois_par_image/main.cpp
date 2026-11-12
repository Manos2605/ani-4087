
#include <iostream>
using namespace std;

int main(){
    int t;
    cin >> t;

    int current = 0;
    int changed = 0;
    int active = 0;

    int appuisReels = 0;
    int saisies = 0;
    int refusees = 0;
    int precedent = 0;

    // Traitement de chaque image
    for (int image = 1; image <= t; image++){
        int p, s, f;
        cin >> p >> s >> f;

        // Synchronisation des actions
        for (int appel = 0; appel < s; appel++){
            if (f == 0){
                refusees++;
            }
            else {
                changed = 0;

                if (p != current){
                    changed = 1;
                }

                current = p;
                active = 1;
            }
        }

        // Vérification d'une saisie détectée
        bool saisit = (current == 1 && changed == 1);

        // Comptage des vrais appuis
        if (p == 1 && precedent == 0){
            appuisReels++;
        }

        precedent = p;

        if (saisit){
            saisies++;
        }

        cout << image << " " << current << " " << changed << " "
             << active << " " << (saisit ? "SAISIT" : "-") << endl;
    }

    // Affichage des résultats
    cout << "APPUIS REELS " << appuisReels << endl;
    cout << "SAISIES " << saisies << endl;
    cout << "SYNC REFUSEES " << refusees << endl;

    return 0;
}
