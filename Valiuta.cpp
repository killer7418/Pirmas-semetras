#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

const double GBP_Bendras  = 0.8729;
const double GBP_Pirkti   = 0.8600;
const double GBP_Parduoti = 0.9220;

const double USD_Bendras  = 1.1793;
const double USD_Pirkti   = 1.1460;
const double USD_Parduoti = 1.2340;

const double INR_Bendras  = 104.6918;
const double INR_Pirkti   = 101.3862;
const double INR_Parduoti = 107.8546;

int main() {
    int veiksmas;

    do {
        cout << "\n Keitykla \n";
        cout << "1  Valiutos kursas  \n";
        cout << "2  Valiutos prikimas \n";
        cout << "3  Valiutos pardavimas  \n";
        cout << "4  Iseiti\n";
        cout << "Pasirinkite veiksma: ";
        cin >> veiksmas;

        if (veiksmas == 4) {
            cout << " Veikia \n";
            break;
        }

        int valiuta;
        cout << "\n  Pasirinkiti valiuta:\n";
        cout << "1  GBP \n";
        cout << "2  USD \n";
        cout << "3  INR \n";
        cout << " parinkti : ";
        cin >> valiuta;

        string kodas;
        switch (valiuta) {
            case 1: kodas = "GBP"; ;
            case 2: kodas = "USD"; ;
            case 3: kodas = "INR"; ;
        }

        cout << "Pasirinkta valiuta: " << kodas << "\n";
        cout << "Veikia \n";

    } while (true);

    return 0;
}
