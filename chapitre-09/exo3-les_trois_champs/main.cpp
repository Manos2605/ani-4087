
#include <iostream>
#include <string>
using namespace std;

int main(){
    int t;
    cin >> t;

    string signes;
    if (t > 0){
        cin >> signes;
    }

    int current = 0;

    int appuis = 0;
    int relaches = 0;
    int fauxRelaches = 0;
    int tenues = 0;

    // Traitement de chaque image
    for (int image = 1; image <= t; image++){
        char signe = signes[image - 1];

        // Vérification de l'état de la source
        int active = 1;
        if (signe == 'x'){
            active = 0;
        }

        int nouveau = 0;
        if (signe == '1'){
            nouveau = 1;
        }

        // Vérification du changement d'état
        int changed = 0;
        if (nouveau != current){
            changed = 1;
        }

        current = nouveau;

        cout << image << " " << current << " "
             << changed << " " << active << endl;

        // Comptage des appuis et des relâchements
        if (current == 1 && changed == 1){
            appuis++;
        }

        if (current == 0 && changed == 1){
            relaches++;

            if (active == 0){
                fauxRelaches++;
            }
        }

        // Comptage des images où l'action est maintenue
        if (current == 1){
            tenues++;
        }
    }

    // Affichage des résultats
    cout << "APPUIS " << appuis << endl;
    cout << "RELACHES " << relaches << endl;
    cout << "FAUX RELACHES " << fauxRelaches << endl;
    cout << "TENUES " << tenues << endl;

    return 0;
}
