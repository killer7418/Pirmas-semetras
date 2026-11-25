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

double kategorijosSuma(string kategorijos[], double kainos[], int kiekiai[], int n, string kategorija) {
    double suma = 0;
    for (int i = 0; i < n; i++) {
        if (kategorijos[i] == kategorija) {
            double s = eilutesSuma(kainos[i], kiekiai[i]);
            suma += s - nuolaida(s, kiekiai[i]);
        }
    }
    return suma;
}

double visoSumaPries(double kainos[], int kiekiai[], int n) {
    double suma = 0;
    for (int i = 0; i < n; i++)
        suma += eilutesSuma(kainos[i], kiekiai[i]);
    return suma;
}

double visoNuolaidos(double kainos[], int kiekiai[], int n) {
    double suma = 0;
    for (int i = 0; i < n; i++)
        suma += nuolaida(eilutesSuma(kainos[i], kiekiai[i]), kiekiai[i]);
    return suma;
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

void spausdintiVisoSumas(ostream &out, double kainos[], int kiekiai[], int n) {
    double pries = visoSumaPries(kainos, kiekiai, n);
    double nuol = visoNuolaidos(kainos, kiekiai, n);
    out << "Suma pries nuolaidas: " << setw(10) << pries << " EUR\n";
    out << "Nuolaidu suma:        " << setw(10) << nuol << " EUR\n";
    out << "Moketina suma:        " << setw(10) << pries - nuol << " EUR\n";
}

void spausdintiKategorijuSumas(ostream &out, string kategorijos[], double kainos[], int kiekiai[], int n) {
    for (int k = 0; k < KATEGORIJU_SK; k++)
        out << left << setw(12) << KATEGORIJOS[k] << right << setw(10)
            << kategorijosSuma(kategorijos, kainos, kiekiai, n, KATEGORIJOS[k]) << " EUR\n";
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
            case 2: {
                cout << "1. Dviraciai\n2. Dalys\n3. Aksesuarai\nPasirinkite kategorija: ";
                string kategorija = KATEGORIJOS[ivestiSkaiciu(1, 3) - 1];
                double suma = kategorijosSuma(kategorijos, kainos, kiekiai, n, kategorija);
                bool yra = false;
                for (int i = 0; i < n; i++)
                    if (kategorijos[i] == kategorija)
                        yra = true;
                if (!yra) {
                    cout << "Kategorijoje " << kategorija << " prekiu nera.\n";
                    break;
                }
                cout << "\n";
                spausdintiLentele(cout, kategorijos, pavadinimai, kainos, kiekiai, n, kategorija);
                cout << "\n" << kategorija << " moketina suma: " << suma << " EUR\n";
                break;
            }
            case 3:
                spausdintiVisoSumas(cout, kainos, kiekiai, n);
                break;
            case 4:
                spausdintiKategorijuSumas(cout, kategorijos, kainos, kiekiai, n);
                break;
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
