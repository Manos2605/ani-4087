#include <iostream>
#include <cstdlib>

using namespace std;

int main() {
    long long w;
    int T;
    cin >> w >> T;

    int fausses = 0;
    long long pireCasque = 0;
    long long pireSimulateur = 0;

    for (int t = 1; t <= T; t++) {
        long long P, S, A;
        char M;
        cin >> P >> S >> A >> M;

        long long lacetRender = w * P;
        long long lacetSoumis = (M == 'R') ? (w * P) : (w * S);
        long long lacetAffiche = w * A;

        long long casque = lacetSoumis - lacetRender;
        long long simulateur = lacetAffiche - lacetRender;

        if (casque != 0) {
            fausses++;
        }

        if (abs(casque) > pireCasque) {
            pireCasque = abs(casque);
        }
        if (abs(simulateur) > pireSimulateur) {
            pireSimulateur = abs(simulateur);
        }

        cout << t << " CASQUE " << casque << " SIMULATEUR " << simulateur << endl;
    }

    cout << "FAUSSES " << fausses << endl;
    cout << "PIRE CASQUE " << pireCasque << endl;
    cout << "PIRE SIMULATEUR " << pireSimulateur << endl;

    return 0;
}
