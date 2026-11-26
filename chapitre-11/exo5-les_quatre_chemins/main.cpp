#include <iostream>
#include <string>

using namespace std;

int main() {
    int k;
    if (!(cin >> k)) {
        return 0;
    }

    if (k == 0) {
        cout << "AUCUNE MACHINE\n";
        return 0;
    }

    for (int i = 1; i <= k; ++i) {
        string d, v, l, r, b, g;
        cin >> d >> v >> l >> r >> b >> g;

        int chargees = 0;
        int liberees = 0;

        // Regle 1 : si on demande le simulateur, on ne tente rien
        if (d == "SIMULATEUR") {
            cout << i << " BACKEND SIMULATEUR DEMANDE\n";
            if (g == "OUI") {
                cout << i << " NIVEAU 2 SIMULATEUR CARTE\n";
            } else {
                cout << i << " NIVEAU 3 SIMULATEUR LOGICIEL\n";
            }
            cout << i << " CHARGEES " << chargees << " LIBEREES " << liberees << "\n";
            continue;
        }

        bool biblioChargee = false;
        bool aPointEntree = false;
        string source = "";

        // Regle 2 : variable d'environnement
        if (v != "-") {
            if (v == "ABSENT") {
                cout << i << " VARIABLE IGNOREE\n";
            } else {
                cout << i << " VARIABLE CHARGEE\n";
                chargees++;
                biblioChargee = true;
                if (v == "BON") {
                    aPointEntree = true;
                    source = "VARIABLE";
                }
            }
        }

        // Regle 3 : loader standard si rien n'est encore charge
        if (!biblioChargee) {
            if (l == "ABSENT") {
                cout << i << " LOADER ABSENT\n";
            } else {
                cout << i << " LOADER CHARGE\n";
                chargees++;
                biblioChargee = true;
                if (l == "BON") {
                    aPointEntree = true;
                    source = "LOADER";
                }
            }
        }

        // Regle 4 : liberer si la bibliotheque chargee n'a pas de point d'entree
        if (biblioChargee && !aPointEntree) {
            cout << i << " SANS_ENTREE\n";
            liberees++;
            biblioChargee = false;
        }

        // Regle 5 : decouverte directe si pas encore de point d'entree
        if (!aPointEntree) {
            cout << i << " DIRECT " << r << "\n";
            if (r == "NEGO_KO" || r == "BON") {
                chargees++;
                biblioChargee = true;
                if (r == "BON") {
                    aPointEntree = true;
                    source = "DIRECT";
                }
            }
        }

        // Regle 6 : bilan du backend OpenXR et liaison graphique
        bool liaisonReussie = false;
        if (!aPointEntree) {
            cout << i << " BACKEND INDISPONIBLE\n";
            if (biblioChargee) {
                liberees++;
                biblioChargee = false;
            }
        } else {
            cout << i << " BACKEND OPENXR " << source << "\n";
            if (b == "OUI") {
                liaisonReussie = true;
            } else {
                cout << i << " LIAISON ECHOUEE\n";
                liberees++;
                biblioChargee = false;
            }
        }

        // Regle 7 : niveau final de fonctionnement
        if (aPointEntree && liaisonReussie) {
            cout << i << " NIVEAU 1 CASQUE\n";
        } else if (g == "OUI") {
            cout << i << " NIVEAU 2 SIMULATEUR CARTE\n";
        } else {
            cout << i << " NIVEAU 3 SIMULATEUR LOGICIEL\n";
        }

        // Regle 8 : compteur de bibliotheques
        cout << i << " CHARGEES " << chargees << " LIBEREES " << liberees << "\n";
    }

    return 0;
}