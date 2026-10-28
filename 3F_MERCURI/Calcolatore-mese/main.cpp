#include <iostream>

using namespace std;

int main() {
    int mese;
    
    cout << "Inserisci il numero del mese: ";
    cin >> mese;

    switch (mese) {
        case 1:
            cout << "Gennaio" << endl;
            break;
        case 2:
            cout << "Febbraio" << endl;
            break;
        case 3:
            cout << "Marzo" << endl;
            break;
        case 4:
            cout << "Aprile" << endl;
            break;
        case 5:
            cout << "Maggio" << endl;
            break;
        case 6:
            cout << "Giugno" << endl;
            break;
        case 7:
            cout << "Luglio" << endl;
            break;
        case 8:
            cout << "Agosto" << endl;
            break;
        case 9:
            cout << "Settembre" << endl;
            break;
        case 10:
            cout << "Ottobre" << endl;
            break;
        case 11:
            cout << "Novembre" << endl;
            break;
        case 12:
            cout << "Dicembre" << endl;
            break;
        default:
            cout << "non hai inserito un valore valido" << endl;
            break;
    }

    return 0;
}

