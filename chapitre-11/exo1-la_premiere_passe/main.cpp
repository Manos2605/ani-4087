#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

using namespace std;

bool estCaractereIdentifiant(char c) {
    return isalnum(static_cast<unsigned char>(c)) || c == '_';
}

bool estCaractereVariable(char c) {
    return isupper(static_cast<unsigned char>(c)) || isdigit(static_cast<unsigned char>(c)) || c == '_';
}

int main() {
    int n;
    if (!(cin >> n)) {
        return 0;
    }

    // Consommer le retour a la ligne apres l'entier N
    string reste;
    getline(cin, reste);

    if (n == 0) {
        cout << "ENTETE 0\n";
        cout << "IGNOREES 0\n";
        cout << "VARIABLE AUCUNE\n";
        cout << "PART 0\n";
        return 0;
    }

    int lignesEntete = 0;
    bool dansEntete = true;
    vector<string> variables;

    for (int i = 0; i < n; ++i) {
        string ligne;
        getline(cin, ligne);

        if (!dansEntete) {
            continue;
        }

        // Retirer les espaces et tabulations du debut
        size_t debut = 0;
        while (debut < ligne.size() && (ligne[debut] == ' ' || ligne[debut] == '\t')) {
            debut++;
        }

        // Verifier si la ligne commence par //
        if (debut + 1 < ligne.size() && ligne[debut] == '/' && ligne[debut + 1] == '/') {
            lignesEntete++;

            // Rechercher les variables NK_...=
            for (size_t pos = debut + 2; pos + 3 < ligne.size(); ++pos) {
                if (ligne[pos] == 'N' && ligne[pos + 1] == 'K' && ligne[pos + 2] == '_') {
                    // Le caractere juste avant NK_ ne doit pas etre une lettre, un chiffre ou _
                    if (pos > 0 && estCaractereIdentifiant(ligne[pos - 1])) {
                        continue;
                    }

                    size_t fin = pos + 3;
                    while (fin < ligne.size() && estCaractereVariable(ligne[fin])) {
                        fin++;
                    }

                    // Il faut au moins un signe apres NK_ et un '=' juste apres
                    if (fin > pos + 3 && fin < ligne.size() && ligne[fin] == '=') {
                        string nomVar = ligne.substr(pos, fin - pos);
                        if (find(variables.begin(), variables.end(), nomVar) == variables.end()) {
                            variables.push_back(nomVar);
                        }
                    }
                }
            }
        } else {
            dansEntete = false;
        }
    }

    int ignorees = n - lignesEntete;
    int part = (100 * lignesEntete) / n;

    cout << "ENTETE " << lignesEntete << "\n";
    cout << "IGNOREES " << ignorees << "\n";

    if (variables.empty()) {
        cout << "VARIABLE AUCUNE\n";
    } else {
        for (const string& v : variables) {
            cout << "VARIABLE " << v << "\n";
        }
    }

    cout << "PART " << part << "\n";

    return 0;
}