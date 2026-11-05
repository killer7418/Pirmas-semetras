#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

const int MAX_MOKINIU = 100;
const int MAX_PAZYMIU = 10;

int ivestiPazymi() {
    int pazymys;
    do {
        cin >> pazymys;
        if (pazymys < 1 || pazymys > 10)
            cout << "Neteisingas pazymys. Iveskite skaiciu nuo 1 iki 10: ";
    } while (pazymys < 1 || pazymys > 10);
    return pazymys;
}

int ivestiNumeri(string kas, int kiekis) {
    int nr;
    do {
        cout << "Iveskite " << kas << " numeri (0 - grizti i meniu): ";
        cin >> nr;
        if (nr == 0)
            return 0;
        if (nr < 1 || nr > kiekis)
            cout << "Neteisingas " << kas << " numeris. Iveskite skaiciu nuo 1 iki " << kiekis << ".\n";
    } while (nr < 1 || nr > kiekis);
    return nr;
}

int main() {
    string vardai[MAX_MOKINIU];
    int pazymiai[MAX_MOKINIU][MAX_PAZYMIU];
    int pazymiuSk[MAX_MOKINIU];
    int mokiniuSk = 0;
    int pasirinkimas;

    cout << fixed << setprecision(2);

    do {
        cout << "\n===== MOKINIU PAZYMIU SISTEMA =====\n";
        cout << "1. Ivesti mokini ir jo pazymius\n";
        cout << "2. Perziureti visu mokiniu pazymius\n";
        cout << "3. Perziureti konkretaus mokinio pazymius\n";
        cout << "4. Atnaujinti pazymi\n";
        cout << "5. Apskaiciuoti mokinio vidurki\n";
        cout << "6. Pasalinti mokini\n";
        cout << "7. Iseiti\n";
        cout << "Pasirinkite: ";
        cin >> pasirinkimas;

        if (pasirinkimas >= 2 && pasirinkimas <= 6 && mokiniuSk == 0) {
            cout << "Sarase nera mokiniu.\n";
            continue;
        }

        switch (pasirinkimas) {
            case 1: {
                if (mokiniuSk >= MAX_MOKINIU) {
                    cout << "Pasiektas mokiniu limitas (" << MAX_MOKINIU << ").\n";
                    break;
                }
                cout << "Iveskite mokinio varda: ";
                cin >> vardai[mokiniuSk];

                int kiek;
                cout << "Kiek pazymiu ivesite (0-" << MAX_PAZYMIU << ")? ";
                cin >> kiek;
                while (kiek < 0 || kiek > MAX_PAZYMIU) {
                    cout << "Neteisingas kiekis. Iveskite skaiciu nuo 0 iki " << MAX_PAZYMIU << ": ";
                    cin >> kiek;
                }

                for (int j = 0; j < kiek; j++) {
                    cout << (j + 1) << " pazymys: ";
                    pazymiai[mokiniuSk][j] = ivestiPazymi();
                }
                pazymiuSk[mokiniuSk] = kiek;
                mokiniuSk++;
                cout << "Mokinys pridetas.\n";
                break;
            }
            case 2: {
                for (int i = 0; i < mokiniuSk; i++) {
                    cout << (i + 1) << ". " << vardai[i] << ": ";
                    if (pazymiuSk[i] == 0)
                        cout << "pazymiu nera";
                    for (int j = 0; j < pazymiuSk[i]; j++)
                        cout << pazymiai[i][j] << " ";
                    cout << "\n";
                }
                break;
            }
            case 3: {
                int nr = ivestiNumeri("mokinio", mokiniuSk);
                if (nr == 0)
                    break;
                int i = nr - 1;
                cout << vardai[i] << ": ";
                if (pazymiuSk[i] == 0)
                    cout << "pazymiu nera";
                for (int j = 0; j < pazymiuSk[i]; j++)
                    cout << pazymiai[i][j] << " ";
                cout << "\n";
                break;
            }
            case 4: {
                int nr = ivestiNumeri("mokinio", mokiniuSk);
                if (nr == 0)
                    break;
                int i = nr - 1;
                if (pazymiuSk[i] == 0) {
                    cout << "Mokinys " << vardai[i] << " pazymiu neturi.\n";
                    break;
                }
                cout << vardai[i] << " pazymiai: ";
                for (int j = 0; j < pazymiuSk[i]; j++)
                    cout << (j + 1) << ") " << pazymiai[i][j] << "  ";
                cout << "\n";

                int pnr = ivestiNumeri("pazymio", pazymiuSk[i]);
                if (pnr == 0)
                    break;
                cout << "Iveskite nauja pazymi: ";
                pazymiai[i][pnr - 1] = ivestiPazymi();
                cout << "Pazymys atnaujintas.\n";
                break;
            }
            case 5: {
                int nr = ivestiNumeri("mokinio", mokiniuSk);
                if (nr == 0)
                    break;
                int i = nr - 1;
                if (pazymiuSk[i] == 0) {
                    cout << "Mokinys " << vardai[i] << " pazymiu neturi, vidurkio apskaiciuoti negalima.\n";
                    break;
                }
                int suma = 0;
                for (int j = 0; j < pazymiuSk[i]; j++)
                    suma += pazymiai[i][j];
                double vidurkis = (double)suma / pazymiuSk[i];
                cout << vardai[i] << " vidurkis: " << vidurkis << "\n";
                break;
            }
            case 6: {
                int nr = ivestiNumeri("mokinio", mokiniuSk);
                if (nr == 0)
                    break;
                int i = nr - 1;
                cout << "Mokinys " << vardai[i] << " pasalintas.\n";
                for (int k = i; k < mokiniuSk - 1; k++) {
                    vardai[k] = vardai[k + 1];
                    pazymiuSk[k] = pazymiuSk[k + 1];
                    for (int j = 0; j < MAX_PAZYMIU; j++)
                        pazymiai[k][j] = pazymiai[k + 1][j];
                }
                mokiniuSk--;
                break;
            }
            case 7:
                cout << "Programa baigta.\n";
                break;
            default:
                cout << "Neteisingas pasirinkimas. Iveskite skaiciu nuo 1 iki 7.\n";
        }

    } while (pasirinkimas != 7);

    return 0;
}
