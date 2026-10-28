#include <iostream>
#include <cmath>

using namespace std;

int main() {
	int scelta;
	
	cout << "---CALCOLATORE FIGURE GEOMETRICHE---" << endl;
	cout << "Cosa vuoi calcolare?" << endl;
	cout << "1. Cerchio" << endl;
	cout << "2. Quadrato" << endl;
	cout << "3. Rettangolo" << endl;
	cout << "4. Triangolo rettangolo" << endl;
	cout << "5. Altro / esci" << endl;
	cout << "Inserisci il numero della tua scelta" << endl;
	cin >> scelta;
	
	const double PI_GRECO = 3.14;
	
	if (scelta == 1) {
        double raggio;
        cout << "\nHai scelto: Cerchio" << endl;
        cout << "Inserisci il raggio: ";
        cin >> raggio;
        double area = PI_GRECO * pow(raggio, 2);
        double circonferenza = 2 * PI_GRECO * raggio;
        cout << "Area: " << area << endl;
        cout << "Circonferenza: " << circonferenza << endl;
    }
    else if (scelta == 2) {
        double lato;
        cout << "\nHai scelto: Quadrato" << endl;
        cout << "Inserisci il lato: ";
        cin >> lato;
        double area = pow(lato, 2);
        double perimetro = lato * 4;
        cout << "Area: " << area << endl;
        cout << "Perimetro: " << perimetro << endl;
    }
    else if (scelta == 3) {
        double base, altezza;
        cout << "\nHai scelto: Rettangolo" << endl;
        cout << "Inserisci la base: ";
        cin >> base;
        cout << "Inserisci l'altezza: ";
        cin >> altezza;
        double area = base * altezza;
        double perimetro = 2 * (base + altezza);
        cout << "Area: " << area << endl;
        cout << "Perimetro: " << perimetro << endl;
    } 
    else if (scelta == 4) {
        double cateto1, cateto2;
        cout << "\nHai scelto: Triangolo Rettangolo" << endl;
        cout << "Inserisci il primo cateto: ";
        cin >> cateto1;
        cout << "Inserisci il secondo cateto: ";
        cin >> cateto2;
        double area = (cateto1 * cateto2) / 2.0;
        double ipotenusa = sqrt(pow(cateto1, 2) + pow(cateto2, 2));
        double perimetro = cateto1 + cateto2 + ipotenusa;
        cout << "Area: " << area << endl;
        cout << "Ipotenusa: " << ipotenusa << endl;
        cout << "Perimetro: " << perimetro << endl;
    } 
    else {
        cout << "Opzione non disponibile \n" << endl;
    }
    
    return 0;
}
