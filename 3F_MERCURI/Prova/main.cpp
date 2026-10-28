#include <iostream>
#include <cmath>

using namespace std;

int main() {
	
	int scelta, potenza, guadagno, barriere, oreLavoro, costoManodopera, supplementoSicurezza, supplementoQuota, totale, tariffa;
	char lavoroInQuota;
	
	cout << "*** GESTIONALE RADIOTECH ***\n";
	cout << "[1] Analisi Sicurezza Radio\n";
	cout << "[2] Calcolo Preventivo\n";
	cout << "============================\n";
	cout << "Inserisci scelta: ";
	cin >> scelta;
	
	switch(scelta) {
		case 1: {
			cout << "... AVVIO MODULO 1: SICUREZZA RADIO\n";
			cout << "Inserisci potenza (W): ";
			cin >> potenza;
			cout << "\nInserisci guadagno: ";
			cin >> guadagno;

			double peff = potenza * pow(guadagno, 2);
			double raggio = sqrt(peff / 10);
			barriere = round(raggio);
			
			cout << "Risultati\n";
			cout << "Potenza efficace: " << peff << " W" << endl;
			cout << "Raggio sicurezza esatto: " << raggio << " m" << endl;
			cout << "Barriere necessarie: " << barriere << endl;
			break;	
		}
			
		case 2: {
			cout << "... AVVIO MODULO 2: preventivo\n";
			cout << "Inserisci ore lavorative: ";
			cin >> oreLavoro; 
			cout << "\nLavoro in quota? (S/N): ";
			cin >> lavoroInQuota;
			
			if(oreLavoro <= 5) {
				tariffa = 50;
			}
			else if (oreLavoro >= 6 && oreLavoro <= 10) {
				tariffa = 45;
			} else if (oreLavoro > 10) {
				tariffa = 40;
			}
			
			costoManodopera = oreLavoro * tariffa;
			if(lavoroInQuota == 'S' || lavoroInQuota == 's') {
				supplementoQuota = 150;
			} else {
				supplementoQuota = 0;
			}
			totale = costoManodopera + supplementoQuota;
			
			cout << "\n";
			cout << "Risultati:\n";
			cout << "Costo manodopera (" << tariffa << " EUR/h): " << costoManodopera << " EUR" << endl;
			cout << "Supplemento quota: " << supplementoQuota << " EUR" << endl;
			cout << "TOTALE PREVENTIVO: " << totale << " EUR" << endl;
			break;
		}
			
		default:
			cout << "Inserisci un'opzione valida." << endl;
	}
	return 0;
}

