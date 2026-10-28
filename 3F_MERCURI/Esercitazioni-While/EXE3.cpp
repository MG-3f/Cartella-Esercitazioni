#include <iostream>

using namespace std;

int main() {
    int N;
    cout << "Inserisci il numero di caramelle";
    cin >> N;
    cout << N << endl;

    while (N > 0) {
        cout << "Mangiata una caramella!" << endl;
        N--;
    }

    cout << "Caramelli finite" << endl;
    return 0;
}

