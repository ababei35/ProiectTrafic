#include <iostream>
#include <cstdlib>
#include <string>
#include <ctime>
#include <fstream> // Pentru lucrul cu fisiere (Rapoarte)

// Culori ANSI pentru Linux
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"
#define MAGENTA "\033[35m" // Pentru ambulanta

using namespace std;

// === ETAPA V: Sistemul de Jurnalizare (Logare in fisier) ===
void scrieInRaport(string mesaj) {
    ofstream fisier("raport_trafic.txt", ios::app); // Deschide in modul "append" (adauga la final)
    if (fisier.is_open()) {
        fisier << mesaj << "\n";
        fisier.close();
    }
}

// === ETAPA IV & V: Clase Avansate (Vehicul, Semafor, Intersectie) ===

class Vehicul {
public:
    bool esteAmbulanta;
    Vehicul(bool prioritate = false) {
        esteAmbulanta = prioritate;
    }
};

class Semafor {
private:
    string directie;
    string culoareCurenta;
    int masiniInAsteptare;
    bool areAmbulanta; // Flag pentru vehicule speciale

public:
    Semafor(string dir) {
        directie = dir;
        culoareCurenta = "ROSU";
        masiniInAsteptare = 0;
        areAmbulanta = false;
    }

    string getDirectie() { return directie; }
    string getCuloare() { return culoareCurenta; }
    int getNumarMasini() { return masiniInAsteptare; }
    bool getAmbulanta() { return areAmbulanta; }

    void schimbaCuloare(string nouaCuloare) {
        culoareCurenta = nouaCuloare;
        scrieInRaport("-> Semaforul " + directie + " a devenit " + nouaCuloare);
    }

    void generareTraficAleator() {
        // Generam intre 0 si 5 masini normale
        int masiniNoi = rand() % 6; 
        masiniInAsteptare += masiniNoi;

        // Sansa de 10% sa apara o ambulanta
        if (rand() % 100 < 10 && !areAmbulanta) {
            areAmbulanta = true;
            scrieInRaport("[!] O ambulanta a ajuns la semaforul " + directie);
        }

        if(masiniNoi > 0) {
           scrieInRaport("Au ajuns " + to_string(masiniNoi) + " masini la semaforul " + directie);
        }
    }

    void scurgeTrafic() {
        if (culoareCurenta == "VERDE") {
            int capacitateTrecere = 8; // Trec max 8 masini pe tur
            
            // Daca are ambulanta, trece prima (o resetam)
            if (areAmbulanta) {
                areAmbulanta = false;
                scrieInRaport("[!] Ambulanta a trecut prin intersectie pe directia " + directie);
                capacitateTrecere -= 1; // Ambulanta a consumat un "loc"
            }

            int masiniCareAuTrecut = min(masiniInAsteptare, capacitateTrecere);
            masiniInAsteptare -= masiniCareAuTrecut;
            
            if (masiniCareAuTrecut > 0) {
                scrieInRaport("Au trecut " + to_string(masiniCareAuTrecut) + " masini pe directia " + directie);
            }
        }
    }

    void afiseazaStare() {
        cout << "Directia " << directie << " | ";
        if (culoareCurenta == "ROSU") cout << RED << "[ROSU] " << RESET;
        else cout << GREEN << "[VERDE]" << RESET;
        
        cout << " | Masini: " << YELLOW << masiniInAsteptare << RESET;
        
        if (areAmbulanta) {
            cout << MAGENTA << " [!!! AMBULANTA IN ASTEPTARE !!!]" << RESET;
        }
        cout << endl;
    }
};

// Clasa Controller care gestioneaza logica "Inteligenta" a intregii intersectii
class Intersectie {
private:
    Semafor semaforNS;
    Semafor semaforEV;

public:
    Intersectie() : semaforNS("Nord-Sud"), semaforEV("Est-Vest") {
        scrieInRaport("\n--- SISTEM PORNIT ---");
    }

    ~Intersectie() {
        scrieInRaport("--- SISTEM OPRIT ---\n");
    }

    void afiseazaHarta() {
        cout << "\n--- STARE CURENTA INTERSECTIE ---\n";
        semaforNS.afiseazaStare();
        semaforEV.afiseazaStare();
        cout << "-------------------------------------------------\n\n";
    }

    void controlManual(int directie) {
        if (directie == 1) { // NS Verde
            semaforNS.schimbaCuloare("VERDE");
            semaforEV.schimbaCuloare("ROSU");
        } else if (directie == 2) { // EV Verde
            semaforEV.schimbaCuloare("VERDE");
            semaforNS.schimbaCuloare("ROSU");
        }
    }

    // Aici este inteligenta artificiala a sistemului
    void simularePasTimp() {
        cout << YELLOW << "\nSe proceseaza traficul..." << RESET << "\n";
        
        // 1. Generam trafic nou
        semaforNS.generareTraficAleator();
        semaforEV.generareTraficAleator();

        // 2. LOGICA INTELIGENTA (Prioritati)
        if (semaforNS.getAmbulanta()) {
            cout << MAGENTA << "Sistem override: Ambulanta detectata pe Nord-Sud! Comutare automata." << RESET << "\n";
            controlManual(1);
        } else if (semaforEV.getAmbulanta()) {
            cout << MAGENTA << "Sistem override: Ambulanta detectata pe Est-Vest! Comutare automata." << RESET << "\n";
            controlManual(2);
        } 
        // Daca nu sunt ambulante, optimizam in functie de aglomeratie
        else {
            if (semaforNS.getNumarMasini() > 15 && semaforNS.getCuloare() == "ROSU") {
                 cout << YELLOW << "Sistem override: Aglomeratie detectata Nord-Sud. Comutare automata." << RESET << "\n";
                 controlManual(1);
            } else if (semaforEV.getNumarMasini() > 15 && semaforEV.getCuloare() == "ROSU") {
                 cout << YELLOW << "Sistem override: Aglomeratie detectata Est-Vest. Comutare automata." << RESET << "\n";
                 controlManual(2);
            }
        }

        // 3. Masinile care au verde trec
        semaforNS.scurgeTrafic();
        semaforEV.scurgeTrafic();
    }
};

void curataEcran() {
    system("clear"); 
}

int main() {
    srand(time(0)); 
    Intersectie intersectiaPrincipala;
    
    int optiune;
    bool ruleaza = true;

    while (ruleaza) {
        curataEcran();
        cout << BLUE << "=================================================" << RESET << endl;
        cout << GREEN << "      SISTEM MANAGEMENT TRAFIC INTELIGENT v3" << RESET << endl;
        cout << BLUE << "=================================================" << RESET << endl;
        
        intersectiaPrincipala.afiseazaHarta();

        cout << YELLOW << "1." << RESET << " Comutare Nord-Sud pe VERDE\n";
        cout << YELLOW << "2." << RESET << " Comutare Est-Vest pe VERDE\n";
        cout << YELLOW << "3." << RESET << " SIMULEAZA TRAFIC (Avansare timp)\n";
        cout << YELLOW << "0." << RESET << " Iesire si generare Raport Fisier\n";
        cout << "-------------------------------------------------\n";
        cout << CYAN << "Alegeti o optiune: " << RESET;

        if (!(cin >> optiune)) {
            cin.clear();
            cin.ignore(10000, '\n');
            optiune = -1; 
        }

        switch (optiune) {
            case 1:
                intersectiaPrincipala.controlManual(1);
                break;
            case 2:
                intersectiaPrincipala.controlManual(2);
                break;
            case 3:
                intersectiaPrincipala.simularePasTimp();
                break;
            case 0:
                cout << "\nSistemul se inchide. Verificati fisierul 'raport_trafic.txt' pentru istoric.\n";
                ruleaza = false;
                break;
            default:
                cout << RED << "\nOptiune invalida!" << RESET << "\n";
        }

        if (ruleaza) {
            cout << "\nApasati [ENTER] pentru a continua...";
            cin.ignore(10000, '\n');
            cin.get();
        }
    }

    return 0;
}g++ main.cpp -o trafic