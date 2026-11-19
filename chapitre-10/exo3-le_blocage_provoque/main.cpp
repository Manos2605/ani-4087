#include <iostream>
#include <string>

using namespace std;

int main() {
    string P;
    int T;
    cin >> P >> T;

    string signes = "";
    if (T > 0) {
        cin >> signes;
    }

    bool ouverte = false;
    int rendues = 0;
    int erreurs = 0;
    int premierRefus = 0;
    bool premierRefusMis = false;

    for (int t = 1; t <= T; t++) {
        bool rendre = false;

        string w = "OK";
        string b = "OK";
        string r = "NON";
        string e = "-";

        if (!ouverte) {
            rendre = (signes[t - 1] - '0') == 1;
        }

        if (ouverte) {
            w = "REFUSE";
            erreurs++;
            if (!premierRefusMis) {
                premierRefus = t;
                premierRefusMis = true;
            }
        } else {
            if (P == "TOUJOURS") {
                w = "OK";
            }
        }

        if (!ouverte) {
            b = "OK";
            ouverte = true;

            if (rendre) {
                r = "OUI";
                rendues++;
            }
        } else {
            b = "REFUSE";
            erreurs++;

            if (!premierRefusMis) {
                premierRefus = t;
                premierRefusMis = true;
            }
        }

        if (P == "TOUJOURS") {
            e = "OK";
            ouverte = false;
        } else if (rendre) {
            e = "OK";
            ouverte = false;
        } else {
            e = "-";
        }

        cout << t << " " << w << " " << b << " " << r << " " << e << endl;
    }

    int perdues = 0;
    for (int t = 1; T > 0 && t <= T; t++) {
        if (signes[t - 1] == '1' && t > 2 && t <= T) {
            perdues++;
        }
    }

    if (premierRefus == 0) {
        cout << "RENDUES " << rendues << endl;
        cout << "PERDUES " << 0 << endl;
        cout << "ERREURS " << erreurs << endl;
        cout << "PREMIER REFUS AUCUN" << endl;
    } else {
        cout << "RENDUES " << rendues << endl;
        cout << "PERDUES " << perdues << endl;
        cout << "ERREURS " << erreurs << endl;
        cout << "PREMIER REFUS " << premierRefus << endl;
    }

    return 0;
}
