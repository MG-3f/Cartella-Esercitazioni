#include <iostream>

using namespace std;

int main() {
    int voto;

    do {
        cout << "Inserisci un voto scolastico (da 1 a 10): ";
        cin >> voto;
    } while (voto < 1 || voto > 10);

    cout << "Voto registrato con successo!" << endl;

    return 0;
}

