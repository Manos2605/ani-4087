#include <iostream>
#include <string>

using namespace std;

int main() {
    int k;
    if (!(cin >> k)) {
        return 0;
    }

    string poubelle;
    getline(cin, poubelle);

    for (int i = 1; i <= k; ++i) {
        string cheminManifeste;
        getline(cin, cheminManifeste);

        int l;
        cin >> l;
        getline(cin, poubelle);

        string contenu = "";
        for (int j = 0; j < l; ++j) {
            string ligne;
            getline(cin, ligne);
            contenu += ligne + "\n";
        }

        if (cheminManifeste == "-") {
            cout << i << " REGISTRE AUCUN\n";
            continue;
        }

        cout << i << " MANIFESTE " << cheminManifeste << "\n";

        if (l == -1) {
            cout << i << " ECHEC ILLISIBLE\n";
            continue;
        }

        string cle = "\"library_path\"";
        size_t posCle = contenu.find(cle);
        if (posCle == string::npos) {
            cout << i << " ECHEC CLE\n";
            continue;
        }

        size_t posDeuxPoints = contenu.find(':', posCle + cle.size());
        if (posDeuxPoints == string::npos) {
            cout << i << " ECHEC DEUX_POINTS\n";
            continue;
        }

        size_t posGuillemet1 = contenu.find('"', posDeuxPoints + 1);
        if (posGuillemet1 == string::npos) {
            cout << i << " ECHEC GUILLEMET\n";
            continue;
        }

        size_t posGuillemet2 = contenu.find('"', posGuillemet1 + 1);
        if (posGuillemet2 == string::npos) {
            cout << i << " ECHEC GUILLEMET\n";
            continue;
        }

        // Recopier la valeur en simplifiant les doubles antislashs
        string valeur = "";
        for (size_t idx = posGuillemet1 + 1; idx < posGuillemet2; ++idx) {
            if (contenu[idx] == '\\' && idx + 1 < posGuillemet2 && contenu[idx + 1] == '\\') {
                valeur += '\\';
                idx++;
            } else {
                valeur += contenu[idx];
            }
        }

        if (valeur.empty()) {
            cout << i << " ECHEC VIDE\n";
            continue;
        }

        cout << i << " VALEUR " << valeur << "\n";

        bool estAbsolu = false;
        if (valeur[0] == '\\' || valeur[0] == '/') {
            estAbsolu = true;
        } else if (valeur.size() >= 2 && valeur[1] == ':') {
            estAbsolu = true;
        }

        if (estAbsolu) {
            cout << i << " ABSOLU\n";
            cout << i << " CHARGE " << valeur << "\n";
        } else {
            cout << i << " RELATIF\n";
            size_t dernierSep = cheminManifeste.find_last_of("\\/");
            string dossier = cheminManifeste.substr(0, dernierSep + 1);
            cout << i << " CHARGE " << dossier << valeur << "\n";
        }
    }

    return 0;
}