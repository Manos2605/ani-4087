#include <iostream>
#include <string>

using namespace std;

class Session {
public:
    bool ouverte = false;
    int debuts = 0;
    int fins = 0;
    int refuses = 0;
    int images = 0;

    bool wait() {
        if (ouverte) {
            refuses++;
            return false;
        }
        return true;
    }

    void begin() {
        debuts++;
        if (ouverte) {
            refuses++;
            return;
        }
        ouverte = true;
    }

    void end_() {
        fins++;
        if (!ouverte) {
            refuses++;
            return;
        }
        ouverte = false;
    }
};

class GardeTrame {
    Session &s;
    bool utilise = false;

public:
    explicit GardeTrame(Session &sess) : s(sess) {}
    ~GardeTrame() {
        if (!utilise) {
            s.end_();
        }
    }
    GardeTrame(const GardeTrame &) = delete;
    GardeTrame &operator=(const GardeTrame &) = delete;
    void utiliser() { utilise = true; }
};

void boucleSans(Session &s, const string &mot) {
    for (int t = 0; t < (int)mot.size(); t++) {
        char c = mot[t];
        bool canRender = (c == 'N');

        if (!s.wait()) {
            if (canRender) {
                c = 'V';
                canRender = false;
            }
        }

        s.begin();

        if (c == 'N') {
            s.end_();
            s.images++;
        } else if (c == 'V') {
            s.end_();
        } else if (c == 'C') {
            continue;
        } else if (c == 'R') {
            s.debuts++;
            return;
        }
    }
}

void boucleAvec(Session &s, const string &mot) {
    for (int t = 0; t < (int)mot.size(); t++) {
        char c = mot[t];
        bool canRender = (c == 'N');

        if (!s.wait()) {
            if (canRender) {
                c = 'V';
                canRender = false;
            }
        }

        s.begin();
        GardeTrame g(s);

        if (c == 'N') {
            g.utiliser();
            s.images++;
        } else if (c == 'V') {
            // la garde ferme dans le destructeur
        } else if (c == 'C') {
            continue;
        } else if (c == 'R') {
            s.debuts++;
            return;
        }
    }
}

int main() {
    int T;
    cin >> T;
    string mot;
    cin >> mot;

    Session sans;
    Session avec;

    boucleSans(sans, mot);
    boucleAvec(avec, mot);

    cout << "SANS DEBUTS " << sans.debuts
         << " FINS " << sans.fins
         << " REFUS " << sans.refuses
         << " IMAGES " << sans.images
         << " OUVERTE " << (sans.ouverte ? "OUI" : "NON") << endl;
    cout << "AVEC DEBUTS " << avec.debuts
         << " FINS " << avec.fins
         << " REFUS " << avec.refuses
         << " IMAGES " << avec.images
         << " OUVERTE " << (avec.ouverte ? "OUI" : "NON") << endl;

    return 0;
}
