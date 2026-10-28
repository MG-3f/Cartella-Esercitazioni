#include <iostream>
using namespace std;

int main() {
    string nome;
    int eta;
    string via;
    int civico; 
    string citta;

    cout << "Ciao, come ti chiami?" << endl;
    cin >> nome;

    cout << "Quanti anni hai?" << endl;
    cin >> eta;

    cout << "Qual e il tuo indirizzo? Inserisci nel seguente ordine: via, civico e citta" << endl;
    cin >> via;
    cin >> civico;
    cin >> citta;

    cout << "Sono " << nome << ", ho " << eta << " anni e vivo in via " << via << ", " << civico << ", " << citta << endl;

    return 0;
}

