#include <iostream>

using namespace std;

int main() {
    int temperatura;
    cout << "Inserisci temperatura:";
    cin >> temperatura;
    cout << "\nTemperatura inziale" << temperatura << endl;

    while (temperatura < 20) {
        temperatura += 1;
        cout << temperatura << endl;
    }

    cout << "La temperatura ha raggiunto o superato i 20 gradi" << endl;
    return 0;
}

