#include <iostream>
#include <string>

using namespace std;

int main() {
    int K, N;
    cin >> K >> N;

    int idxG = 0;
    int idxD = 0;

    bool emprunteG = false;
    bool emprunteD = false;
    bool ouverte = false;

    int journal = 0;
    int avertissements = 0;
    int silencieux = 0;

    for (int appel = 1; appel <= N; appel++) {
        string cmd;
        cin >> cmd;

        if (cmd == "wait") {
            if (ouverte) {
                cout << appel << " wait REFUSE" << endl;
                journal++;
            } else {
                cout << appel << " wait OK" << endl;
            }
        }
        else if (cmd == "begin") {
            if (ouverte) {
                cout << appel << " begin REFUSE" << endl;
                journal++;
            } else {
                cout << appel << " begin OK" << endl;
                ouverte = true;
            }
        }
        else if (cmd == "acquire") {
            char c;
            cin >> c;

            if (c == 'G') {
                if (emprunteG) {
                    cout << appel << " acquire REFUSE" << endl;
                    silencieux++;
                } else {
                    cout << appel << " acquire OK " << idxG << endl;
                    emprunteG = true;
                }
            } else {
                if (emprunteD) {
                    cout << appel << " acquire REFUSE" << endl;
                    silencieux++;
                } else {
                    cout << appel << " acquire OK " << idxD << endl;
                    emprunteD = true;
                }
            }
        }
        else if (cmd == "release") {
            char c;
            cin >> c;

            if (c == 'G') {
                if (!emprunteG) {
                    cout << appel << " release REFUSE" << endl;
                    silencieux++;
                } else {
                    cout << appel << " release OK" << endl;
                    idxG = (idxG + 1) % K;
                    emprunteG = false;
                }
            } else {
                if (!emprunteD) {
                    cout << appel << " release REFUSE" << endl;
                    silencieux++;
                } else {
                    cout << appel << " release OK" << endl;
                    idxD = (idxD + 1) % K;
                    emprunteD = false;
                }
            }
        }
        else if (cmd == "end") {
            string extra;
            cin >> extra;

            bool projection = (extra != "-");
            bool g = false;
            bool d = false;

            if (projection) {
                g = (extra == "1");

                int valeurD;
                cin >> valeurD;
                d = (valeurD == 1);
            }

            if (!ouverte) {
                cout << appel << " end REFUSE AUCUNE" << endl;
                journal++;
            }
            else {
                bool bloque = false;

                if (projection && g && emprunteG) {
                    cout << appel << " end REFUSE ACQUISE G" << endl;
                    bloque = true;
                    journal++;
                }
                else if (projection && d && emprunteD) {
                    cout << appel << " end REFUSE ACQUISE D" << endl;
                    bloque = true;
                    journal++;
                }

                if (!bloque) {
                    string sortie = to_string(appel) + " end OK";

                    if (projection) {
                        if (!g) {
                            sortie += " AVERT G";
                            avertissements++;
                        }

                        if (!d) {
                            sortie += " AVERT D";
                            avertissements++;
                        }
                    }

                    cout << sortie << endl;
                }

                ouverte = false;
            }
        }
    }

    cout << "JOURNAL " << journal << endl;
    cout << "AVERTISSEMENTS " << avertissements << endl;
    cout << "SILENCIEUX " << silencieux << endl;

    return 0;
}