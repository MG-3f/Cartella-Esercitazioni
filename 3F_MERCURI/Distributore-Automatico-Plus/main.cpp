#include <iostream>

using namespace std;

int main() {
    int scelta;
    int prezzo = 0;
    int moneta;
    int totaleMoneteInserite;
    int resto;

    while (true) { // Ciclo infinito gestito da return e break
        cout << "\n=== DISTRIBUTORE AUTOMATICO ===" << endl;
        cout << "1: Acqua (1 EURO)" << endl;
        cout << "2: Snack (2 EURO)" << endl;
        cout << "3: Red Bull (3 EURO)" << endl;
        cout << "4: Esci" << endl;
        cout << "==============================" << endl;
        cout << "Seleziona un prodotto: ";
        cin >> scelta;

        switch (scelta) {
            case 1:
                prezzo = 1; 
                break;
            case 2:
                prezzo = 2;
                break;
            case 3:
                prezzo = 3;
                break;
            case 4:
                cout << "Arrivederci!" << endl;
                return 1;
            default:
                cout << "Scelta non valida. Riprova." << endl;
                continue; // salta il resto del ciclo e torna al menu
        }

        // se si va qui la scelta valida è sicuramente 1 2 o 3
        totaleMoneteInserite = 0;
        do {
            cout << "Prezzo: " << prezzo << " EURO. Inseriti: " << totaleMoneteInserite << " EURO. Inserisci moneta: ";
            cin >> moneta;
            totaleMoneteInserite += moneta;
        } while (totaleMoneteInserite < prezzo);

        // calcolo ed erogazione del resto
        resto = totaleMoneteInserite - prezzo;
        while (resto > 0) {
            cout << "Erogazione 1 Euro di resto" << endl;
            resto -= 1;
        }
        
        cout << "Prodotto erogato con successo!" << endl;
    }

    return 0;
}

