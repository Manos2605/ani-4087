#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

bool contient(const vector<string>& liste, const string& mot) {
    return find(liste.begin(), liste.end(), mot) != liste.end();
}

int main() {
    int nA;
    if (!(cin >> nA)) {
        return 0;
    }

    vector<string> listeA;
    for (int i = 0; i < nA; ++i) {
        string nom;
        cin >> nom;
        listeA.push_back(nom);
    }

    int nB;
    if (!(cin >> nB)) {
        return 0;
    }

    vector<string> listeB;
    for (int i = 0; i < nB; ++i) {
        string nom;
        cin >> nom;
        listeB.push_back(nom);
    }

    vector<string> communs;
    vector<string> seulA;
    vector<string> seulB;
    vector<string> unionListe;

    for (const string& nom : listeA) {
        if (!contient(unionListe, nom)) {
            unionListe.push_back(nom);
        }
        if (contient(listeB, nom)) {
            if (!contient(communs, nom)) {
                communs.push_back(nom);
            }
        } else {
            seulA.push_back(nom);
        }
    }

    for (const string& nom : listeB) {
        if (!contient(unionListe, nom)) {
            unionListe.push_back(nom);
        }
        if (!contient(listeA, nom)) {
            seulB.push_back(nom);
        }
    }

    cout << "DEUX " << communs.size() << "\n";

    for (const string& nom : seulA) {
        cout << "SEUL_A " << nom << "\n";
    }

    for (const string& nom : seulB) {
        cout << "SEUL_B " << nom << "\n";
    }

    size_t diff = seulA.size() + seulB.size();
    cout << "DIFFERENTES " << diff << " SUR " << unionListe.size() << "\n";

    return 0;
}