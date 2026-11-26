#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cctype>

using namespace std;

// Lit un entier selon la regle 4 depuis la position pos (mise a jour apres lecture)
bool lireEntier(const string& texte, size_t& pos, long long& resultat) {
    while (pos < texte.size() && texte[pos] == ' ') {
        pos++;
    }

    int signe = 1;
    if (pos < texte.size() && (texte[pos] == '+' || texte[pos] == '-')) {
        if (texte[pos] == '-') {
            signe = -1;
        }
        pos++;
    }

    long long valeur = 0;
    bool aChiffre = false;
    while (pos < texte.size() && isdigit(static_cast<unsigned char>(texte[pos]))) {
        aChiffre = true;
        valeur = valeur * 10 + (texte[pos] - '0');
        pos++;
    }

    if (!aChiffre) {
        resultat = 0;
        return false;
    }

    resultat = signe * valeur;
    return true;
}

string lireInterrupteur(const string& val) {
    if (val[0] == '0' || val == "false") {
        return "NON";
    }
    return "OUI";
}

int main() {
    string backend, demi, profondeur, liste;
    long long hz, latence;
    if (!(cin >> backend >> hz >> demi >> profondeur >> liste >> latence)) {
        return 0;
    }

    string poubelle;
    getline(cin, poubelle);

    string loader;
    getline(cin, loader);

    string lignePose;
    getline(cin, lignePose);

    bool poseFixe = false;
    long long pose[5] = {0, 0, 0, 0, 0};

    if (lignePose != "LIBRE") {
        poseFixe = true;
        stringstream ss(lignePose);
        string motFixe;
        ss >> motFixe;
        for (int i = 0; i < 5; ++i) {
            ss >> pose[i];
        }
    }

    int m;
    cin >> m;
    getline(cin, poubelle);

    int appliquees = 0;
    int ignorees = 0;

    for (int i = 0; i < m; ++i) {
        string ligne;
        getline(cin, ligne);

        size_t posEgal = ligne.find('=');
        if (posEgal == string::npos) {
            ignorees++;
            continue;
        }

        string nom = ligne.substr(0, posEgal);
        string val = ligne.substr(posEgal + 1);

        if (val.empty()) {
            ignorees++;
            continue;
        }

        if (nom == "NK_XR_BACKEND") {
            if (val == "openxr") {
                backend = "OPENXR";
                appliquees++;
            } else {
                ignorees++;
            }
        } else if (nom == "NK_XR_HZ") {
            size_t pos = 0;
            long long nb = 0;
            lireEntier(val, pos, nb);
            hz = nb;
            appliquees++;
        } else if (nom == "NK_XR_SIM_LATENCY_MS") {
            size_t pos = 0;
            long long nb = 0;
            lireEntier(val, pos, nb);
            latence = nb;
            appliquees++;
        } else if (nom == "NK_XR_HALF_RATE") {
            demi = lireInterrupteur(val);
            appliquees++;
        } else if (nom == "NK_XR_DEPTH_LAYER") {
            profondeur = lireInterrupteur(val);
            appliquees++;
        } else if (nom == "NK_XR_LIST_EXT") {
            liste = lireInterrupteur(val);
            appliquees++;
        } else if (nom == "NK_XR_OPENXR_LOADER") {
            loader = val;
            appliquees++;
        } else if (nom == "NK_XR_SIM_POSE") {
            size_t pos = 0;
            vector<long long> nombres;
            while (nombres.size() < 5) {
                long long nb = 0;
                if (!lireEntier(val, pos, nb)) {
                    break;
                }
                nombres.push_back(nb);
                while (pos < val.size() && (val[pos] == ',' || val[pos] == ' ')) {
                    pos++;
                }
            }

            if (nombres.size() == 5) {
                poseFixe = true;
                for (int j = 0; j < 5; ++j) {
                    pose[j] = nombres[j];
                }
                appliquees++;
            } else {
                ignorees++;
            }
        } else {
            ignorees++;
        }
    }

    cout << "BACKEND " << backend << "\n";
    cout << "HZ " << hz << "\n";
    cout << "DEMI " << demi << "\n";
    cout << "PROFONDEUR " << profondeur << "\n";
    cout << "LISTE " << liste << "\n";
    cout << "LOADER " << loader << "\n";
    cout << "LATENCE " << latence << "\n";

    if (!poseFixe) {
        cout << "POSE LIBRE\n";
    } else {
        cout << "POSE FIXE " << pose[0] << " " << pose[1] << " " << pose[2] << " " << pose[3] << " " << pose[4] << "\n";
    }

    cout << "APPLIQUEES " << appliquees << "\n";
    cout << "IGNOREES " << ignorees << "\n";

    return 0;
}