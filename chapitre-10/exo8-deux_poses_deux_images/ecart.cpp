#include <iostream>
#include <cmath>

using namespace std;

int main() {
    int N;
    long long d;
    cin >> N >> d;

    if (N == 0 || d == 0) {
        cout << "AUCUNE" << endl;
        return 0;
    }

    long long totalPos = 0;
    long long totalAngle = 0;
    long long pirePos = 0;
    long long pireAngle = 0;

    for (int t = 1; t <= N; t++) {
        long long x1, z1, a1;
        long long x2, z2, a2;
        cin >> x1 >> z1 >> a1 >> x2 >> z2 >> a2;

        long long dx = x2 - x1;
        long long dz = z2 - z1;
        long long dist = (long long)round(sqrt(dx * dx + dz * dz));

        long long da = llabs(a2 - a1);

        cout << t << " POSITION " << dist << " ANGLE " << da << endl;

        totalPos += dist;
        totalAngle += da;

        if (dist > pirePos) {
            pirePos = dist;
        }
        if (da > pireAngle) {
            pireAngle = da;
        }
    }

    long long moyPos = totalPos / N;
    long long moyAngle = totalAngle / N;
    long long vitesse = totalAngle / (N * d * 10);

    cout << "MOYENNE POSITION " << moyPos << endl;
    cout << "MOYENNE ANGLE " << moyAngle << endl;
    cout << "VITESSE " << vitesse << endl;

    return 0;
}
