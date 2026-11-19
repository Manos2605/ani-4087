#include <iostream>
#include <cmath>

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

    long long ex, ey, ez;
    cin >> ex >> ey >> ez;

    int N;
    cin >> N;

    for (int i = 1; i <= N; i++) {
        char E;
        long long x, y, z, w, h;
        int k;
        cin >> E >> x >> y >> z >> k >> w >> h;

        long long centreX, centreY, centreZ;
        int centreK;

        if (E == '-') {
            cout << "PANNEAU " << i << " LOCAL" << endl;
            centreX = ax;
            centreY = ay;
            centreZ = az;

            long long localX = x;
            long long localZ = z;
            tourner(localX, localZ, ak);

            centreX += localX;
            centreY += y;
            centreZ += localZ;
            centreK = (ak + k) % 4;
        } else if (E == 'S') {
            cout << "PANNEAU " << i << " STAGE" << endl;
            centreX = x;
            centreY = y;
            centreZ = z;
            centreK = k;
        } else {
            cout << "PANNEAU " << i << " LOCAL" << endl;
            centreX = ax;
            centreY = ay;
            centreZ = az;

            long long localX = x;
            long long localZ = z;
            tourner(localX, localZ, ak);

            centreX += localX;
            centreY += y;
            centreZ += localZ;
            centreK = (ak + k) % 4;
        }

        long long coinX[4];
        long long coinY[4];
        long long coinZ[4];

        long long cxx[4] = {-w / 2, w / 2, w / 2, -w / 2};
        long long cyy[4] = {-h / 2, -h / 2, h / 2, h / 2};
        long long czz[4] = {0, 0, 0, 0};

        for (int c = 0; c < 4; c++) {
            long long rx = cxx[c];
            long long rz = czz[c];
            tourner(rx, rz, centreK);

            coinX[c] = centreX + rx;
            coinY[c] = centreY + cyy[c];
            coinZ[c] = centreZ + rz;
        }

        for (int c = 0; c < 4; c++) {
            cout << coinX[c] << " " << coinY[c] << " " << coinZ[c] << endl;
        }

        double dx = centreX - ex;
        double dy = centreY - ey;
        double dz = centreZ - ez;
        long long dist = (long long)round(sqrt(dx * dx + dy * dy + dz * dz));
        cout << "DISTANCE " << dist << endl;
    }

    cout << "PANNEAUX " << N << endl;

    return 0;
}
