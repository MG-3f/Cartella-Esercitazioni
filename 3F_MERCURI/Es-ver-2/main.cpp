#include <iostream>
#include <string>

using namespace std;

int main() {
    int scelta;
    
    cout << "=== MENU VALUTAZIONE STUDENTI ===" << endl;
    cout << "1: Verifica Promozione" << endl;
    cout << "2: Converti Voto in Giudizio" << endl;
    cout << "3: Confronta Due Voti" << endl;
    cout << "4: Uscita" << endl;
    cout << "Seleziona un'opzione: ";
    cin >> scelta;

    switch (scelta) {
        case 1:
            float media;
            cout << "Inserisci la media dei voti: ";
            cin >> media;
            cout << (media >= 6.0f ? "Promosso" : "Non promosso") << endl;
            break;
            
        case 2:
            int voto;
            cout << "Inserisci un voto intero (1-10): ";
            cin >> voto;
            
            switch (voto) {
                case 10:
                    cout << "Eccellente" << endl;
                    break;
                case 9:
                case 8:
                    cout << "Ottimo" << endl;
                    break;
                case 7:
                case 6:
                    cout << "Sufficiente" << endl;
                    break;
                case 5:
                case 4:
                case 3:
                case 2:
                case 1:
                    cout << "Insufficiente" << endl;
                    break;
                default:
                    cout << "Voto non valido" << endl;
                    break;
            }
            break;
            
        case 3:
            float voto1, voto2;
            cout << "Inserisci il primo voto: ";
            cin >> voto1;
            cout << "Inserisci il secondo voto: ";
            cin >> voto2;
            cout << (voto1 == voto2 ? "I voti sono uguali" : (voto1 > voto2 ? "Il voto maggiore e': " + to_string(voto1) : "Il voto maggiore e': " + to_string(voto2))) << endl;
            break;
            
        case 4:
            cout << "Sessione terminata. Arrivederci!" << endl;
            break;
            
        default:
            cout << "Opzione non valida! Riavvia il programma." << endl;
            break;
    }

    return 0;
}

