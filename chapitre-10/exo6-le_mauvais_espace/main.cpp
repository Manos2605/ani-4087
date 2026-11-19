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

        long long vraieX, vraieY, vraieZ;
        int vraiK;

        if (D == 'L') {
            long long rx = x;
            long long rz = z;

            tourner(rx, rz, ak);

            vraieX = ax + rx;
            vraieY = ay + y;
            vraieZ = az + rz;
            vraiK = (ak + k) % 4;
        } else {
            vraieX = x;
            vraieY = y;
            vraieZ = z;
            vraiK = k;
        }

        long long cruX, cruY, cruZ;
        int cruK;

        if (C == 'L') {
            long long rx = x;
            long long rz = z;

            tourner(rx, rz, ak);

            cruX = ax + rx;
            cruY = ay + y;
            cruZ = az + rz;
            cruK = (ak + k) % 4;
        } else {
            cruX = x;
            cruY = y;
            cruZ = z;
            cruK = k;
        }

        long long dx = cruX - vraieX;
        long long dy = cruY - vraieY;
        long long dz = cruZ - vraieZ;

        int dk = (cruK - vraiK + 4) % 4;

        cout << i << " CRU "
             << cruX << " " << cruY << " "
             << cruZ << " " << cruK << endl;

        cout << i << " ECART "
             << dx << " " << dy << " "
             << dz << " " << dk << endl;

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