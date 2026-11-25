// Dviračių parduotuvės užsakymo apdorojimo programa.
// Programa nuskaito užsakymo duomenis iš failo uzsakymas.txt,
// apskaičiuoja sumas, nuolaidas, kategorijų sumas ir leidžia
// ataskaitą parodyti ekrane bei išsaugoti faile ataskaita.txt.

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cmath>

using namespace std;

// Konstantos (ne kintamieji, todėl duomenys tarp funkcijų jomis neperduodami)
const int MAX_PREKIU = 100;          // didžiausias galimas prekių įrašų skaičius
const int MIN_PREKIU = 1;            // mažiausias galimas prekių įrašų skaičius
const int KATEGORIJU_SK = 3;
const string KATEGORIJOS[KATEGORIJU_SK] = {"Dviraciai", "Dalys", "Aksesuarai"};
const int NUOLAIDOS_KIEKIS = 10;     // nuo tiek vienetų taikoma nuolaida
const double NUOLAIDOS_DALIS = 0.10; // 10 % nuolaida
const string DUOMENU_FAILAS = "uzsakymas.txt";
const string ATASKAITOS_FAILAS = "ataskaita.txt";
const int ISEITI = 7;                // meniu punktas „Išeiti“

// Lentelės stulpelių pločiai
const int PL_NR = 4;
const int PL_KATEGORIJA = 12;
const int PL_PAVADINIMAS = 47;
const int PL_KAINA = 11;
const int PL_KIEKIS = 8;
const int PL_SUMA = 12;
const int PL_NUOLAIDA = 11;
const int PL_MOKETI = 12;
const int LENTELES_PLOTIS = PL_NR + PL_KATEGORIJA + PL_PAVADINIMAS + PL_KAINA
                            + PL_KIEKIS + PL_SUMA + PL_NUOLAIDA + PL_MOKETI;

// ---------------------------------------------------------------------
// Pagalbinės teksto tikrinimo funkcijos
// ---------------------------------------------------------------------

// Pašalina tarpus, tabuliacijas ir '\r' simbolius iš teksto pradžios ir pabaigos
string apkarpyti(string tekstas)
{
    int pradzia = 0;
    int pabaiga = (int)tekstas.length() - 1;
    while (pradzia <= pabaiga &&
           (tekstas[pradzia] == ' ' || tekstas[pradzia] == '\t' || tekstas[pradzia] == '\r'))
    {
        pradzia++;
    }
    while (pabaiga >= pradzia &&
           (tekstas[pabaiga] == ' ' || tekstas[pabaiga] == '\t' || tekstas[pabaiga] == '\r'))
    {
        pabaiga--;
    }
    return tekstas.substr(pradzia, pabaiga - pradzia + 1);
}

// Tikrina, ar tekstas yra neneigiamas sveikasis skaičius; jei taip, grąžina jį per parametrą
bool arSveikasisSkaicius(string tekstas, int &skaicius)
{
    if (tekstas.length() == 0 || tekstas.length() > 9)
    {
        return false;
    }
    for (int i = 0; i < (int)tekstas.length(); i++)
    {
        if (tekstas[i] < '0' || tekstas[i] > '9')
        {
            return false;
        }
    }
    skaicius = stoi(tekstas);
    return true;
}

// Tikrina, ar tekstas yra realusis skaičius su tašku (pvz. 950.00); jei taip, grąžina jį per parametrą
bool arRealusisSkaicius(string tekstas, double &skaicius)
{
    int taskuSk = 0;
    int skaitmenuSk = 0;
    if (tekstas.length() == 0 || tekstas.length() > 15)
    {
        return false;
    }
    for (int i = 0; i < (int)tekstas.length(); i++)
    {
        if (tekstas[i] == '.')
        {
            taskuSk++;
        }
        else if (tekstas[i] >= '0' && tekstas[i] <= '9')
        {
            skaitmenuSk++;
        }
        else
        {
            return false;
        }
    }
    if (taskuSk > 1 || skaitmenuSk == 0)
    {
        return false;
    }
    skaicius = stod(tekstas);
    return true;
}

// Tikrina, ar kategorija yra viena iš trijų leistinų
bool arTinkamaKategorija(string kategorija)
{
    for (int i = 0; i < KATEGORIJU_SK; i++)
    {
        if (kategorija == KATEGORIJOS[i])
        {
            return true;
        }
    }
    return false;
}

// Išskaido eilutę pagal kabliataškius į lygiai 4 laukus. Grąžina false, jei laukų ne 4.
bool isskaidytiEilute(string eilute, string laukai[])
{
    int laukoNr = 0;
    laukai[0] = "";
    for (int i = 0; i < (int)eilute.length(); i++)
    {
        if (eilute[i] == ';')
        {
            laukoNr++;
            if (laukoNr > 3)
            {
                return false;
            }
            laukai[laukoNr] = "";
        }
        else
        {
            laukai[laukoNr] = laukai[laukoNr] + eilute[i];
        }
    }
    if (laukoNr != 3)
    {
        return false;
    }
    for (int i = 0; i < 4; i++)
    {
        laukai[i] = apkarpyti(laukai[i]);
    }
    return true;
}

// Išveda klaidos pranešimą apie konkrečią duomenų failo eilutę
void eilutesKlaida(int eilutesNr, string pranesimas)
{
    cout << "KLAIDA duomenu failo " << eilutesNr << " eiluteje: " << pranesimas << endl;
}

// ---------------------------------------------------------------------
// Duomenų nuskaitymas
// ---------------------------------------------------------------------

// Nuskaito užsakymo duomenis į masyvus. Grąžina true tik tada, kai visi įrašai teisingi.
bool nuskaitytiDuomenis(string failoVardas, string kategorijos[], string pavadinimai[],
                        double kainos[], int kiekiai[], int &n)
{
    ifstream fin;
    fin.open(failoVardas.c_str());
    if (fin.fail())
    {
        cout << "KLAIDA: nepavyko atverti duomenu failo \"" << failoVardas << "\"." << endl;
        cout << "Patikrinkite, ar failas yra tame paciame aplanke kaip ir programa." << endl;
        return false;
    }

    string eilute;
    if (!getline(fin, eilute))
    {
        cout << "KLAIDA: duomenu failas tuscias." << endl;
        fin.close();
        return false;
    }
    // Jei failas išsaugotas su UTF-8 BOM žyme, ją pašaliname
    if (eilute.length() >= 3 && eilute.substr(0, 3) == "\xEF\xBB\xBF")
    {
        eilute = eilute.substr(3);
    }

    int irasuSk = 0;
    if (!arSveikasisSkaicius(apkarpyti(eilute), irasuSk))
    {
        eilutesKlaida(1, "pirmoje eiluteje turi buti irasu skaicius (sveikasis skaicius).");
        fin.close();
        return false;
    }
    if (irasuSk < MIN_PREKIU || irasuSk > MAX_PREKIU)
    {
        cout << "KLAIDA: irasu skaicius " << irasuSk << " netinkamas. Leidziama nuo "
             << MIN_PREKIU << " iki " << MAX_PREKIU << " irasu." << endl;
        fin.close();
        return false;
    }

    string laukai[4];
    int nuskaityta = 0;
    int eilutesNr = 1;
    while (nuskaityta < irasuSk && getline(fin, eilute))
    {
        eilutesNr++;
        eilute = apkarpyti(eilute);
        if (eilute == "")
        {
            continue; // tuščias eilutes praleidžiame
        }
        if (!isskaidytiEilute(eilute, laukai))
        {
            eilutesKlaida(eilutesNr, "turi buti 4 laukai, atskirti kabliatasciais "
                                     "(kategorija;pavadinimas;kaina;kiekis).");
            fin.close();
            return false;
        }
        if (!arTinkamaKategorija(laukai[0]))
        {
            eilutesKlaida(eilutesNr, "netinkama kategorija \"" + laukai[0]
                                     + "\". Leidziamos: Dviraciai, Dalys, Aksesuarai.");
            fin.close();
            return false;
        }
        if (laukai[1] == "")
        {
            eilutesKlaida(eilutesNr, "nenurodytas prekes pavadinimas.");
            fin.close();
            return false;
        }
        double kaina = 0;
        if (!arRealusisSkaicius(laukai[2], kaina) || kaina <= 0)
        {
            eilutesKlaida(eilutesNr, "vieneto kaina \"" + laukai[2]
                                     + "\" turi buti teigiamas skaicius (pvz. 20.00).");
            fin.close();
            return false;
        }
        int kiekis = 0;
        if (!arSveikasisSkaicius(laukai[3], kiekis) || kiekis <= 0)
        {
            eilutesKlaida(eilutesNr, "kiekis \"" + laukai[3]
                                     + "\" turi buti teigiamas sveikasis skaicius.");
            fin.close();
            return false;
        }

        // Tos pačios prekės duomenys visuose masyvuose saugomi tuo pačiu indeksu
        kategorijos[nuskaityta] = laukai[0];
        pavadinimai[nuskaityta] = laukai[1];
        kainos[nuskaityta] = kaina;
        kiekiai[nuskaityta] = kiekis;
        nuskaityta++;
    }

    if (nuskaityta < irasuSk)
    {
        cout << "KLAIDA: faile nurodyta " << irasuSk << " irasu, bet rasta tik "
             << nuskaityta << "." << endl;
        fin.close();
        return false;
    }

    // Patikriname, ar po nurodytų įrašų nėra papildomų duomenų
    while (getline(fin, eilute))
    {
        if (apkarpyti(eilute) != "")
        {
            cout << "KLAIDA: faile yra daugiau irasu nei nurodyta pirmoje eiluteje ("
                 << irasuSk << ")." << endl;
            fin.close();
            return false;
        }
    }

    fin.close();
    n = irasuSk;
    return true;
}

// ---------------------------------------------------------------------
// Skaičiavimo funkcijos
// ---------------------------------------------------------------------

// Suapvalina pinigų sumą iki centų
double apvalintiCentais(double suma)
{
    return round(suma * 100.0) / 100.0;
}

// Prekės eilutės suma prieš nuolaidą: vieneto kaina × kiekis
double eilutesSuma(double kaina, int kiekis)
{
    return apvalintiCentais(kaina * kiekis);
}

// Nuolaida eilutei: 10 %, jei užsakyta bent 10 vienetų, kitu atveju 0
double nuolaidosSuma(double suma, int kiekis)
{
    if (kiekis >= NUOLAIDOS_KIEKIS)
    {
        return apvalintiCentais(suma * NUOLAIDOS_DALIS);
    }
    return 0;
}

// Prekės eilutės mokėtina suma po nuolaidos
double moketinaEilutesSuma(double kaina, int kiekis)
{
    double suma = eilutesSuma(kaina, kiekis);
    return suma - nuolaidosSuma(suma, kiekis);
}

// Viso užsakymo suma prieš nuolaidas
double uzsakymoSumaPriesNuolaidas(const double kainos[], const int kiekiai[], int n)
{
    double suma = 0;
    for (int i = 0; i < n; i++)
    {
        suma = suma + eilutesSuma(kainos[i], kiekiai[i]);
    }
    return suma;
}

// Bendra visų nuolaidų suma
double uzsakymoNuolaidos(const double kainos[], const int kiekiai[], int n)
{
    double suma = 0;
    for (int i = 0; i < n; i++)
    {
        suma = suma + nuolaidosSuma(eilutesSuma(kainos[i], kiekiai[i]), kiekiai[i]);
    }
    return suma;
}

// Galutinė mokėtina viso užsakymo suma po nuolaidų
double uzsakymoGalutineSuma(const double kainos[], const int kiekiai[], int n)
{
    double suma = 0;
    for (int i = 0; i < n; i++)
    {
        suma = suma + moketinaEilutesSuma(kainos[i], kiekiai[i]);
    }
    return suma;
}

// Pasirinktos kategorijos mokėtina suma po nuolaidų (jei prekių nėra – 0)
double kategorijosSuma(const string kategorijos[], const double kainos[], const int kiekiai[],
                       int n, string kategorija)
{
    double suma = 0;
    for (int i = 0; i < n; i++)
    {
        if (kategorijos[i] == kategorija)
        {
            suma = suma + moketinaEilutesSuma(kainos[i], kiekiai[i]);
        }
    }
    return suma;
}

// Kiek prekių įrašų priklauso pasirinktai kategorijai
int kategorijosPrekiuSkaicius(const string kategorijos[], int n, string kategorija)
{
    int kiekis = 0;
    for (int i = 0; i < n; i++)
    {
        if (kategorijos[i] == kategorija)
        {
            kiekis++;
        }
    }
    return kiekis;
}

// ---------------------------------------------------------------------
// Išvedimo funkcijos (out gali būti ekranas cout arba failas)
// ---------------------------------------------------------------------

void spausdintiLinija(ostream &out, char simbolis)
{
    for (int i = 0; i < LENTELES_PLOTIS; i++)
    {
        out << simbolis;
    }
    out << endl;
}

void spausdintiLentelesAntraste(ostream &out)
{
    spausdintiLinija(out, '-');
    out << left << setw(PL_NR) << "Nr."
        << setw(PL_KATEGORIJA) << "Kategorija"
        << setw(PL_PAVADINIMAS) << "Pavadinimas"
        << right << setw(PL_KAINA) << "Vnt. kaina"
        << setw(PL_KIEKIS) << "Kiekis"
        << setw(PL_SUMA) << "Suma"
        << setw(PL_NUOLAIDA) << "Nuolaida"
        << setw(PL_MOKETI) << "Moketi" << endl;
    spausdintiLinija(out, '-');
}

void spausdintiPrekesEilute(ostream &out, int nr, string kategorija, string pavadinimas,
                            double kaina, int kiekis)
{
    double suma = eilutesSuma(kaina, kiekis);
    double nuolaida = nuolaidosSuma(suma, kiekis);
    // Per ilgą pavadinimą sutrumpiname, kad nesugriūtų lentelė
    if ((int)pavadinimas.length() > PL_PAVADINIMAS - 1)
    {
        pavadinimas = pavadinimas.substr(0, PL_PAVADINIMAS - 4) + "...";
    }
    out << fixed << setprecision(2);
    out << left << setw(PL_NR) << nr
        << setw(PL_KATEGORIJA) << kategorija
        << setw(PL_PAVADINIMAS) << pavadinimas
        << right << setw(PL_KAINA) << kaina
        << setw(PL_KIEKIS) << kiekis
        << setw(PL_SUMA) << suma
        << setw(PL_NUOLAIDA) << nuolaida
        << setw(PL_MOKETI) << suma - nuolaida << endl;
}

// Išveda prekių lentelę. Jei kategorija tuščia (""), išvedamos visos prekės.
void spausdintiPrekiuLentele(ostream &out, const string kategorijos[], const string pavadinimai[],
                             const double kainos[], const int kiekiai[], int n, string kategorija)
{
    out << "Visos sumos pateiktos eurais (EUR)." << endl;
    spausdintiLentelesAntraste(out);
    int nr = 0;
    for (int i = 0; i < n; i++)
    {
        if (kategorija == "" || kategorijos[i] == kategorija)
        {
            nr++;
            spausdintiPrekesEilute(out, nr, kategorijos[i], pavadinimai[i], kainos[i], kiekiai[i]);
        }
    }
    spausdintiLinija(out, '-');
}

void spausdintiUzsakymoSumas(ostream &out, const double kainos[], const int kiekiai[], int n)
{
    out << fixed << setprecision(2);
    out << "VISO UZSAKYMO SUMOS" << endl;
    out << left << setw(36) << "Suma pries nuolaidas:"
        << right << setw(12) << uzsakymoSumaPriesNuolaidas(kainos, kiekiai, n) << " EUR" << endl;
    out << left << setw(36) << "Bendra nuolaidu suma:"
        << right << setw(12) << uzsakymoNuolaidos(kainos, kiekiai, n) << " EUR" << endl;
    out << left << setw(36) << "Galutine moketina suma:"
        << right << setw(12) << uzsakymoGalutineSuma(kainos, kiekiai, n) << " EUR" << endl;
}

void spausdintiKategorijuSuvestine(ostream &out, const string kategorijos[], const double kainos[],
                                   const int kiekiai[], int n)
{
    out << fixed << setprecision(2);
    out << "SUMOS PAGAL KATEGORIJAS (po nuolaidu)" << endl;
    for (int k = 0; k < KATEGORIJU_SK; k++)
    {
        // Ta pati funkcija kviečiama kiekvienai kategorijai, kategoriją perduodant parametru
        out << left << setw(14) << KATEGORIJOS[k]
            << "(" << kategorijosPrekiuSkaicius(kategorijos, n, KATEGORIJOS[k]) << " iras.)";
        out << right << setw(18)
            << kategorijosSuma(kategorijos, kainos, kiekiai, n, KATEGORIJOS[k]) << " EUR" << endl;
    }
}

// Visa užsakymo ataskaita: prekių lentelė, užsakymo sumos ir kategorijų suvestinė
void spausdintiAtaskaita(ostream &out, const string kategorijos[], const string pavadinimai[],
                         const double kainos[], const int kiekiai[], int n)
{
    spausdintiLinija(out, '=');
    out << "DVIRACIU PARDUOTUVES UZSAKYMO ATASKAITA" << endl;
    out << "Prekiu irasu: " << n << endl;
    spausdintiLinija(out, '=');
    spausdintiPrekiuLentele(out, kategorijos, pavadinimai, kainos, kiekiai, n, "");
    out << endl;
    spausdintiUzsakymoSumas(out, kainos, kiekiai, n);
    out << endl;
    spausdintiKategorijuSuvestine(out, kategorijos, kainos, kiekiai, n);
    spausdintiLinija(out, '=');
}

// Įrašo ataskaitą į failą (ankstesnis turinys perrašomas). Grąžina true tik sėkmės atveju.
bool issaugotiAtaskaita(string failoVardas, const string kategorijos[], const string pavadinimai[],
                        const double kainos[], const int kiekiai[], int n)
{
    ofstream fout;
    fout.open(failoVardas.c_str()); // numatytasis režimas – failas perrašomas
    if (fout.fail())
    {
        cout << "KLAIDA: nepavyko atverti rezultatu failo \"" << failoVardas << "\" irasymui." << endl;
        return false;
    }
    spausdintiAtaskaita(fout, kategorijos, pavadinimai, kainos, kiekiai, n);
    if (fout.fail())
    {
        cout << "KLAIDA: nepavyko irasyti duomenu i faila \"" << failoVardas << "\"." << endl;
        fout.close();
        return false;
    }
    fout.close();
    if (fout.fail())
    {
        cout << "KLAIDA: nepavyko uzdaryti failo \"" << failoVardas << "\"." << endl;
        return false;
    }
    return true;
}

// ---------------------------------------------------------------------
// Meniu ir vartotojo įvestis
// ---------------------------------------------------------------------

void rodytiMeniu()
{
    cout << endl;
    cout << "========== MENIU ==========" << endl;
    cout << "1. Perziureti visas uzsakymo prekes" << endl;
    cout << "2. Perziureti pasirinktos kategorijos prekes" << endl;
    cout << "3. Perziureti viso uzsakymo sumas" << endl;
    cout << "4. Perziureti sumas pagal kategorijas" << endl;
    cout << "5. Parodyti visa uzsakymo ataskaita" << endl;
    cout << "6. Issaugoti visa ataskaita i faila" << endl;
    cout << "7. Iseiti" << endl;
}

// Nuskaito skaičių iš intervalo [min; max]. Kol įvestis netinkama, prašo įvesti iš naujo.
// Jei įvestis baigėsi (pvz. Ctrl+Z / Ctrl+D), grąžina -1.
int skaitytiPasirinkima(string uzklausa, int min, int max)
{
    string ivestis;
    int pasirinkimas = 0;
    while (true)
    {
        cout << uzklausa;
        if (!getline(cin, ivestis))
        {
            cout << endl;
            return -1;
        }
        if (arSveikasisSkaicius(apkarpyti(ivestis), pasirinkimas)
                && pasirinkimas >= min && pasirinkimas <= max)
        {
            return pasirinkimas;
        }
        cout << "Netinkamas pasirinkimas. Iveskite skaiciu nuo " << min << " iki " << max << "." << endl;
    }
}

// Leidžia pasirinkti kategoriją ir parodo jos prekes bei mokėtiną sumą
void rodytiKategorijosPrekes(const string kategorijos[], const string pavadinimai[],
                             const double kainos[], const int kiekiai[], int n)
{
    cout << endl << "Pasirinkite kategorija:" << endl;
    for (int k = 0; k < KATEGORIJU_SK; k++)
    {
        cout << k + 1 << ". " << KATEGORIJOS[k] << endl;
    }
    int nr = skaitytiPasirinkima("Kategorijos numeris: ", 1, KATEGORIJU_SK);
    if (nr == -1)
    {
        return;
    }
    string kategorija = KATEGORIJOS[nr - 1];

    cout << endl << "KATEGORIJA: " << kategorija << endl;
    if (kategorijosPrekiuSkaicius(kategorijos, n, kategorija) == 0)
    {
        cout << "Sioje kategorijoje uzsakytu prekiu nera. Moketina suma: 0.00 EUR" << endl;
        return;
    }
    spausdintiPrekiuLentele(cout, kategorijos, pavadinimai, kainos, kiekiai, n, kategorija);
    cout << fixed << setprecision(2);
    cout << "Kategorijos \"" << kategorija << "\" moketina suma: "
         << kategorijosSuma(kategorijos, kainos, kiekiai, n, kategorija) << " EUR" << endl;
}

// ---------------------------------------------------------------------

int main()
{
    // Vienmačiai masyvai – tas pats indeksas reiškia tą pačią prekę
    string kategorijos[MAX_PREKIU];
    string pavadinimai[MAX_PREKIU];
    double kainos[MAX_PREKIU];
    int kiekiai[MAX_PREKIU];
    int n = 0;

    cout << fixed << setprecision(2);

    if (!nuskaitytiDuomenis(DUOMENU_FAILAS, kategorijos, pavadinimai, kainos, kiekiai, n))
    {
        cout << "Duomenys netinkami arba nepilni, todel ataskaita neformuojama. Programa baigiama." << endl;
        return 1;
    }
    cout << "Duomenys sekmingai nuskaityti is \"" << DUOMENU_FAILAS << "\": "
         << n << " prekiu irasu." << endl;

    int pasirinkimas;
    do
    {
        rodytiMeniu();
        pasirinkimas = skaitytiPasirinkima("Jusu pasirinkimas: ", 1, ISEITI);
        cout << endl;
        switch (pasirinkimas)
        {
        case 1:
            cout << "VISOS UZSAKYMO PREKES" << endl;
            spausdintiPrekiuLentele(cout, kategorijos, pavadinimai, kainos, kiekiai, n, "");
            break;
        case 2:
            rodytiKategorijosPrekes(kategorijos, pavadinimai, kainos, kiekiai, n);
            break;
        case 3:
            spausdintiUzsakymoSumas(cout, kainos, kiekiai, n);
            break;
        case 4:
            spausdintiKategorijuSuvestine(cout, kategorijos, kainos, kiekiai, n);
            break;
        case 5:
            spausdintiAtaskaita(cout, kategorijos, pavadinimai, kainos, kiekiai, n);
            break;
        case 6:
            if (issaugotiAtaskaita(ATASKAITOS_FAILAS, kategorijos, pavadinimai, kainos, kiekiai, n))
            {
                cout << "Ataskaita sekmingai issaugota faile \"" << ATASKAITOS_FAILAS << "\"." << endl;
            }
            break;
        case ISEITI:
            cout << "Programa baigta." << endl;
            break;
        default: // įvestis baigėsi (-1)
            cout << "Ivestis baigesi. Programa baigta." << endl;
            pasirinkimas = ISEITI;
            break;
        }
    }
    while (pasirinkimas != ISEITI);

    return 0;
}
