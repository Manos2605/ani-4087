#include <iostream>
#include <string>

using namespace std;

int main() {
    int N;
    cin >> N;

    int muettes = 0;

    for (int i = 1; i <= N; i++) {
        string fin;
        int g, d;
        char instant, pose, espace;
        cin >> fin >> g >> d >> instant >> pose >> espace;

        bool fige = (fin == "JAMAIS");
        bool neRepartPas = (fin == "SI_RENDU");
        bool oeilNoirG = (g == 0);
        bool oeilNoirD = (d == 0);
        bool tremble = (instant == 'N');
        bool deTravers = (pose == 'F');
        bool decale = (espace == 'L');

        bool aUnSymptome = fige || neRepartPas || oeilNoirG || oeilNoirD || tremble || deTravers || decale;

        bool simulateurDit = (fin != "TOUJOURS") || (g == 0) || (d == 0);

        if (aUnSymptome && !simulateurDit) {
            muettes++;
        }

        cout << i << " CASQUE";

        if (fige) cout << " FIGE";
        if (neRepartPas) cout << " NE_REPART_PAS";
        if (oeilNoirG) cout << " OEIL_NOIR_G";
        if (oeilNoirD) cout << " OEIL_NOIR_D";
        if (tremble) cout << " TREMBLE";
        if (deTravers) cout << " DE_TRAVERS";
        if (decale) cout << " DECALE";

        if (!aUnSymptome) {
            cout << " AUCUN";
        }

        cout << endl;

        cout << i << " SIMULATEUR";
        if (simulateurDit) {
            cout << " DIT" << endl;
        } else {
            cout << " MUET" << endl;
        }
    }

    cout << "MUETTES " << muettes << endl;

    return 0;
}
