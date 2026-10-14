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

    cout << fixed << setprecision(2);

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
        double bendras = 0, pirkti = 0, parduoti = 0;

        switch (valiuta) {
            case 1:
                kodas = "GBP"; bendras = GBP_Bendras; pirkti = GBP_Pirkti; parduoti = GBP_Parduoti;
                break;
            case 2:
                kodas = "USD"; bendras = USD_Bendras; pirkti = USD_Pirkti; parduoti = USD_Parduoti;
                break;
            case 3:
                kodas = "INR"; bendras = INR_Bendras; pirkti = INR_Pirkti; parduoti = INR_Parduoti;
                break;
        }

        if (veiksmas == 1) {
            cout << "\n   " << kodas << " kursai euro atzvilgiu    \n";
            cout << setprecision(4);
            cout << "Bendras kursas:   1 EUR = " << bendras  << " " << kodas << "\n";
            cout << "Pirkimo kursas:   1 EUR = " << pirkti   << " " << kodas << "\n";
            cout << "Pardavimo kursas: 1 EUR = " << parduoti << " " << kodas << "\n";
            cout << setprecision(2);
            cout << "Parduodant 1 " << kodas << " gaunas " << 1 / parduoti << " EUR\n";
        }
        else if (veiksmas == 2) {
            double suma;
            cout << "Iveskiti suma ,  ";
            cin >> suma;

            double gaunama = suma * pirkti;
            cout << "\n    Pirkimas    \n";
            cout << "Moketi:   " << suma << " EUR\n";
            cout << "Valiuta:  " << kodas << "\n";
            cout << "Gaunti:  " << gaunama << " " << kodas << "\n";
        }
        else if (veiksmas == 3) {
            double suma;
            cout << "Ivesti" << kodas << " suma, kuria parduoti: ";
            cin >> suma;

            double gaunama = suma / parduoti;
            cout << "\n    Pardavimas    \n";
            cout << "Parduoduoti: " << suma << " " << kodas << "\n";
            cout << "Valiuta:    " << kodas << "\n";
            cout << "Gaunti:    " << gaunama << " EUR\n";
        }

    } while (true);

    return 0;
}
