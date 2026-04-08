#include <iostream>
#include <cstdlib>
#include <string>
#include <ctime>
#include <fstream>
#include <iomanip> // Pentru formatarea ceasului

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m" // Ambulanta
#define CYAN    "\033[36m" // Politie
#define WHITE   "\033[37m" // Pietoni

using namespace std;

void scrieInLog(string mesaj) {
    ofstream fisier("raport_trafic.txt", ios::app);
    if (fisier.is_open()) {
        fisier << mesaj << "\n";
        fisier.close();
    }
}

class Semafor {
private:
    string directie;
    string culoare;
    int masini;
    bool ambulanta, politie;

public:
    Semafor(string dir) : directie(dir), culoare("ROSU"), masini(0), ambulanta(false), politie(false) {}

    string getCuloare() { return culoare; }
    int getMasini() { return masini; }
    bool areUrgenta() { return ambulanta || politie; }
    bool areAmbulanta() { return ambulanta; }
    bool arePolitie() { return politie; }

    void setCuloare(string c) { 
        culoare = c; 
        scrieInLog("Semafor " + directie + " -> " + c);
    }

    void adaugaTrafic(int n, bool isRushHour) {
        int bonus = isRushHour ? 4 : 0; // Mai multe masini la ora de varf
        int noi = rand() % (6 + bonus);
        masini += noi;
        
        if (rand() % 100 < 5) ambulanta = true;
        if (rand() % 100 < 8) politie = true;
    }

    void proceseazaTrecere(int rata) {
        if (culoare == "VERDE") {
            if (ambulanta) ambulanta = false;
            else if (politie) politie = false;
            int trecute = min(masini, rata);
            masini -= trecute;
        }
    }

    void afisare() {
        cout << "Directia " << directie << " | ";
        if (culoare == "ROSU") cout << RED << "[ROSU] " << RESET;
        else cout << GREEN << "[VERDE]" << RESET;
        cout << " | Masini: " << YELLOW << masini << RESET;
        if (ambulanta) cout << MAGENTA << " [AMBULANTA]" << RESET;
        if (politie) cout << CYAN << " [POLITIE]" << RESET;
        cout << endl;
    }
};

class Intersectie {
private:
    Semafor ns, ev;
    int ora, minut;
    bool pietoniInTrecere;
    int durataPietoni;

public:
    Intersectie() : ns("Nord-Sud"), ev("Est-Vest"), ora(8), minut(0), pietoniInTrecere(false), durataPietoni(0) {}

    void avanseazaTimp() {
        minut += 15; // Fiecare pas simuleaza 15 minute
        if (minut >= 60) { minut = 0; ora++; }
        if (ora >= 24) ora = 0;

        bool rushHour = (ora >= 7 && ora <= 9) || (ora >= 17 && ora <= 19);
        
        // Generam trafic
        ns.adaugaTrafic(0, rushHour);
        ev.adaugaTrafic(0, rushHour);

        // LOGICA DE CONTROL
        if (pietoniInTrecere) {
            durataPietoni++;
            ns.setCuloare("ROSU");
            ev.setCuloare("ROSU");
            if (durataPietoni >= 2) { // Pietonii trec in 2 pasi de timp
                pietoniInTrecere = false;
                durataPietoni = 0;
                scrieInLog("Trecere pietoni finalizata.");
            }
        } 
        else if (ns.areUrgenta()) { controlManual(1); }
        else if (ev.areUrgenta()) { controlManual(2); }
        else {
            // Inteligenta bazata pe aglomeratie
            if (ns.getMasini() > ev.getMasini()) controlManual(1);
            else controlManual(2);
        }

        ns.proceseazaTrecere(8);
        ev.proceseazaTrecere(8);
    }

    void solicitaPietoni() {
        if (!pietoniInTrecere) {
            pietoniInTrecere = true;
            durataPietoni = 0;
            scrieInLog("(!) Buton pietoni apasat.");
        }
    }

    void controlManual(int d) {
        if (d == 1) { ns.setCuloare("VERDE"); ev.setCuloare("ROSU"); }
        else { ev.setCuloare("VERDE"); ns.setCuloare("ROSU"); }
    }

    void afiseazaDashboard() {
        system("clear");
        cout << BLUE << "=================================================" << RESET << endl;
        cout << GREEN << "    SMART CITY TRAFFIC CONTROL v5.0 - LINUX" << RESET << endl;
        cout << "    Ora curenta: " << YELLOW << setfill('0') << setw(2) << ora << ":" << setw(2) << minut << RESET;
        if ((ora >= 7 && ora <= 9) || (ora >= 17 && ora <= 19)) cout << RED << " [ORA DE VARF]" << RESET;
        cout << "\n" << BLUE << "=================================================" << RESET << endl;

        ns.afisare();
        ev.afisare();
        
        if (pietoniInTrecere) 
            cout << WHITE << "\n[!] STATUS: PIETONI PE TRECERE - TRAFIC BLOCAT" << RESET << endl;
        
        cout << "\n1. Verde NS | 2. Verde EV | 3. PAS TIMP | 4. BUTON PIETONI | 0. EXIT\n";
        cout << "Selectie: ";
    }
};

int main() {
    srand(time(0));
    Intersectie oras;
    int opt;

    while (true) {
        oras.afiseazaDashboard();
        if (!(cin >> opt)) { cin.clear(); cin.ignore(10000, '\n'); continue; }
        if (opt == 0) break;
        if (opt == 1) oras.controlManual(1);
        if (opt == 2) oras.controlManual(2);
        if (opt == 3) oras.avanseazaTimp();
        if (opt == 4) oras.solicitaPietoni();
    }
    return 0;
}