#include <iostream>
#include <string>
#include <vector>
#include <cctype>

using namespace std;

bool estIdentifiant(char c) {
    return isalnum(static_cast<unsigned char>(c)) || c == '_';
}

bool contientMotEntier(const string& texte, const string& mot) {
    if (mot.empty() || texte.size() < mot.size()) {
        return false;
    }

    size_t pos = texte.find(mot);
    while (pos != string::npos) {
        bool avantOk = (pos == 0) || !estIdentifiant(texte[pos - 1]);
        size_t fin = pos + mot.size();
        bool apresOk = (fin == texte.size()) || !estIdentifiant(texte[fin]);

        if (avantOk && apresOk) {
            return true;
        }
        pos = texte.find(mot, pos + 1);
    }
    return false;
}

bool contientUnDesMots(const string& texte, const vector<string>& listeMots) {
    for (const string& mot : listeMots) {
        if (contientMotEntier(texte, mot)) {
            return true;
        }
    }
    return false;
}

int main() {
    string etiquette;
    int k;
    if (!(cin >> etiquette >> k)) {
        return 0;
    }

    vector<string> nomsBackend;
    for (int i = 0; i < k; ++i) {
        string nom;
        cin >> nom;
        nomsBackend.push_back(nom);
    }

    int m;
    cin >> etiquette >> m;
    vector<string> nomsDefinis;
    for (int i = 0; i < m; ++i) {
        string nom;
        cin >> nom;
        nomsDefinis.push_back(nom);
    }

    int n;
    cin >> n;
    string poubelle;
    getline(cin, poubelle);

    int decisions = 0;
    int fuites = 0;
    int definis = 0;

    for (int numLigne = 1; numLigne <= n; ++numLigne) {
        string ligneComplete;
        getline(cin, ligneComplete);

        // Separer la ZONE du reste du texte
        size_t posEspace = ligneComplete.find(' ');
        string zone;
        string texte = "";

        if (posEspace == string::npos) {
            zone = ligneComplete;
        } else {
            zone = ligneComplete.substr(0, posEspace);
            texte = ligneComplete.substr(posEspace + 1);
        }

        // Ignorer les espaces au debut du texte pour tester le commentaire //
        size_t debut = 0;
        while (debut < texte.size() && texte[debut] == ' ') {
            debut++;
        }

        if (debut + 1 < texte.size() && texte[debut] == '/' && texte[debut + 1] == '/') {
            continue;
        }

        bool aNomC = contientUnDesMots(texte, nomsBackend);
        bool aNomD = contientUnDesMots(texte, nomsDefinis);

        if (aNomC) {
            if (zone == "INIT") {
                decisions++;
            } else if (zone == "BOUCLE" || zone == "SCENE") {
                fuites++;
                cout << "FUITE " << numLigne << " " << zone << "\n";
            }
        }

        if (aNomD && zone != "FABRIQUE") {
            definis++;
            cout << "DEFINI " << numLigne << " " << zone << "\n";
        }
    }

    cout << "DECISIONS " << decisions << "\n";
    cout << "FUITES " << fuites << "\n";
    cout << "DEFINIS " << definis << "\n";

    if (fuites == 0 && definis == 0) {
        cout << "ABSTRACTION TENUE\n";
    } else {
        cout << "ABSTRACTION FUIT\n";
    }

    return 0;
}