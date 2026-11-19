#include <iostream>
#include <string>

using namespace std;

int main() {
    int N, T;
    cin >> N >> T;

    if (N < 1 || N > 4) {
        cout << "CHAINE REFUSEE" << endl;
        return 0;
    }

    string signes = "";

    if (T > 0) {
        cin >> signes;
    }

    int image = 0;
    int fins = 0;
    int rendues = 0;
    int sautees = 0;

    for (int t = 1; t <= T; t++) {
        int rendre = signes[t - 1] - '0';

        if (rendre == 1) {
            int lacet = t - 1;

            cout << t << " OUI 1 " << lacet << " " << image << endl;

            image = (image + 1) % N;
            rendues++;
        } else {
            cout << t << " NON 0" << endl;
            sautees++;
        }

        fins++;
    }

    cout << "FINS " << fins << endl;
    cout << "RENDUES " << rendues << endl;
    cout << "SAUTEES " << sautees << endl;
    cout << "PROCHAINE " << image << endl;

    return 0;
}