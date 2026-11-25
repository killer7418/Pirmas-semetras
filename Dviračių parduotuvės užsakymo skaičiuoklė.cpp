#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;

const int MAX = 100;
const string KATEGORIJOS[3] = {"Dviraciai", "Dalys", "Aksesuarai"};

string valyti(string s)
{
    while (s.length() > 0 && (s[s.length() - 1] == ' ' || s[s.length() - 1] == '\r'))
        s.erase(s.length() - 1);
    while (s.length() > 0 && s[0] == ' ')
        s.erase(0, 1);
    return s;
}

bool arSkaicius(string s, bool galiButiTaskas)
{
    int taskai = 0;
    if (s == "" || s == "." || s.length() > 9)
        return false;
    for (int i = 0; i < (int)s.length(); i++)
    {
        if (s[i] == '.' && galiButiTaskas)
            taskai++;
        else if (s[i] < '0' || s[i] > '9')
            return false;
    }
    return taskai <= 1;
}

bool arKategorija(string k)
{
    for (int i = 0; i < 3; i++)
        if (k == KATEGORIJOS[i])
            return true;
    return false;
}

bool nuskaityti(string kat[], string pav[], double kaina[], int kiekis[], int &n)
{
    ifstream fin("uzsakymas.txt");
    if (fin.fail())
    {
        cout << "KLAIDA: nepavyko atverti failo uzsakymas.txt" << endl;
        return false;
    }
    string eil;
    getline(fin, eil);
    eil = valyti(eil);
    if (!arSkaicius(eil, false) || stoi(eil) < 1 || stoi(eil) > MAX)
    {
        cout << "KLAIDA: irasu skaicius turi buti nuo 1 iki " << MAX << endl;
        return false;
    }
    n = stoi(eil);
    for (int i = 0; i < n; i++)
    {
        if (!getline(fin, eil))
        {
            cout << "KLAIDA: nurodyta " << n << " irasu, bet nuskaityta tik " << i << endl;
            return false;
        }
        string laukai[4];
        int dalis = 0;
        for (int j = 0; j < (int)eil.length(); j++)
        {
            if (eil[j] == ';')
                dalis++;
            else if (dalis < 4)
                laukai[dalis] += eil[j];
        }
        for (int j = 0; j < 4; j++)
            laukai[j] = valyti(laukai[j]);
        if (dalis != 3 || !arKategorija(laukai[0]) || laukai[1] == "" ||
            !arSkaicius(laukai[2], true) || stod(laukai[2]) <= 0 ||
            !arSkaicius(laukai[3], false) || stoi(laukai[3]) <= 0)
        {
            cout << "KLAIDA: netinkami duomenys " << i + 2 << " eiluteje: " << valyti(eil) << endl;
            cout << "Kategorija - Dviraciai, Dalys arba Aksesuarai; kaina > 0; "
                 << "kiekis - teigiamas sveikasis skaicius." << endl;
            return false;
        }
        kat[i] = laukai[0];
        pav[i] = laukai[1];
        kaina[i] = stod(laukai[2]);
        kiekis[i] = stoi(laukai[3]);
    }
    fin.close();
    return true;
}

int main()
{
    string kat[MAX], pav[MAX];
    double kaina[MAX];
    int kiekis[MAX];
    int n = 0;

    if (!nuskaityti(kat, pav, kaina, kiekis, n))
    {
        cout << "Programa baigiama." << endl;
        return 1;
    }
    cout << "Nuskaityta prekiu irasu: " << n << endl;
    cout << fixed << setprecision(2);
    for (int i = 0; i < n; i++)
        cout << left << setw(12) << kat[i] << setw(47) << pav[i]
             << right << setw(10) << kaina[i] << setw(6) << kiekis[i] << endl;

    return 0;
}
