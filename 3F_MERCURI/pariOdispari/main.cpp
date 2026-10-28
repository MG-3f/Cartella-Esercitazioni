#include <iostream>

using namespace std;

int main() {
    int numero;
    
    cout << "Inserisci un numero: ";
    cin >> numero;
    
    if (numero % 2 == 0) {
        cout << "il numero " << numero << " è pari";
    } else {
        cout << "il numero " << numero << " non è pari";
    }
    
    return 0;
}

