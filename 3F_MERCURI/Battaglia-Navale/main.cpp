#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <cctype>
#include <iomanip>

#ifdef _WIN32
#include <windows.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#endif

using namespace std;

// ==========================================
// CONFIGURAZIONE COLORI ANSI E STILE
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

const string COLORE_RESET   = "\033[0m";
const string COLORE_GRIGIO  = "\033[90m";
const string COLORE_BLU     = "\033[34m"; // Acqua mare
const string COLORE_ROSSO   = "\033[31m"; // Colpo a segno / Nemico
const string COLORE_BIANCO  = "\033[37m"; // Mancato
const string COLORE_VERDE   = "\033[32m"; // Nave / Vittoria
const string COLORE_GIALLO  = "\033[33m"; // Intestazioni / Evidenziato
const string COLORE_CIANO   = "\033[36m"; // Titoli e BORDURA
const string COLORE_MAGENTA = "\033[35m"; // Statistiche

void attendiInvio() {
    cout << "\n" << COLORE_GRIGIO << "Premi INVIO per continuare..." << COLORE_RESET;
    cin.get();
}

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
// UTILITY DI INTERFACCIA E GRAFICA
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

void mostraBanner() {
    cout << COLORE_CIANO;
    cout << "====================================================\n";
    cout << "  ____   _  _____ _____  _    ____ _     ___    _   \n";
    cout << " | __ ) / \\|_   _|_   _|/ \\  / ___| |   |_ _|  / \\  \n";
    cout << " |  _ \\/ _ \\ | |   | | / _ \\| |  _| |    | |  / _ \\ \n";
    cout << " | |_) / ___ \\| |   | |/ ___ \\ |_| | |___ | | / ___ \\\n";
    cout << " |____/_/   \\_\\_|   |_/_/   \\_\\____|_____|___/_/   \\_\\\n";
    cout << "                 N A V A L E                        \n";
    cout << "====================================================\n" << COLORE_RESET;
}

void mostraGriglia(const Griglia& griglia, bool nascondiNavi) {
    cout << "     0  1  2  3  4  5  6  7  8  9 \n";
    cout << "   +------------------------------+\n";

    for (int y = 0; y < 10; ++y) {
        cout << " " << (char)('A' + y) << " |";
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
        cout << "|\n";
    }
    cout << "   +------------------------------+\n";
}

bool inputCoordinata(Coordinata& coord) {
    string in;
    cout << COLORE_GIALLO << "Inserisci coordinata (es. A5): " << COLORE_RESET;
    if (!(cin >> in)) return false;
    
    char lett = ' ';
    string numStr = "";

    for (char c : in) {
        if (isalpha(c) && lett == ' ') {
            lett = toupper(c);
        } else if (isdigit(c)) {
            numStr += c;
        }
    }

    if (lett < 'A' || lett > 'J' || numStr.empty()) return false;

    int valNum = stoi(numStr);
    if (valNum < 0 || valNum > 9) return false;

    coord.y = lett - 'A';
    coord.x = valNum;
    return true;
}

void mostraRegole() {
    pulisciSchermo();
    mostraBanner();
    cout << COLORE_GIALLO << "\n--- REGOLE DI GIOCO ---\n" << COLORE_RESET;
    cout << "1. Il tabellone e' una griglia 10x10 (righe A-J, colonne 0-9).\n";
    cout << "2. Entrambi i giocatori possiedono 5 navi posizionate casualmente:\n";
    cout << "   - Portaerei (5 caselle)\n";
    cout << "   - Corazzata (4 caselle)\n";
    cout << "   - Incrociatore (3 caselle)\n";
    cout << "   - Sottomarino (3 caselle)\n";
    cout << "   - Cacciatorpediniere (2 caselle)\n";
    cout << "3. Legenda Simboli:\n";
    cout << "   " << COLORE_BLU << "~" << COLORE_RESET << " : Acqua / Non esplorato\n";
    cout << "   " << COLORE_VERDE << "O" << COLORE_RESET << " : La tua nave\n";
    cout << "   " << COLORE_ROSSO << "X" << COLORE_RESET << " : Colpo a segno!\n";
    cout << "   " << COLORE_BIANCO << "*" << COLORE_RESET << " : Acqua (Colpo mancato)\n";
    cout << "4. Vince chi affonda per primo tutte le navi nemiche!\n";
    attendiInvio();
}

void mostraStatistichePartita(const Giocatore& u, const Giocatore& c) {
    pulisciSchermo();
    mostraBanner();
    cout << COLORE_MAGENTA << "\n--- STATISTICHE ULTIMA PARTITA ---\n" << COLORE_RESET;
    
    auto calcolaAccuratezza = [](int colpi, int messiA) {
        if (colpi == 0) return 0.0;
        return (double)messiA / colpi * 100.0;
    };

    cout << "\n[ Tu ]\n";
    cout << " - Colpi totali sparati : " << u.getColpiSparati() << "\n";
    cout << " - Colpi a segno        : " << u.getColpiASegno() << "\n";
    cout << " - Colpi mancati        : " << u.getMancati() << "\n";
    cout << fixed << setprecision(1);
    cout << " - Accuratezza          : " << calcolaAccuratezza(u.getColpiSparati(), u.getColpiASegno()) << "%\n";

    cout << "\n[ Computer ]\n";
    cout << " - Colpi totali sparati : " << c.getColpiSparati() << "\n";
    cout << " - Colpi a segno        : " << c.getColpiASegno() << "\n";
    cout << " - Accuratezza          : " << calcolaAccuratezza(c.getColpiSparati(), c.getColpiASegno()) << "%\n";

    attendiInvio();
}

// ==========================================
// LOOP DEL GIOCO PRINCIPALE
// ==========================================
void giocaPartita(Giocatore& umano, Giocatore& computer) {
    umano.azzeraStatistiche();
    computer.azzeraStatistiche();

    for (const auto& n : creaFlottaStandard()) {
        umano.getGriglia().aggiungiNave(n);
        computer.getGriglia().aggiungiNave(n);
    }

    umano.getGriglia().posizionaNaviCasualmente();
    computer.getGriglia().posizionaNaviCasualmente();

    bool partitaInCorso = true;
    string messaggioEsitoGiocatore = "";
    string messaggioEsitoComputer = "";

    while (partitaInCorso) {
        pulisciSchermo();
        mostraBanner();

        cout << COLORE_GIALLO << "\nIL TUO CAMPO:" << COLORE_RESET << "\n";
        mostraGriglia(umano.getGriglia(), false);

        cout << "\n" << COLORE_GIALLO << "CAMPO NEMICO:" << COLORE_RESET << "\n";
        mostraGriglia(computer.getGriglia(), true);

        if (!messaggioEsitoGiocatore.empty()) {
            cout << "\n" << messaggioEsitoGiocatore << "\n";
        }
        if (!messaggioEsitoComputer.empty()) {
            cout << messaggioEsitoComputer << "\n";
        }

        // --- Turno Giocatore ---
        Coordinata bersaglio;
        Nave* colpita = nullptr;

        while (!inputCoordinata(bersaglio)) {
            cout << COLORE_ROSSO << "Coordinata non valida! Usare un formato come A5 o B2: " << COLORE_RESET;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        int esito = computer.getGriglia().attaccaCella(bersaglio, colpita);

        if (esito == -1) {
            messaggioEsitoGiocatore = COLORE_BIANCO + "Hai gia' sparato qui! Turno sprecato." + COLORE_RESET;
            umano.registraColpo(false);
        } else if (esito == 0) {
            messaggioEsitoGiocatore = COLORE_BIANCO + "> Tu: Acqua! Nessun bersaglio colpito." + COLORE_RESET;
            umano.registraColpo(false);
        } else if (esito == 1) {
            messaggioEsitoGiocatore = COLORE_ROSSO + "> Tu: COLPITO! " + colpita->getNome() + " danneggiata!" + COLORE_RESET;
            umano.registraColpo(true);
        } else if (esito == 2) {
            messaggioEsitoGiocatore = COLORE_ROSSO + "> Tu: COLPITO E AFFONDATO! Hai distrutto la " + colpita->getNome() + "!" + COLORE_RESET;
            umano.registraColpo(true);
        }

        if (computer.getGriglia().tutteNaviAffondate()) {
            pulisciSchermo();
            mostraBanner();
            cout << "\n" << COLORE_VERDE << "******************************************\n";
            cout << "   COMPLIMENTI! HAI VINTO LA PARTITA!\n";
            cout << "******************************************" << COLORE_RESET << "\n";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            attendiInvio();
            break;
        }

        // --- Turno Computer ---
        Coordinata bersaglioComp;
        int esitoComp = -1;

        while (esitoComp == -1) {
            bersaglioComp.x = rand() % 10;
            bersaglioComp.y = rand() % 10;
            esitoComp = umano.getGriglia().attaccaCella(bersaglioComp, colpita);
        }

        string coordCompStr = string(1, (char)('A' + bersaglioComp.y)) + to_string(bersaglioComp.x);

        if (esitoComp == 0) {
            messaggioEsitoComputer = COLORE_GRIGIO + "> Nemico (" + coordCompStr + "): Acqua!" + COLORE_RESET;
            computer.registraColpo(false);
        } else if (esitoComp == 1) {
            messaggioEsitoComputer = COLORE_ROSSO + "> Nemico (" + coordCompStr + "): Ha COLPITO una tua nave!" + COLORE_RESET;
            computer.registraColpo(true);
        } else if (esitoComp == 2) {
            messaggioEsitoComputer = COLORE_ROSSO + "> Nemico (" + coordCompStr + "): Ha AFFONDATO una tua nave!" + COLORE_RESET;
            computer.registraColpo(true);
        }

        if (umano.getGriglia().tutteNaviAffondate()) {
            pulisciSchermo();
            mostraBanner();
            cout << "\n" << COLORE_ROSSO << "******************************************\n";
            cout << "   SCONFITTA! La tua flotta e' distrutta.\n";
            cout << "******************************************" << COLORE_RESET << "\n";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            attendiInvio();
            break;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// ==========================================
// MAIN CON MENU INIZIALE
// ==========================================
int main() {
    abilitaAnsi();
    srand(time(0));

    Giocatore umano("Giocatore");
    Giocatore computer("Computer");
    bool haGiocatoPartita = false;

    while (true) {
        pulisciSchermo();
        mostraBanner();

        cout << "\n" << COLORE_GIALLO << "=== MENU PRINCIPALE ===" << COLORE_RESET << "\n";
        cout << " 1. Nuova Partita\n";
        cout << " 2. Come Giocare (Regole)\n";
        cout << " 3. Statistiche Ultima Partita\n";
        cout << " 0. Esci dal gioco\n";
        cout << "\nSeleziona un'opzione (0-3): ";

        char scelta;
        if (!(cin >> scelta)) break;

        // Pulisce il buffer di input dopo aver letto 'scelta'
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (scelta == '1') {
            giocaPartita(umano, computer);
            haGiocatoPartita = true;
        } else if (scelta == '2') {
            mostraRegole();
        } else if (scelta == '3') {
            if (haGiocatoPartita) {
                mostraStatistichePartita(umano, computer);
            } else {
                cout << COLORE_ROSSO << "\nNessuna partita giocata in questa sessione!\n" << COLORE_RESET;
                attendiInvio();
            }
        } else if (scelta == '0') {
            cout << "\nGrazie per aver giocato! Alla prossima.\n\n";
            break;
        } else {
            cout << COLORE_ROSSO << "\nScelta non valida! Riprova.\n" << COLORE_RESET;
            attendiInvio();
        }
    }

    return 0;
}
