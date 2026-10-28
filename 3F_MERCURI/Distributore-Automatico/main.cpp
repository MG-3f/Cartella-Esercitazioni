#include <iostream>
using namespace std;

int main() {
    int scelta;
	
	do {
		cout << "=== DISTRIBUTORE AUTOMATICO ===\n";
        cout << "1: Acqua (1 €)\n";
        cout << "2: Snack (2 €)\n";
        cout << "3: Red Bull (3 €)\n";
        cout << "4: Esci\n";
        cout << "==============================\n";
        cout << "Seleziona un prodotto: ";
        cin >> scelta;
        
        
	}while(scelta!= 4)
}
