#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

struct InfoReglage {
    bool aGetenv = false;
    bool aLit = false;
    bool aEcrit = false;
    bool aEnv = false;
};

int main() {
    int r;
    if (!(cin >> r)) {
        return 0;
    }

    vector<string> ordre;
    unordered_map<string, InfoReglage> infos;

    for (int i = 0; i < r; ++i) {
        string nom, action;
        cin >> nom >> action;

        if (infos.find(nom) == infos.end()) {
            ordre.push_back(nom);
            infos[nom] = InfoReglage{};
        }

        if (action == "GETENV") {
            infos[nom].aGetenv = true;
        } else if (action == "LIT") {
            infos[nom].aLit = true;
        } else if (action == "ECRIT") {
            infos[nom].aEcrit = true;
        } else if (action == "ENV") {
            infos[nom].aEnv = true;
        }
    }

    int nbMenu = 0;
    for (const string& nom : ordre) {
        const InfoReglage& reg = infos[nom];
        string classe;

        if (reg.aGetenv) {
            classe = "CACHE";
        } else if (!reg.aLit) {
            classe = "FANTOME";
        } else if (!reg.aEcrit && !reg.aEnv) {
            classe = "CONSTANTE";
        } else {
            classe = "MENU";
            nbMenu++;
        }

        cout << nom << " " << classe << "\n";
    }

    cout << "MENU " << nbMenu << " SUR " << ordre.size() << "\n";

    return 0;
}