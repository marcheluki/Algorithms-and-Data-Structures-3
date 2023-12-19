#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

class bitacoraEntrada {
public:
    string mes;
    int dia;
    string fecha;
    string ipAddress;
    string razon;
    bitacoraEntrada* next;
    bitacoraEntrada* prev;

    bitacoraEntrada(string m, int d, string t, string ip, string r) : mes(m), dia(d), fecha(t), ipAddress(ip), razon(r), next(nullptr), prev(nullptr) {}
};

class bitacoraLista {
public:
    bitacoraEntrada* head;
    bitacoraEntrada* tail;

    bitacoraLista() : head(nullptr), tail(nullptr) {}

    void addOrderedEntry(string m, int d, string t, string ip, string r) {
        bitacoraEntrada* newEntry = new bitacoraEntrada(m, d, t, ip, r);

        if (!head) {
            head = tail = newEntry;
        } else {
            bitacoraEntrada* current = head;

            while (current && ip > current->ipAddress) {
                current = current->next;
            }

            if (!current) {
                // inserta al final
                newEntry->prev = tail;
                tail->next = newEntry;
                tail = newEntry;
            } else if (current == head) {
                // inserta al principio
                newEntry->next = head;
                head->prev = newEntry;
                head = newEntry;
            } else {
                // inserta en medio
                newEntry->next = current;
                newEntry->prev = current->prev;
                current->prev->next = newEntry;
                current->prev = newEntry;
            }
        }
    }

    void displayEntries() {
        bitacoraEntrada* current = head;
        while (current != nullptr) {
            cout << "Mes: " << current->mes << endl;
            cout << "Día: " << current->dia << endl;
            cout << "Fecha: " << current->fecha << endl;
            cout << "IP: " << current->ipAddress << endl;
            cout << "Razón: " << current->razon << endl;
            cout << "----------------------" << endl;
            current = current->next;
        }
    }

    void filterAndDisplayEntries(string startIP, string endIP) {
        bitacoraEntrada* current = head;

        while (current != nullptr && current->ipAddress <= endIP) {
            if (current->ipAddress >= startIP) {
                cout << "Mes: " << current->mes << endl;
                cout << "Día: " << current->dia << endl;
                cout << "Fecha: " << current->fecha << endl;
                cout << "IP: " << current->ipAddress << endl;
                cout << "Razón: " << current->razon << endl;
                cout << "----------------------" << endl;
            }
            current = current->next;
        }
    }

    void saveOrderedDataToFile(const string& filename) {
        ofstream outputFile(filename);
        bitacoraEntrada* current = head;

        while (current != nullptr) {
            outputFile << current->mes << " " << current->dia << " " << current->fecha << " " << current->ipAddress << " " << current->razon << endl;
            current = current->next;
        }

        outputFile.close();
    }

    ~bitacoraLista() {
        bitacoraEntrada* current = head;
        while (current != nullptr) {
            bitacoraEntrada* temp = current;
            current = current->next;
            delete temp;
        }
    }
};

int main() {
    ifstream inputFile("bitacora.txt");
    if (!inputFile.is_open()) {
        cerr << "Error: No se pudo abrir el archivo." << endl;
        return 1;
    }

    bitacoraLista logList;
    string mes, fecha, ipAddress, razon;
    int dia;

    while (inputFile >> mes >> dia >> fecha >> ipAddress) { // lee lo demás como razon
        getline(inputFile, razon);  // quita el espacio de razon
        razon = razon.substr(1);
        logList.addOrderedEntry(mes, dia, fecha, ipAddress, razon);
    }

    inputFile.close();
    logList.displayEntries(); // muestra los datos ordenados, se puede comentar para que no se imprima la base de datos completa
    string startIP, endIP;
    cout << "Inicio de la busqueda del IP: ";
    cin >> startIP;
    cout << "Final de la busqueda del IP: ";
    cin >> endIP;

    logList.filterAndDisplayEntries(startIP, endIP);

    logList.saveOrderedDataToFile("ordenadas.txt");

    return 0;
}
