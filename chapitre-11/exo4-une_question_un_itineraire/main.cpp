#include <iostream>
#include <string>

using namespace std;

bool lireLignePropre(istream& flux, string& ligne) {
    if (!getline(flux, ligne)) {
        return false;
    }
    if (!ligne.empty() && ligne.back() == '\r') {
        ligne.pop_back();
    }
    return true;
}

string extraireValeur(const string& contenu, size_t debut, size_t fin) {
    string valeur = "";
    for (size_t idx = debut; idx < fin; ++idx) {
        if (contenu[idx] == '\\' && idx + 1 < fin && contenu[idx + 1] == '\\') {
            valeur += '\\';
            idx++;
        } else {
            valeur += contenu[idx];
        }
    }
    return valeur;
}

void traiterMachine(int num, const string& cheminManifeste, int nbLignes, const string& contenu) {
    if (cheminManifeste == "-") {
        cout << num << " REGISTRE AUCUN\n";
        return;
    }

    cout << num << " MANIFESTE " << cheminManifeste << "\n";

    if (nbLignes == -1) {
        cout << num << " ECHEC ILLISIBLE\n";
        return;
    }

    string cle = "\"library_path\"";
    size_t posCle = contenu.find(cle);
    if (posCle == string::npos) {
        cout << num << " ECHEC CLE\n";
        return;
    }

    size_t posDeuxPoints = contenu.find(':', posCle + cle.size());
    if (posDeuxPoints == string::npos) {
        cout << num << " ECHEC DEUX_POINTS\n";
        return;
    }

    size_t posGuillemet1 = contenu.find('"', posDeuxPoints + 1);
    if (posGuillemet1 == string::npos) {
        cout << num << " ECHEC GUILLEMET\n";
        return;
    }

    size_t posGuillemet2 = contenu.find('"', posGuillemet1 + 1);
    if (posGuillemet2 == string::npos) {
        cout << num << " ECHEC GUILLEMET\n";
        return;
    }

    string valeur = extraireValeur(contenu, posGuillemet1 + 1, posGuillemet2);
    if (valeur.empty()) {
        cout << num << " ECHEC VIDE\n";
        return;
    }

    cout << num << " VALEUR " << valeur << "\n";

    bool estAbsolu = (valeur[0] == '\\' || valeur[0] == '/') ||
                     (valeur.size() >= 2 && valeur[1] == ':');

    if (estAbsolu) {
        cout << num << " ABSOLU\n";
        cout << num << " CHARGE " << valeur << "\n";
    } else {
        cout << num << " RELATIF\n";
        size_t dernierSep = cheminManifeste.find_last_of("\\/");
        string dossier = (dernierSep == string::npos) ? "" : cheminManifeste.substr(0, dernierSep + 1);
        cout << num << " CHARGE " << dossier << valeur << "\n";
    }
}

int main() {
    int k;
    if (!(cin >> k)) {
        return 0;
    }

    string poubelle;
    lireLignePropre(cin, poubelle);

    for (int i = 1; i <= k; ++i) {
        string cheminManifeste;
        lireLignePropre(cin, cheminManifeste);

        int nbLignes;
        cin >> nbLignes;
        lireLignePropre(cin, poubelle);

        string contenu = "";
        for (int j = 0; j < nbLignes; ++j) {
            string ligne;
            lireLignePropre(cin, ligne);
            contenu += ligne + "\n";
        }

        traiterMachine(i, cheminManifeste, nbLignes, contenu);
    }

    return 0;
}