#include <iostream>

using namespace std;

void tourner(long long &x, long long &z, int k) {
    long long u = x;
    long long v = z;

    if (k == 0) {
        x = u;
        z = v;
    } else if (k == 1) {
        x = v;
        z = -u;
    } else if (k == 2) {
        x = -u;
        z = -v;
    } else {
        x = -v;
        z = u;
    }
}

int main() {
    long long ax, ay, az;
    int ak;
    cin >> ax >> ay >> az >> ak;

    int N;
    cin >> N;

    int justes = 0;
    int fausses = 0;

    for (int i = 1; i <= N; i++) {
        char D, C;
        long long x, y, z;
        int k;
        cin >> D >> C >> x >> y >> z >> k;

        long long localDrX = x;
        long long localDrZ = z;
        tourner(localDrX, localDrZ, ak);

        long long ancreDrX = ax;
        long long ancreDrZ = az;
        tourner(ancreDrX, ancreDrZ, ak);

        long long vraieX, vraieZ;
        if (D == 'L') {
            vraieX = ancreDrX + localDrX;
            vraieZ = ancreDrZ + localDrZ;
        } else {
            vraieX = x;
            vraieZ = z;
        }
        long long vraieY = (D == 'L') ? (ay + y) : y;
        int vraiK = (D == 'L') ? ((ak + k) % 4) : k;

        long long localCrueX = x;
        long long localCrueZ = z;
        tourner(localCrueX, localCrueZ, ak);

        long long ancreCrueX = ax;
        long long ancreCrueZ = az;
        tourner(ancreCrueX, ancreCrueZ, ak);

        long long cruX, cruZ;
        if (C == 'L') {
            cruX = ancreCrueX + localCrueX;
            cruZ = ancreCrueZ + localCrueZ;
        } else {
            cruX = x;
            cruZ = z;
        }
        long long cruY = (C == 'L') ? (ay + y) : y;
        int cruK = (C == 'L') ? ((ak + k) % 4) : k;

        long long dx = cruX - vraieX;
        long long dy = cruY - vraieY;
        long long dz = cruZ - vraieZ;
        int dk = ((cruK - vraiK) + 4) % 4;

        cout << i << " CRU " << cruX << " " << cruY << " " << cruZ << " " << cruK << endl;
        cout << i << " ECART " << dx << " " << dy << " " << dz << " " << dk << endl;

        if (dx == 0 && dy == 0 && dz == 0 && dk == 0) {
            justes++;
        } else {
            fausses++;
        }
    }

    cout << "JUSTES " << justes << endl;
    cout << "FAUSSES " << fausses << endl;

    return 0;
}
