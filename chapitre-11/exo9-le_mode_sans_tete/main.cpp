#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    long long hz;
    if (!(cin >> hz)) {
        return 0;
    }

    if (hz <= 1) {
        hz = 72;
    }

    long long periode = 1000000000LL / hz;

    string typePose;
    cin >> typePose;

    long long lacet = 0, tangage = 0, x = 0, y = 1700, z = 0;
    if (typePose == "FIXE") {
        cin >> lacet >> tangage >> x >> y >> z;
    }

    int t;
    cin >> t;

    cout << "PERIODE " << periode << "\n";
    cout << "POSE " << lacet << " " << tangage << " " << x << " " << y << " " << z << "\n";

    if (t == 0) {
        cout << "BRUT IDENTIQUE\n";
        cout << "RELATIF IDENTIQUE\n";
        return 0;
    }

    vector<long long> horlogeA(t);
    vector<long long> horlogeB(t);

    for (int i = 0; i < t; ++i) {
        cin >> horlogeA[i];
    }
    for (int i = 0; i < t; ++i) {
        cin >> horlogeB[i];
    }

    long long preditA1 = horlogeA[0] + periode;
    long long preditB1 = horlogeB[0] + periode;

    int premierBrutDiff = -1;
    int premierRelatifDiff = -1;

    for (int i = 0; i < t; ++i) {
        long long numImage = i + 1;
        long long preditA = horlogeA[i] + periode;
        long long preditB = horlogeB[i] + periode;

        long long brut = preditB - preditA;
        long long relatif = (preditB - preditB1) - (preditA - preditA1);
        long long scripte = numImage * periode;

        if (brut != 0 && premierBrutDiff == -1) {
            premierBrutDiff = i + 1;
        }
        if (relatif != 0 && premierRelatifDiff == -1) {
            premierRelatifDiff = i + 1;
        }

        cout << numImage << " BRUT " << brut << " RELATIF " << relatif << " SCRIPTE " << scripte << "\n";
    }

    if (premierBrutDiff == -1) {
        cout << "BRUT IDENTIQUE\n";
    } else {
        cout << "BRUT DIFFERENT " << premierBrutDiff << "\n";
    }

    if (premierRelatifDiff == -1) {
        cout << "RELATIF IDENTIQUE\n";
    } else {
        cout << "RELATIF DIFFERENT " << premierRelatifDiff << "\n";
    }

    return 0;
}