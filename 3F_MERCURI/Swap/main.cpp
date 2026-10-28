#include <iostream>
using namespace std;
int main() {
    int x, y;
    
    cout << "Inserisci x e y" << endl;
    cin >> x >> y;

    cout << "Prima:  x = " << x << ", y = " << y << endl;

    int temp = x;
    x = y;
    y = temp;

    cout << "Dopo  x = " << x << ", y = " << y << endl;

    return 0;
}

