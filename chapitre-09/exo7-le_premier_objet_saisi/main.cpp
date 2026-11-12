
#include <iostream>
#include <vector>
using namespace std;

struct Pose {
    long long x = 0;
    long long y = 0;
    int k = 0;
};

// Rotation d'une position selon l'angle k
void tourner(long long u, long long v, int k, long long &rx, long long &ry){
    if (k == 0){
        rx = u;
        ry = v;
    }
    else if (k == 1){
        rx = -v;
        ry = u;
    }
    else if (k == 2){
        rx = -u;
        ry = -v;
    }
    else {
        rx = v;
        ry = -u;
    }
}

// Composition de deux poses
Pose composer(Pose a, Pose b){
    Pose resultat;

    long long rx, ry;
    tourner(b.x, b.y, a.k, rx, ry);

    resultat.x = a.x + rx;
    resultat.y = a.y + ry;
    resultat.k = (a.k + b.k) % 4;

    return resultat;
}

// Calcul de la pose inverse
Pose inverser(Pose a){
    Pose resultat;

    resultat.k = (4 - a.k) % 4;

    long long rx, ry;
    tourner(-a.x, -a.y, resultat.k, rx, ry);

    resultat.x = rx;
    resultat.y = ry;

    return resultat;
}

int main(){
    int m, t;
    cin >> m >> t;

    vector<Pose> objets(m);

    // Lecture des positions initiales des objets
    for (int i = 0; i < m; i++){
        cin >> objets[i].x >> objets[i].y >> objets[i].k;
    }

    int tenu = -1;
    Pose attache;

    int precedent = 0;

    int saisies = 0;
    int ratees = 0;

    // Traitement de chaque image
    for (int image = 1; image <= t; image++){
        Pose paume;
        int bouton;

        cin >> paume.x >> paume.y >> paume.k >> bouton;

        bool appui = (bouton == 1 && precedent == 0);
        bool relachement = (bouton == 0 && precedent == 1);

        bool rate = false;

        precedent = bouton;

        // Recherche de l'objet le plus proche lors de l'appui
        if (appui){
            tenu = -1;

            long long meilleure = 0;

            for (int i = 0; i < m; i++){
                long long dx = objets[i].x - paume.x;
                long long dy = objets[i].y - paume.y;

                long long distance = dx * dx + dy * dy;

                if (distance <= 22500 &&
                    (tenu == -1 || distance < meilleure)){
                    tenu = i;
                    meilleure = distance;
                }
            }

            // Enregistrement de la pose relative de l'objet
            if (tenu != -1){
                attache = composer(inverser(paume), objets[tenu]);
                saisies++;
            }
            else {
                ratees++;
                rate = true;
            }
        }

        // Relâchement de l'objet
        if (relachement){
            tenu = -1;
        }

        // Mise à jour de la position de l'objet tenu
        if (tenu != -1){
            objets[tenu] = composer(paume, attache);

            cout << image << " TIENT " << tenu + 1 << " "
                 << objets[tenu].x << " " << objets[tenu].y
                 << " " << objets[tenu].k << endl;
        }
        else if (rate){
            cout << image << " RATE" << endl;
        }
        else {
            cout << image << " LIBRE" << endl;
        }
    }

    // Affichage des statistiques
    cout << "SAISIES " << saisies << endl;
    cout << "RATEES " << ratees << endl;

    // Affichage des positions finales des objets
    for (int i = 0; i < m; i++){
        cout << "OBJET " << i + 1 << " "
             << objets[i].x << " " << objets[i].y
             << " " << objets[i].k << endl;
    }

    return 0;
}
