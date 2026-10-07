#include <iostream>
#include <string>
#include <vector>

struct Mur {
    std::string nom;
    long long xmin;
    long long xmax;
    long long zmin;
    long long zmax;
};

struct Angle {
    std::string nom;
    long long xmin;
    long long xmax;
    long long zmin;
    long long zmax;
};

int main() {
    long long L, e;
    std::cin >> L >> e;

    int n;
    std::cin >> n;

    std::vector<Mur> murs;

    for (int i = 0; i < n; i++) {
        std::string nom;
        long long cx, cz, sx, sz;

        std::cin >> nom >> cx >> cz >> sx >> sz;

        Mur mur;
        mur.nom = nom;
        mur.xmin = cx - sx / 2;
        mur.xmax = cx + sx / 2;
        mur.zmin = cz - sz / 2;
        mur.zmax = cz + sz / 2;

        murs.push_back(mur);

        std::cout << nom << " "
                  << mur.xmin << " "
                  << mur.xmax << " "
                  << mur.zmin << " "
                  << mur.zmax << "\n";
    }

    long long h = L / 2;

    // Les quatre carrés dans les angles
    std::vector<Angle> angles = {
        {"FOND_GAUCHE", -h - e, -h, -h - e, -h},
        {"FOND_DROIT", h, h + e, -h - e, -h},
        {"ENTREE_GAUCHE", -h - e, -h, h, h + e},
        {"ENTREE_DROIT", h, h + e, h, h + e}
    };

    int trous = 0;

    for (const Angle& angle : angles) {
        bool bouche = false;

        for (const Mur& mur : murs) {
            bool contient =
                mur.xmin <= angle.xmin &&
                mur.xmax >= angle.xmax &&
                mur.zmin <= angle.zmin &&
                mur.zmax >= angle.zmax;

            if (contient) {
                bouche = true;
                break;
            }
        }

        if (bouche) {
            std::cout << angle.nom << " BOUCHE\n";
        }
        else {
            std::cout << angle.nom << " TROU\n";
            trous++;
        }
    }

    std::cout << "TROUS " << trous << "\n";

    return 0;
}