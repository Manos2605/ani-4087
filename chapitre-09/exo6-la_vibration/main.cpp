
#include <iostream>
#include <string>
using namespace std;

int main(){
    int n;
    cin >> n;

    int bonnes = 0;
    int longues = 0;
    int muettes = 0;
    long long temps = 0;

    // Traitement de chaque vibration
    for (int appel = 1; appel <= n; appel++){
        long long a, d, f;
        cin >> a >> d >> f;

        // Limitation de l'amplitude entre 0 et 100
        long long amplitude = a;

        if (amplitude < 0){
            amplitude = 0;
        }
        else if (amplitude > 100){
            amplitude = 100;
        }

        // Vérification de la durée
        long long duree = d;
        if (duree <= 0){
            duree = 0;
        }

        // Détermination du verdict
        string verdict;

        if (amplitude == 0){
            verdict = "MUETTE";
            muettes++;
        }
        else if (d < 50){
            verdict = "BREVE";
        }
        else if (d <= 200){
            verdict = "BONNE";
            bonnes++;
        }
        else {
            verdict = "LONGUE";
            longues++;
        }

        temps += duree;

        // Affichage de la vibration
        cout << appel << " " << amplitude << " ";

        if (d > 0){
            cout << d;
        } else {
            cout << "MIN";
        }

        cout << " ";

        if (f > 0){
            cout << f;
        } else {
            cout << "DEFAUT";
        }

        cout << " " << verdict << endl;
    }

    // Affichage des statistiques
    cout << "BONNES " << bonnes << endl;
    cout << "LONGUES " << longues << endl;
    cout << "MUETTES " << muettes << endl;
    cout << "TEMPS " << temps << endl;

    return 0;
}
