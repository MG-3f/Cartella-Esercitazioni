#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <cctype>
#include <iomanip>

// Prima includiamo le librerie di sistema Windows per evitare conflitti con 'byte'
#ifdef _WIN32
#include <windows.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#endif

// Spostato qui dopo le inclusioni di sistema
using namespace std;

// ==========================================
// CONFIGURAZIONE COLORI ANSI
// ==========================================
void abilitaAnsi() {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
}

void pulisciSchermo() {
    cout << "\033[2J\033[1;1H";
}

const string COLORE_RESET = "\033[0m";
const string COLORE_BLU   = "\033[34m"; // Acqua
const string COLORE_ROSSO = "\033[31m"; // Colpo a segno
const string COLORE_BIANCO= "\033[37m"; // Mancato
const string COLORE_VERDE = "\033[32m"; // Nave
const string COLORE_GIALLO= "\033[33m"; // Evidenziato / Info
const string COLORE_CIANO = "\033[36m"; // Intestazioni

// ==========================================
// STRUTTURE E ENUMS
// ==========================================
enum class TipoNave { PORTAEREI, CORAZZATA, INCROCIATORE, SOTTOMARINO, CACCIATORPEDINIERE };
enum class StatoCella { VUOTA, NAVE, COLPITO, MANCATO };

struct Coordinata {
    int x;
    int y;
    bool operator==(const Coordinata& altra) const { return x == altra.x && y == altra.y; }
};

// ==========================================
// CLASSE NAVE
// ==========================================
class Nave {
private:
    string nome_;
    int dimensione_;
    TipoNave tipo_;
    bool orizzontale_;
    Coordinata posIniziale_;
    vector<Coordinata> coordinate_;
    int colpiRicevuti_;

public:
    Nave(string nome, int dimensione, TipoNave tipo)
        : nome_(nome), dimensione_(dimensione), tipo_(tipo), orizzontale_(true), posIniziale_{-1, -1}, colpiRicevuti_(0) {}

    void posiziona(Coordinata inizio, bool orizzontale) {
        posIniziale_ = inizio;
        orizzontale_ = orizzontale;
        coordinate_.clear();
        for (int i = 0; i < dimensione_; ++i) {
            if (orizzontale_) coordinate_.push_back({inizio.x + i, inizio.y});
            else coordinate_.push_back({inizio.x, inizio.y + i});
        }
    }

    bool eAffondata() const { return colpiRicevuti_ >= dimensione_; }
    void colpisci() { if (colpiRicevuti_ < dimensione_) colpiRicevuti_++; }
    
    string getNome() const { return nome_; }
    int getDimensione() const { return dimensione_; }
    TipoNave getTipo() const { return tipo_; }
    bool eOrizzontale() const { return orizzontale_; }
    const vector<Coordinata>& getCoordinate() const { return coordinate_; }
    bool ePosizionata() const { return posIniziale_.x != -1 && posIniziale_.y != -1; }
};

// ==========================================
// CLASSE GRIGLIA
// ==========================================
class Griglia {
private:
    static const int DIMENSIONE = 10;
    StatoCella griglia_[DIMENSIONE][DIMENSIONE];
    vector<Nave> navi_;

public:
    Griglia() { azzera(); }

    void azzera() {
        for (int y = 0; y < DIMENSIONE; ++y)
            for (int x = 0; x < DIMENSIONE; ++x)
                griglia_[y][x] = StatoCella::VUOTA;
        navi_.clear();
    }

    bool puoPosizionare(const Nave& nave, Coordinata inizio, bool orizzontale) const {
        if (inizio.x < 0 || inizio.x >= DIMENSIONE || inizio.y < 0 || inizio.y >= DIMENSIONE) return false;

        int dim = nave.getDimensione();
        if (orizzontale && inizio.x + dim > DIMENSIONE) return false;
        if (!orizzontale && inizio.y + dim > DIMENSIONE) return false;

        for (int i = 0; i < dim; ++i) {
            int x = orizzontale ? inizio.x + i : inizio.x;
            int y = orizzontale ? inizio.y : inizio.y + i;
            if (griglia_[y][x] == StatoCella::NAVE) return false;
        }
        return true;
    }

    bool posizionaNave(size_t indiceNave, Coordinata inizio, bool orizzontale) {
        if (indiceNave >= navi_.size()) return false;
        
        Nave& nave = navi_[indiceNave];
        if (!puoPosizionare(nave, inizio, orizzontale)) return false;

        if (nave.ePosizionata()) {
            for (const auto& coord : nave.getCoordinate()) {
                griglia_[coord.y][coord.x] = StatoCella::VUOTA;
            }
        }

        nave.posiziona(inizio, orizzontale);
        for (const auto& coord : nave.getCoordinate()) {
            griglia_[coord.y][coord.x] = StatoCella::NAVE;
        }
        return true;
    }

    int attaccaCella(Coordinata coord, Nave*& naveColpita) {
        if (coord.x < 0 || coord.x >= DIMENSIONE || coord.y < 0 || coord.y >= DIMENSIONE) return -1;

        StatoCella& stato = griglia_[coord.y][coord.x];
        if (stato == StatoCella::COLPITO || stato == StatoCella::MANCATO) return -1;

        if (stato == StatoCella::VUOTA) {
            stato = StatoCella::MANCATO;
            return 0; 
        }

        if (stato == StatoCella::NAVE) {
            stato = StatoCella::COLPITO;
            for (auto& nave : navi_) {
                for (const auto& c : nave.getCoordinate()) {
                    if (c == coord) {
                        nave.colpisci();
                        naveColpita = &nave;
                        if (nave.eAffondata()) return 2; 
                        return 1; 
                    }
                }
            }
        }
        return -1;
    }

    bool tutteNaviAffondate() const {
        if (navi_.empty()) return false;
        for (const auto& nave : navi_) {
            if (!nave.eAffondata()) return false;
        }
        return true;
    }

    void posizionaNaviCasualmente() {
        for (int y = 0; y < DIMENSIONE; ++y)
            for (int x = 0; x < DIMENSIONE; ++x)
                griglia_[y][x] = StatoCella::VUOTA;

        for (size_t i = 0; i < navi_.size(); ++i) {
            bool posizionata = false;
            while (!posizionata) {
                Coordinata inizio = {rand() % DIMENSIONE, rand() % DIMENSIONE};
                bool oriz = (rand() % 2 == 0);
                if (puoPosizionare(navi_[i], inizio, oriz)) {
                    navi_[i].posiziona(inizio, oriz);
                    for (const auto& coord : navi_[i].getCoordinate()) {
                        griglia_[coord.y][coord.x] = StatoCella::NAVE;
                    }
                    posizionata = true;
                }
            }
        }
    }

    StatoCella getStatoCella(int x, int y) const { return griglia_[y][x]; }
    vector<Nave>& getNavi() { return navi_; }
    const vector<Nave>& getNavi() const { return navi_; }
    void aggiungiNave(Nave nave) { navi_.push_back(nave); }
};

// ==========================================
// CLASSE GIOCATORE
// ==========================================
class Giocatore {
private:
    string nome_;
    Griglia griglia_;
    int colpiSparati_;
    int colpiASegno_;
    int mancati_;

public:
    Giocatore(string nome = "Giocatore")
        : nome_(nome), colpiSparati_(0), colpiASegno_(0), mancati_(0) {}

    string getNome() const { return nome_; }
    Griglia& getGriglia() { return griglia_; }
    const Griglia& getGriglia() const { return griglia_; }

    int getColpiSparati() const { return colpiSparati_; }
    int getColpiASegno() const { return colpiASegno_; }
    int getMancati() const { return mancati_; }

    void registraColpo(bool successo) {
        colpiSparati_++;
        if (successo) colpiASegno_++;
        else mancati_++;
    }

    void azzeraStatistiche() {
        colpiSparati_ = 0;
        colpiASegno_ = 0;
        mancati_ = 0;
        griglia_.azzera();
    }
};

// ==========================================
// FUNZIONI DI INTERFACCIA COMPLEMENTARI
// ==========================================
vector<Nave> creaFlottaStandard() {
    return {
        Nave("Portaerei", 5, TipoNave::PORTAEREI),
        Nave("Corazzata", 4, TipoNave::CORAZZATA),
        Nave("Incrociatore", 3, TipoNave::INCROCIATORE),
        Nave("Sottomarino", 3, TipoNave::SOTTOMARINO),
        Nave("Cacciatorpediniere", 2, TipoNave::CACCIATORPEDINIERE)
    };
}

void mostraGriglia(const Griglia& griglia, bool nascondiNavi) {
    cout << "   ";
    for (int x = 0; x < 10; ++x) cout << " " << x << " ";
    cout << "\n";

    for (int y = 0; y < 10; ++y) {
        cout << " " << (char)('A' + y) << " ";
        for (int x = 0; x < 10; ++x) {
            StatoCella stato = griglia.getStatoCella(x, y);
            if (stato == StatoCella::VUOTA) {
                cout << COLORE_BLU << " ~ " << COLORE_RESET;
            } else if (stato == StatoCella::NAVE) {
                if (nascondiNavi) cout << COLORE_BLU << " ~ " << COLORE_RESET;
                else cout << COLORE_VERDE << " O " << COLORE_RESET;
            } else if (stato == StatoCella::COLPITO) {
                cout << COLORE_ROSSO << " X " << COLORE_RESET;
            } else if (stato == StatoCella::MANCATO) {
                cout << COLORE_BIANCO << " * " << COLORE_RESET;
            }
        }
        cout << "\n";
    }
}

bool inputCoordinata(Coordinata& coord) {
    string in;
    cout << "Inserisci coordinata (es. A5 o a5): ";
    if (!(cin >> in)) return false;
    if (in.length() < 2) return false;
    
    char lett = toupper(in[0]);
    char num = in[1];

    if (lett < 'A' || lett > 'J' || num < '0' || num > '9') return false;
    
    coord.y = lett - 'A';
    coord.x = num - '0';
    return true;
}

// ==========================================
// MAIN LOOP DI GIOCO
// ==========================================
int main() {
    abilitaAnsi();
    srand(time(0));

    Giocatore umano("Giocatore");
    Giocatore computer("Computer");

    // Inizializza le flotte
    for (const auto& n : creaFlottaStandard()) {
        umano.getGriglia().aggiungiNave(n);
        computer.getGriglia().aggiungiNave(n);
    }

    // Posizionamento navi
    umano.getGriglia().posizionaNaviCasualmente();
    computer.getGriglia().posizionaNaviCasualmente();

    bool partitaInCorso = true;

    while (partitaInCorso) {
        pulisciSchermo();
        cout << COLORE_CIANO << "=== BATTAGLIA NAVALE ===" << COLORE_RESET << "\n\n";

        cout << COLORE_GIALLO << "IL TUO CAMPO DI BATTAGLIA:" << COLORE_RESET << "\n";
        mostraGriglia(umano.getGriglia(), false);

        cout << "\n" << COLORE_GIALLO << "CAMPO NEMICO:" << COLORE_RESET << "\n";
        mostraGriglia(computer.getGriglia(), true);

        // ------------------------------------------
        // Turno Umano
        // ------------------------------------------
        Coordinata bersaglio;
        Nave* colpita = nullptr;

        cout << "\nE' il tuo turno! ";
        while (!inputCoordinata(bersaglio)) {
            cout << "Input non valido. Riprova: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        int esito = computer.getGriglia().attaccaCella(bersaglio, colpita);

        if (esito == -1) {
            cout << "Hai già sparato qui! Perdi il turno.\n";
            umano.registraColpo(false);
        } else if (esito == 0) {
            cout << COLORE_BIANCO << "Mancato!" << COLORE_RESET << "\n";
            umano.registraColpo(false);
        } else if (esito == 1) {
            cout << COLORE_ROSSO << "Colpito! " << colpita->getNome() << " danneggiata!" << COLORE_RESET << "\n";
            umano.registraColpo(true);
        } else if (esito == 2) {
            cout << COLORE_ROSSO << "Colpito e AFFONDATO! Hai distrutto la " << colpita->getNome() << "!" << COLORE_RESET << "\n";
            umano.registraColpo(true);
        }

        if (computer.getGriglia().tutteNaviAffondate()) {
            cout << "\n" << COLORE_VERDE << "COMPLIMENTI! Hai vinto la partita!" << COLORE_RESET << "\n";
            partitaInCorso = false;
            break;
        }

        // ------------------------------------------
        // Turno Computer
        // ------------------------------------------
        Coordinata bersaglioComp;
        int esitoComp = -1;

        while (esitoComp == -1) {
            bersaglioComp.x = rand() % 10;
            bersaglioComp.y = rand() % 10;
            esitoComp = umano.getGriglia().attaccaCella(bersaglioComp, colpita);
        }

        cout << "\nIl nemico spara in " << (char)('A' + bersaglioComp.y) << bersaglioComp.x << "... ";

        if (esitoComp == 0) {
            cout << "Mancato!\n";
        } else if (esitoComp == 1) {
            cout << "Ha COLPITO una tua nave!\n";
        } else if (esitoComp == 2) {
            cout << "Ha AFFONDATO una tua nave!\n";
        }

        if (umano.getGriglia().tutteNaviAffondate()) {
            cout << "\n" << COLORE_ROSSO << "IL NEMICO HA VINTO! La tua flotta è distrutta." << COLORE_RESET << "\n";
            partitaInCorso = false;
        }

        // Fine turno
        cout << "\nPremi Invio per continuare il prossimo turno...";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
    }

    return 0;
}
