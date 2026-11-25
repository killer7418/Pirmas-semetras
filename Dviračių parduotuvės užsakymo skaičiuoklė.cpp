#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;

const int MAX_PREKIU = 100;
const int KATEGORIJU_SK = 3;
const string KATEGORIJOS[KATEGORIJU_SK] = {"Dviraciai", "Dalys", "Aksesuarai"};

bool arTinkamaKategorija(string kategorija) {
    for (int k = 0; k < KATEGORIJU_SK; k++)
        if (kategorija == KATEGORIJOS[k])
            return true;
    return false;
}

bool nuskaitytiDuomenis(string failoVardas, string kategorijos[], string pavadinimai[],
                        double kainos[], int kiekiai[], int &n) {
    ifstream fin(failoVardas);
    if (!fin) {
        cout << "Klaida: nepavyko atidaryti failo " << failoVardas << ".\n";
        return false;
    }

    fin >> n;
    if (!fin || n < 1 || n > MAX_PREKIU) {
        cout << "Klaida: irasu skaicius turi buti nuo 1 iki " << MAX_PREKIU << ".\n";
        return false;
    }

    for (int i = 0; i < n; i++) {
        char skyriklis;
        string likutis;

        fin >> ws;
        getline(fin, kategorijos[i], ';');
        getline(fin, pavadinimai[i], ';');
        fin >> kainos[i] >> skyriklis >> kiekiai[i];
        getline(fin, likutis);
        if (!likutis.empty() && likutis[likutis.size() - 1] == '\r')
            likutis.erase(likutis.size() - 1);

        if (!fin || skyriklis != ';' || !likutis.empty()) {
            cout << "Klaida: nepavyko nuskaityti " << (i + 1) << " iraso (faile turi buti " << n << " irasu).\n";
            return false;
        }
        if (!arTinkamaKategorija(kategorijos[i])) {
            cout << "Klaida: " << (i + 1) << " iraso kategorija \"" << kategorijos[i] << "\" netinkama.\n";
            return false;
        }
        if (kainos[i] <= 0) {
            cout << "Klaida: " << (i + 1) << " iraso kaina turi buti teigiama.\n";
            return false;
        }
        if (kiekiai[i] <= 0) {
            cout << "Klaida: " << (i + 1) << " iraso kiekis turi buti teigiamas sveikasis skaicius.\n";
            return false;
        }
    }

    fin.close();
    return true;
}

double eilutesSuma(double kaina, int kiekis) {
    return kaina * kiekis;
}

double nuolaida(double suma, int kiekis) {
    if (kiekis >= 10)
        return suma * 0.10;
    return 0;
}

void spausdintiLentele(ostream &out, string kategorijos[], string pavadinimai[],
                       double kainos[], int kiekiai[], int n, string filtras) {
    out << left << setw(12) << "Kategorija" << setw(47) << "Pavadinimas"
        << right << setw(10) << "Kaina" << setw(8) << "Kiekis"
        << setw(12) << "Suma" << setw(11) << "Nuolaida" << setw(12) << "Moketi" << "\n";
    out << string(112, '-') << "\n";

    for (int i = 0; i < n; i++) {
        if (filtras != "" && kategorijos[i] != filtras)
            continue;
        double suma = eilutesSuma(kainos[i], kiekiai[i]);
        double nuol = nuolaida(suma, kiekiai[i]);
        out << left << setw(12) << kategorijos[i] << setw(47) << pavadinimai[i]
            << right << setw(10) << kainos[i] << setw(8) << kiekiai[i]
            << setw(12) << suma << setw(11) << nuol << setw(12) << suma - nuol << "\n";
    }
}

int ivestiSkaiciu(int nuo, int iki) {
    int x;
    while (true) {
        cin >> x;
        if (cin && x >= nuo && x <= iki)
            return x;
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Neteisingas pasirinkimas. Iveskite skaiciu nuo " << nuo << " iki " << iki << ": ";
    }
}

int main() {
    string kategorijos[MAX_PREKIU];
    string pavadinimai[MAX_PREKIU];
    double kainos[MAX_PREKIU];
    int kiekiai[MAX_PREKIU];
    int n = 0;

    if (!nuskaitytiDuomenis("uzsakymas.txt", kategorijos, pavadinimai, kainos, kiekiai, n)) {
        cout << "Programa baigiama.\n";
        return 1;
    }
    cout << "Nuskaityta prekiu: " << n << "\n";

    cout << fixed << setprecision(2);

    int pasirinkimas;
    do {
        cout << "\n===== DVIRACIU PARDUOTUVES UZSAKYMAS =====\n";
        cout << "1. Perziureti visas uzsakymo prekes\n";
        cout << "2. Perziureti pasirinktos kategorijos prekes\n";
        cout << "3. Perziureti viso uzsakymo sumas\n";
        cout << "4. Perziureti sumas pagal kategorijas\n";
        cout << "5. Parodyti visa uzsakymo ataskaita\n";
        cout << "6. Issaugoti visa ataskaita i faila\n";
        cout << "7. Iseiti\n";
        cout << "Pasirinkite: ";
        pasirinkimas = ivestiSkaiciu(1, 7);
        cout << "\n";

        switch (pasirinkimas) {
            case 1:
                spausdintiLentele(cout, kategorijos, pavadinimai, kainos, kiekiai, n, "");
                break;
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
                cout << "Si funkcija dar neparuosta.\n";
                break;
            case 7:
                cout << "Programa baigta.\n";
                break;
        }
    } while (pasirinkimas != 7);

    return 0;
}
