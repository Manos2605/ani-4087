#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>

using namespace std;

bool estCpp(const string& nom) {
    return nom.size() >= 4 && nom.substr(nom.size() - 4) == ".cpp";
}

int main() {
    int e;
    if (!(cin >> e)) {
        return 0;
    }

    unordered_set<string> externes;
    for (int i = 0; i < e; ++i) {
        string nomExt;
        cin >> nomExt;
        externes.insert(nomExt);
    }

    int f;
    if (!(cin >> f)) {
        return 0;
    }

    vector<string> ordreFichiers;
    unordered_map<string, vector<string>> inclusions;

    for (int i = 0; i < f; ++i) {
        string nom;
        int k;
        cin >> nom >> k;
        ordreFichiers.push_back(nom);

        vector<string> listeInc;
        for (int j = 0; j < k; ++j) {
            string inc;
            cin >> inc;
            listeInc.push_back(inc);
        }
        inclusions[nom] = listeInc;
    }

    int touchees = 0;
    int totalUnites = 0;

    for (const string& fichier : ordreFichiers) {
        if (!estCpp(fichier)) {
            continue;
        }

        totalUnites++;

        queue<string> file;
        unordered_set<string> dejaVu;
        unordered_map<string, string> parent;

        file.push(fichier);
        dejaVu.insert(fichier);

        string externeTrouve = "";

        while (!file.empty() && externeTrouve.empty()) {
            string courant = file.front();
            file.pop();

            // Si ce n'est pas un fichier du projet (en-tete systeme), on n'explore pas plus loin
            if (inclusions.find(courant) == inclusions.end()) {
                continue;
            }

            for (const string& voisin : inclusions[courant]) {
                if (dejaVu.count(voisin)) {
                    continue;
                }

                dejaVu.insert(voisin);
                parent[voisin] = courant;

                if (externes.count(voisin)) {
                    externeTrouve = voisin;
                    break;
                }

                file.push(voisin);
            }
        }

        if (externeTrouve.empty()) {
            cout << fichier << " NON\n";
        } else {
            touchees++;
            vector<string> chemin;
            string cur = externeTrouve;
            while (cur != fichier) {
                chemin.push_back(cur);
                cur = parent[cur];
            }
            chemin.push_back(fichier);

            cout << fichier << " OUI ";
            for (int i = static_cast<int>(chemin.size()) - 1; i >= 0; --i) {
                cout << chemin[i];
                if (i > 0) {
                    cout << ">";
                }
            }
            cout << "\n";
        }
    }

    cout << "TOUCHEES " << touchees << " SUR " << totalUnites << "\n";

    return 0;
}