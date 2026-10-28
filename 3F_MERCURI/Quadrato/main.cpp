#include <iostream>

int main() {
	
	double lato, perimetro, area;
	
	cout << "Inserisci la lunghezza del lato del quadrato: \n";
	cin >> lato;
	
	perimetro = lato * 4;
	area = lato * lato;
	
	cout << "Perimetro: " << perimetro << endl;
	cout << "Area: " << area << endl;
	
	return 0;
	
}
