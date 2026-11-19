#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    int T;
    cin >> T;

    int justes = 0;
    int fautives = 0;

    for (int image = 1; image <= T; image++) {
        int n;
        cin >> n;

        vector<string> appels;

        for (int i = 0; i < n; i++) {
            string a;
            cin >> a;
            appels.push_back(a);
        }

        bool attendu = false;
        bool commence = false;
        bool terminee = false;
        bool vues = false;
        bool emprunteG = false;
        bool emprunteD = false;

        int fautePlace = 0;
        string fauteCode = "";

        for (int i = 0; i < n; i++) {
            string a = appels[i];

            if (!fauteCode.empty()) {
                continue;
            }

            if (terminee) {
                fautePlace = i + 1;
                fauteCode = "APRES_END";
                continue;
            }

            if (a == "WAIT") {
                if (commence) {
                    fautePlace = i + 1;
                    fauteCode = "WAIT_APRES_BEGIN";
                } else {
                    attendu = true;
                }
            }
            else if (a == "BEGIN") {
                if (!attendu) {
                    fautePlace = i + 1;
                    fauteCode = "BEGIN_SANS_WAIT";
                } else if (commence) {
                    fautePlace = i + 1;
                    fauteCode = "DOUBLE_BEGIN";
                } else {
                    commence = true;
                }
            }
            else if (a == "VUES") {
                if (!attendu) {
                    fautePlace = i + 1;
                    fauteCode = "VUES_SANS_WAIT";
                } else {
                    vues = true;
                }
            }
            else if (a == "PREND_G") {
                if (!commence) {
                    fautePlace = i + 1;
                    fauteCode = "PREND_SANS_BEGIN";
                }
                else if (!vues) {
                    fautePlace = i + 1;
                    fauteCode = "PREND_SANS_VUES";
                }
                else if (emprunteG) {
                    fautePlace = i + 1;
                    fauteCode = "DOUBLE_PREND";
                } else {
                    emprunteG = true;
                }
            }
            else if (a == "PREND_D") {
                if (!commence) {
                    fautePlace = i + 1;
                    fauteCode = "PREND_SANS_BEGIN";
                }
                else if (!vues) {
                    fautePlace = i + 1;
                    fauteCode = "PREND_SANS_VUES";
                }
                else if (emprunteD) {
                    fautePlace = i + 1;
                    fauteCode = "DOUBLE_PREND";
                } else {
                    emprunteD = true;
                }
            }
            else if (a == "LACHE_G") {
                if (!emprunteG) {
                    fautePlace = i + 1;
                    fauteCode = "LACHE_SANS_PREND";
                } else {
                    emprunteG = false;
                }
            }
            else if (a == "LACHE_D") {
                if (!emprunteD) {
                    fautePlace = i + 1;
                    fauteCode = "LACHE_SANS_PREND";
                } else {
                    emprunteD = false;
                }
            }
            else if (a == "END") {
                if (!commence) {
                    fautePlace = i + 1;
                    fauteCode = "END_SANS_BEGIN";
                }
                else if (emprunteG || emprunteD) {
                    fautePlace = i + 1;
                    fauteCode = "END_IMAGE_PRISE";
                } else {
                    commence = false;
                    vues = false;
                    terminee = true;
                }
            }
        }

        if (fauteCode.empty() && !terminee) {
            if (commence || !terminee) {
                fautePlace = n + 1;
                fauteCode = "SANS_END";
            }
        }

        if (fauteCode.empty()) {
            cout << image << " OK" << endl;
            justes++;
        } else {
            cout << image << " FAUTE " << fautePlace << " " << fauteCode << endl;
            fautives++;
        }
    }

    cout << "JUSTES " << justes << endl;
    cout << "FAUTIVES " << fautives << endl;

    return 0;
}