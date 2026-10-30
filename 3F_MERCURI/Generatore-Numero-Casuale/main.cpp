#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

int main() {
    srand(time(0));
    int scelta;

    do {
        cout << "--- MENU ---\n";
        cout << "1. Genera numero casuale in un intervallo\n";
        cout << "2. Esci\n";
        cout << "Scelta: ";
        cin >> scelta;

        if (scelta == 1) {
            int min, max;
            cout << "Inserisci il valore minimo: ";
            cin >> min;
            cout << "Inserisci il valore massimo: ";
            cin >> max;

            if (min > max) {
                int temp = min;
                min = max;
                max = temp;
            }

            int numeroCasuale = min + rand() % (max - min + 1);
            cout << "Numero generato: " << numeroCasuale << "\n\n";
        } else if (scelta != 2) {
            cout << "Scelta non valida.\n\n";
        }

    } while (scelta != 2);

    return 0;
}

