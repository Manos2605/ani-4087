#include <iostream>
#include <string>

struct Position {
    long long x;
    long long y;
    long long z;
};

Position PoserAuSol(long long sy, long long x, long long z) {
    return {x, sy / 2, z};
}

Position PoserSurTable(long long sy, long long H, long long x, long long z) {
    return {x, H + sy / 2, z};
}

int main() {
    long long L, P, H, ep, pied, tx, tz;
    std::cin >> L >> P >> H >> ep >> pied >> tx >> tz;

    // Centre du plateau
    long long plateauY = H - ep / 2;

    std::cout << "PLATEAU "
              << tx << " "
              << plateauY << " "
              << tz << "\n";

    // Hauteur et décalage des pieds
    long long hauteurPied = H - ep;
    long long decalageX = L / 2 - pied;
    long long decalageZ = P / 2 - pied;

    Position pied1 = PoserAuSol(hauteurPied, tx - decalageX, tz - decalageZ);
    Position pied2 = PoserAuSol(hauteurPied, tx + decalageX, tz - decalageZ);
    Position pied3 = PoserAuSol(hauteurPied, tx - decalageX, tz + decalageZ);
    Position pied4 = PoserAuSol(hauteurPied, tx + decalageX, tz + decalageZ);

    std::cout << "PIED " << pied1.x << " " << pied1.y << " " << pied1.z << "\n";
    std::cout << "PIED " << pied2.x << " " << pied2.y << " " << pied2.z << "\n";
    std::cout << "PIED " << pied3.x << " " << pied3.y << " " << pied3.z << "\n";
    std::cout << "PIED " << pied4.x << " " << pied4.y << " " << pied4.z << "\n";

    int n;
    std::cin >> n;

    for (int i = 0; i < n; i++) {
        std::string nom, ou;
        long long sx, sy, sz, x, z;

        std::cin >> nom >> sx >> sy >> sz >> x >> z >> ou;

        Position position;

        if (ou == "SOL") {
            position = PoserAuSol(sy, x, z);
        }
        else {
            position = PoserSurTable(sy, H, x, z);
        }

        std::cout << nom << " " << position.x << " " << position.y << " " << position.z << "\n";
    }

    return 0;
}