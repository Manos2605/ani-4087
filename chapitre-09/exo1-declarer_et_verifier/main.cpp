
#include <iostream>
#include <vector>
#include <string>
#include <set>
using namespace std;

int numeroUsage(string usage){
    string noms[] = {
        "SELECT", "GRAB", "MENU", "MOVE", "AIM_POSE", "GRIP_POSE", "HAPTIC",
        "SELECT_LEFT", "GRAB_LEFT", "AIM_POSE_LEFT", "GRIP_POSE_LEFT",
        "HAPTIC_LEFT", "MOVE_RIGHT"
    };

    for (int i = 0; i < 13; i++){
        if (usage == noms[i]){
            return i;
        }
    }

    return -1;
}

int main(){
    int n;
    cin >> n;

    vector<string> noms;
    set<string> dejaVus;

    int doubles = 0;
    int attachees = 0;
    bool attacheFait = false;

    for (int i = 0; i < n; i++){
        string ordre;
        cin >> ordre;

        if (ordre == "cree"){
            string nom, type, usage;
            cin >> nom >> type >> usage;

            noms.push_back(nom);

            if (dejaVus.count(nom) > 0){
                doubles++;
            } else {
                dejaVus.insert(nom);
            }

            cout << "ACTION " << noms.size() << " " << nom << " "
                 << type << " " << numeroUsage(usage) << endl;
        }

        else if (ordre == "attache"){
            if (attacheFait){
                cout << "ATTACHE REFUSE" << endl;
            } else {
                attacheFait = true;
                attachees = noms.size();
                cout << "ATTACHE " << attachees << endl;
            }
        }

        else if (ordre == "lit"){
            unsigned long long h;
            cin >> h;

            if (h == 0 || h > noms.size()){
                cout << "LIT " << h << " INVALIDE" << endl;
            }
            else if (!attacheFait || h > attachees){
                cout << "LIT " << h << " INCONNUE" << endl;
            }
            else {
                cout << "LIT " << h << " " << noms[h - 1] << endl;
            }
        }
    }

    cout << "ACTIONS " << noms.size() << endl;
    cout << "ATTACHEES " << attachees << endl;
    cout << "NOMS EN DOUBLE " << doubles << endl;

    return 0;
}
