#include <iostream>

using namespace std;

int main() {
    long long hz, ipd;
    int n;
    if (!(cin >> hz >> ipd >> n)) {
        return 0;
    }

    long long x, y, z, k0, dk, vx;
    cin >> x >> y >> z >> k0 >> dk >> vx;

    if (hz <= 1) {
        hz = 72;
    }

    long long periode = 1000000000LL / hz;
    long long demiIpd = ipd / 2;

    cout << "PERIODE " << periode << "\n";

    // Direction de la droite de la tete en (x, z) selon le lacet (0, 1, 2, 3)
    long long droiteX[4] = {1, 0, -1, 0};
    long long droiteZ[4] = {0, -1, 0, 1};

    for (int t = 1; t <= n; ++t) {
        long long pas = t - 1;

        long long lacet = (k0 + pas * dk) % 4;
        if (lacet < 0) {
            lacet += 4;
        }

        long long teteX = x + pas * vx;
        long long teteY = y;
        long long teteZ = z;

        long long dx = droiteX[lacet];
        long long dz = droiteZ[lacet];

        long long gaucheX = teteX - demiIpd * dx;
        long long gaucheY = teteY;
        long long gaucheZ = teteZ - demiIpd * dz;

        long long droitX = teteX + demiIpd * dx;
        long long droitY = teteY;
        long long droitZ = teteZ + demiIpd * dz;

        long long instant = t * periode;

        cout << t
             << " G " << gaucheX << " " << gaucheY << " " << gaucheZ
             << " D " << droitX << " " << droitY << " " << droitZ
             << " I " << instant << "\n";
    }

    return 0;
}