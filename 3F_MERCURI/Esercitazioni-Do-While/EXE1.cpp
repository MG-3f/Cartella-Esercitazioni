#include <iostream>

using namespace std;

int main() {
    int numero;

    do {
        cout << "Inserisci un numero intero positivo: ";
        cin >> numero;
    } while (numero <= 0);

    cout << "Numero positivo accettato!" << endl;

    return 0;
}

