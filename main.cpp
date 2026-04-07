#include <iostream>
#include <cstdlib>
#include <string>

// Culori ANSI pentru consola de Linux
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"

class Semafor {
private:
    std::string directie;
    std::string culoareCurenta;

public:
    Semafor(std::string dir) {
        directie = dir;
        culoareCurenta = "ROSU";
    }

    void schimbaCuloare(std::string nouaCuloare) {
        culoareCurenta = nouaCuloare;
    }

    void afiseazaStare() {
        std::cout << "Semafor " << directie << ": ";
        if (culoareCurenta == "ROSU") {
            std::cout << RED << "[ROSU]" << RESET << std::endl;
        } else if (culoareCurenta == "VERDE") {
            std::cout << GREEN << "[VERDE]" << RESET << std::endl;
        } else {
            std::cout << YELLOW << "[" << culoareCurenta << "]" << RESET << std::endl;
        }
    }
};

void curataEcran() {
    // Comanda pentru a goli consola in LINUX
    system("clear"); 
}

int main() {
    Semafor semaforNordSud("Nord-Sud");
    Semafor semaforEstVest("Est-Vest");

    int optiune;
    bool ruleaza = true;

    while (ruleaza) {
        curataEcran();
        std::cout << BLUE << "=======================================" << RESET << std::endl;
        std::cout << GREEN << "   SISTEM MANAGEMENT TRAFIC RUTIER" << RESET << std::endl;
        std::cout << BLUE << "=======================================" << RESET << std::endl;
        std::cout << YELLOW << "1." << RESET << " Vizualizare stadiu intersectie\n";
        std::cout << YELLOW << "2." << RESET << " Comutare Nord-Sud pe VERDE\n";
        std::cout << YELLOW << "3." << RESET << " Comutare Est-Vest pe VERDE\n";
        std::cout << YELLOW << "0." << RESET << " Iesire din aplicatie\n";
        std::cout << "---------------------------------------\n";
        std::cout << CYAN << "Alegeti o optiune: " << RESET;

        if (!(std::cin >> optiune)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            optiune = -1; 
        }

        std::cout << "\n";

        switch (optiune) {
            case 1:
                std::cout << "--- STARE CURENTA INTERSECTIE ---\n";
                semaforNordSud.afiseazaStare();
                semaforEstVest.afiseazaStare();
                break;
            case 2:
                semaforNordSud.schimbaCuloare("VERDE");
                semaforEstVest.schimbaCuloare("ROSU");
                std::cout << GREEN << "Traficul a fost deschis pe directia Nord-Sud." << RESET << "\n";
                break;
            case 3:
                semaforEstVest.schimbaCuloare("VERDE");
                semaforNordSud.schimbaCuloare("ROSU");
                std::cout << GREEN << "Traficul a fost deschis pe directia Est-Vest." << RESET << "\n";
                break;
            case 0:
                std::cout << "Inchidere sistem... La revedere!\n";
                ruleaza = false;
                break;
            default:
                std::cout << RED << "Optiune invalida! Va rugam selectati o cifra din meniu." << RESET << "\n";
        }

        if (ruleaza) {
            std::cout << "\nApasati [ENTER] pentru a reveni la meniu...";
            std::cin.ignore(10000, '\n');
            std::cin.get();
        }
    }

    return 0;
}