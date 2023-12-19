#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
using namespace std;

int toSeconds(string month, int day, int hr, int min, int sec)
{

    map<string, int> months = {
        {"Jun", 1},
        {"Jul", 2},
        {"Aug", 3},
        {"Sep", 4},
        {"Oct", 5},
    };

    int hours2seconds = hr * 3600 + min * 60 + sec;
    int days2seconds = day * 86400;
    int month2seconds = months[month] * 2629743;

    int totalsec = hours2seconds + days2seconds + month2seconds;

    return totalsec;
}

void BubbleSort(vector<int> &registroSegundos, vector<string> &data)
{
    for (int i = 0; i < registroSegundos.size(); i++)
    {

        // Last i elements are already in place
        for (int j = 0; j < registroSegundos.size() - i; j++)
        {

            // Swap if the element found is greater than the next element
            if (registroSegundos[j] > registroSegundos[j + 1])
            {
                // Swap registroSegundos
                int swapSegundos = registroSegundos[j];
                registroSegundos[j] = registroSegundos[j + 1];
                registroSegundos[j + 1] = swapSegundos;

                // Swap data
                string swapData = data[j];
                data[j] = data[j + 1];
                data[j + 1] = swapData;
            }
        }
    }
}

void busqBinariaIterativo(vector<int> &registroSegundos, int starDate, int endingDate, vector<string> data)
{  
    cout << "Dates in the specified range:" << endl;

    for (int i = 0; i < registroSegundos.size(); i++)
    {
        int timestamp = registroSegundos[i];

        if (timestamp >= (starDate) && timestamp <= (endingDate))
        {
            cout << data[i] << endl;
        }
    }
}

int main()
{
    string line;
    vector<string> data;
    vector<int> registroSegundos; // Vector para almacenar los segundos
    string month, hour;
    int hr, min, sec, day;
    int starDate, endingDate;
    ifstream archivo("bitacora.txt");
    if (!archivo.is_open())
    {
        cout << "Error al abrir archivo" << endl;
        return 1;
    }

    while (getline(archivo, line))
    {
        data.push_back(line); // Leer cada línea y almacenarla en el vector 'data'
    }

    archivo.close(); // Cerrar el archivo después de leer

    // Separar strings
    char separador;
    for (int i = 0; i < data.size(); i++)
    {
        string elemento = data[i];
        stringstream newlineHour(elemento);
        newlineHour >> month >> day >> hour; // Leer mes, día y hora

        newlineHour.clear();                                       // Limpiar el stringstream
        newlineHour.str(hour);                                     // Cambiar el contenido del stringstream a 'hour'
        newlineHour >> hr >> separador >> min >> separador >> sec; // Leer hora, minutos y segundos

        int conversion = toSeconds(month, day, hr, min, sec);

        registroSegundos.push_back(conversion);
    }

    // Aplicar Bubble Sort para ordenar los registros por hora
    BubbleSort(registroSegundos, data);

    ofstream outputFile("ordenados.txt");

    for (int i = 0; i < data.size(); i++) {
        outputFile << data[i] << endl;
    }

    string startMonth; int startDay;
    cout << "Ingresa fecha de inicio (ej. Oct 29): ";
    cin >> startMonth >> startDay;

    // Convertir
    starDate = toSeconds(startMonth, startDay, 0, 0, 0);

    cout << " Ingresa fecha final (ej. Oct 29): ";
    string endMonth;
    int endDay;
    cin >> endMonth >> endDay;

    // Convertir
    endingDate = toSeconds(endMonth, endDay, 23, 59, 59); // 23 hrs, 59 min, 59 sec

    busqBinariaIterativo(registroSegundos, starDate, endingDate, data);

    // Imprimir los registros ordenados

    return 0;
}