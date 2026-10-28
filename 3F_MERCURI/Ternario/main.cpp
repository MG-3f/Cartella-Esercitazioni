#include <iostream>

using namespace std;

int main() {
    int x;
    
    cout << "Inserisci un numero";
    cin >> x;

    char a;
    a = (x % 2 == 0) ? 'p' : 'd';

    cout << a << endl;

    return 0;
}

