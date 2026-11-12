
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

    bool tenuAvec = false;
    bool tenuSans = false;
    bool tombeSans = false;

    int prisesAvec = 0;
    int prisesSans = 0;
    int chutesSans = 0;
    int coincesAvec = 0;
    int differences = 0;

    // Traitement de chaque image
    for (int image = 1; image <= t; image++){
        char signe = signes[image - 1];

        // Calcul des trois champs
        int active = 1;
        if (signe == 'x'){
            active = 0;
        }

        int nouveau = 0;
        if (signe == '1'){
            nouveau = 1;
        }

        int changed = 0;
        if (nouveau != current){
            changed = 1;
        }

        current = nouveau;

        // Objet avec vérification de active
        if (active == 1){
            if (current == 1 && changed == 1){
                tenuAvec = true;
                prisesAvec++;
            }
            else if (current == 0 && changed == 1){
                tenuAvec = false;
            }
        }

        // Objet sans vérification de active
        if (!tombeSans){
            if (current == 1 && changed == 1){
                tenuSans = true;
                prisesSans++;
            }
            else if (current == 0 && changed == 1){
                tenuSans = false;

                if (active == 0){
                    chutesSans++;
                    tombeSans = true;
                }
            }
        }

        // Comptage des images où l'objet reste coincé
        if (signe == '0' && tenuAvec){
            coincesAvec++;
        }

        // Comparaison des deux objets
        if (tenuAvec != tenuSans){
            differences++;
        }

        cout << image << " " << (tenuAvec ? "TENU" : "LIBRE")
             << " " << (tenuSans ? "TENU" : "LIBRE") << endl;
    }

    // Affichage des résultats
    cout << "PRISES AVEC " << prisesAvec << endl;
    cout << "PRISES SANS " << prisesSans << endl;
    cout << "CHUTES SANS " << chutesSans << endl;
    cout << "COINCES AVEC " << coincesAvec << endl;
    cout << "DIFFERENCES " << differences << endl;

    return 0;
}
