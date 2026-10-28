#include <iostream>

using namespace std;

int main() {
    int dado;

    do {
        cout << "Inserisci il valore del dado (da 1 a 6): ";
        cin >> dado;
        
        if (dado != 6) {
            cout << "Non hai fatto 6, ritenta!" << endl;
        }
    } while (dado != 6);

    cout << "Hai fatto 6! Hai vinto!" << endl;

    return 0;
}

