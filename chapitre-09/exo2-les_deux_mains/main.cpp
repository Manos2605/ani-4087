
#include <iostream>
#include <vector>
#include <string>
#include <set>
using namespace std;

struct Lien {
    string main;
    string commande;
};

// Association des usages aux commandes
Lien lienPour(string usage){
    if (usage == "SELECT") return {"DROITE", "SOURIS_G"};
    if (usage == "GRAB") return {"DROITE", "SOURIS_D"};
    if (usage == "MENU") return {"GAUCHE", "TAB"};
    if (usage == "MOVE") return {"GAUCHE", "FLECHES"};
    if (usage == "AIM_POSE") return {"DROITE", "AUCUNE"};
    if (usage == "GRIP_POSE") return {"DROITE", "AUCUNE"};
    if (usage == "HAPTIC") return {"DROITE", "AUCUNE"};
    if (usage == "SELECT_LEFT") return {"GAUCHE", "SOURIS_M"};
    if (usage == "GRAB_LEFT") return {"GAUCHE", "MAJ_G"};
    if (usage == "AIM_POSE_LEFT") return {"GAUCHE", "AUCUNE"};
    if (usage == "GRIP_POSE_LEFT") return {"GAUCHE", "AUCUNE"};
    if (usage == "HAPTIC_LEFT") return {"GAUCHE", "AUCUNE"};
    if (usage == "MOVE_RIGHT") return {"DROITE", "IJKL"};

    return {"DROITE", "AUCUNE"};
}

// Calcul du déplacement en X et Y
string manche(set<string> tenues, string droite, string gauche,
              string haut, string bas){
    int x = 0;
    int y = 0;

    if (tenues.count(droite)) x++;
    if (tenues.count(gauche)) x--;
    if (tenues.count(haut)) y++;
    if (tenues.count(bas)) y--;

    return to_string(x) + "," + to_string(y);
}

int main(){
    int a, t;
    cin >> a >> t;

    vector<string> noms(a);
    vector<Lien> liens(a);

    int droite = 0;
    int gauche = 0;

    // Lecture des actions et de leurs commandes
    for (int i = 0; i < a; i++){
        string usage;
        cin >> noms[i] >> usage;

        liens[i] = lienPour(usage);

        if (liens[i].main == "DROITE"){
            droite++;
        } else {
            gauche++;
        }
    }

    // Affichage des associations
    for (int i = 0; i < a; i++){
        cout << noms[i] << " " << liens[i].main << " "
             << liens[i].commande << endl;
    }

    int imagesTete = 0;

    // Lecture des commandes pour chaque image
    for (int image = 1; image <= t; image++){
        int k;
        cin >> k;

        set<string> tenues;

        for (int j = 0; j < k; j++){
            string commande;
            cin >> commande;
            tenues.insert(commande);
        }

        // Vérification des touches qui déplacent la tête
        if (tenues.count("Z") || tenues.count("Q") ||
            tenues.count("S") || tenues.count("D") ||
            tenues.count("W") || tenues.count("A")){
            imagesTete++;
        }

        cout << image;

        // Calcul de la valeur de chaque action
        for (int i = 0; i < a; i++){
            string commande = liens[i].commande;
            string valeur;

            if (commande == "FLECHES"){
                valeur = manche(tenues, "F_DROITE", "F_GAUCHE",
                                "F_HAUT", "F_BAS");
            }
            else if (commande == "IJKL"){
                valeur = manche(tenues, "L", "J", "I", "K");
            }
            else if (commande == "AUCUNE"){
                valeur = "-";
            }
            else {
                if (tenues.count(commande)){
                    valeur = "1";
                } else {
                    valeur = "0";
                }
            }

            cout << " " << valeur;
        }

        cout << endl;
    }

    // Affichage des statistiques
    cout << "DROITE " << droite << endl;
    cout << "GAUCHE " << gauche << endl;
    cout << "TETE " << imagesTete << endl;

    return 0;
}
