#include <iostream>
using namespace std;

int main() {
    int x, y;
    
    cout << "Inserisci 2 numeri: ";
    cin >> x >> y;
    
    if (x > y) {
        cout << "il valore massimo tra " << x << " e " << y << " è " << x;
    } else {
        cout << "il valore massimo tra " << x << " e " << y << " è " << y;
    }
    
    return 0;
}

