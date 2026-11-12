
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

struct V3{
    long long x = 0;
    long long y = 0;
    long long z = 0;
};

// Rotation puis ajout de la position de la tête
V3 dansLaSalle(V3 tete, V3 local, int k){
    long long x = local.x;
    long long z = local.z;

    if (k == 1){
        x = local.z;
        z = -local.x;
    }
    else if (k == 2){
        x = -local.x;
        z = -local.z;
    }
    else if (k == 3){
        x = -local.z;
        z = local.x;
    }

    V3 resultat;
    resultat.x = tete.x + x;
    resultat.y = tete.y + local.y;
    resultat.z = tete.z + z;

    return resultat;
}

// Calcul de la distance au carré
long long carre(long long v){
    return v * v;
}

// Recherche de l'objet le plus proche à portée
int objetLePlusProche(vector<V3> objets, V3 pose){
    int choisi = 0;
    long long meilleure = 22501;

    for (int i = 0; i < objets.size(); i++){
        long long dx = objets[i].x - pose.x;
        long long dy = objets[i].y - pose.y;
        long long dz = objets[i].z - pose.z;

        long long d = carre(dx) + carre(dy) + carre(dz);

        if (d <= 22500 && d < meilleure){
            meilleure = d;
            choisi = i + 1;
        }
    }

    return choisi;
}

// Affichage d'une position
void afficherPose(string etiquette, V3 p){
    cout << etiquette << " " << p.x << " "
         << p.y << " " << p.z << endl;
}

// Affichage du résultat de la saisie
void afficherSaisie(string etiquette, int numero){
    cout << etiquette << " ";

    if (numero == 0){
        cout << "AUCUNE" << endl;
    } else {
        cout << numero << endl;
    }
}

int main(){
    int nbTetes, nbObjets;
    cin >> nbTetes >> nbObjets;

    vector<V3> tetes(nbTetes);
    vector<int> lacets(nbTetes);
    vector<V3> objets(nbObjets);

    // Lecture des têtes
    for (int i = 0; i < nbTetes; i++){
        cin >> tetes[i].x >> tetes[i].y
            >> tetes[i].z >> lacets[i];
    }

    // Lecture des objets
    for (int i = 0; i < nbObjets; i++){
        cin >> objets[i].x >> objets[i].y >> objets[i].z;
    }

    // Positions locales des deux mains
    V3 paumeDroite = {180, -300, -250};
    V3 viseeDroite = {180, -250, -400};

    V3 paumeGauche = {-180, -300, -250};
    V3 viseeGauche = {-180, -250, -400};

    // Calcul de la distance entre la paume et la visée
    long long dx = viseeDroite.x - paumeDroite.x;
    long long dy = viseeDroite.y - paumeDroite.y;
    long long dz = viseeDroite.z - paumeDroite.z;

    int ecart = round(sqrt(dx * dx + dy * dy + dz * dz));

    // Calcul des positions pour chaque tête
    for (int i = 0; i < nbTetes; i++){
        V3 paumeD = dansLaSalle(tetes[i], paumeDroite, lacets[i]);
        V3 viseeD = dansLaSalle(tetes[i], viseeDroite, lacets[i]);

        V3 paumeG = dansLaSalle(tetes[i], paumeGauche, lacets[i]);
        V3 viseeG = dansLaSalle(tetes[i], viseeGauche, lacets[i]);

        cout << "TETE " << i + 1 << endl;

        afficherPose("PAUME D", paumeD);
        afficherPose("VISEE D", viseeD);
        afficherPose("PAUME G", paumeG);
        afficherPose("VISEE G", viseeG);

        afficherSaisie("SAISIE PAUME", objetLePlusProche(objets, paumeD));

        afficherSaisie("SAISIE VISEE", objetLePlusProche(objets, viseeD));
    }

    cout << "ECART " << ecart << endl;

    return 0;
}
