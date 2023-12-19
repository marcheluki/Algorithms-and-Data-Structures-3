#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <sstream>
#include <algorithm>
#include <string>
using namespace std;

const int maxVert = 999;

struct Node {
    int section;
    int outDegree;
    Node (int s, int o) {
        section = s;
        outDegree = o;
    }
};

class Graph {
    private:
        int numVert;
        list<int> *adjList;
    
    public:
        Graph(){
            numVert = 0;
            adjList = new list<int>[maxVert+1];
        }

        void addEdge(int startVertex, int endVertex){
            if (startVertex >= 0 && startVertex < maxVert && endVertex >= 0 && endVertex < maxVert) {
                adjList[startVertex].push_back(endVertex);
            } else {
                cerr << "Vértices no permitidos" << endl;
            }  
        }

        void printAdjList(){
            for(int i = 0; i <= maxVert; i++){
                cout << i << ": ";
                for(list<int>::iterator it = adjList[i].begin(); it != adjList[i].end(); it++){
                    cout << *it << " ";
                }
                cout << endl;
            }
        }

        int outDegree(){
            int outDegree;
            vector<Node> outDegreeVector;


            for(int i = 0; i <= maxVert; i++){
                outDegree = adjList[i].size();
                Node node(i, outDegree);
                outDegreeVector.push_back(node);
            }

            sort(outDegreeVector.begin(), outDegreeVector.end(), 
            [](const Node &a, const Node &b) {return a.outDegree > b.outDegree;});

            for(int i = 0; i < 10; i++){
                cout << "IP segment: " << outDegreeVector[i].section << ", Out-degree value: " << outDegreeVector[i].outDegree << endl;
            }

            int maxOutDegree = outDegreeVector[0].outDegree;

            return maxOutDegree;
        }

};

int main(){
    

    ifstream inputFile("bitacora.txt");
    if (!inputFile.is_open()) {
        cerr << "Error: No se pudo arbrir el archivo" << endl;
        return 1;
    }

    Graph g;

    string mes, fecha, ipAddress, razon, ipComplete; 
    int dia, startVertex, endVertex;

    cout << "Direcciones IP donde se encuentra el boot master: " << endl;

    while (inputFile >> mes >> dia >> fecha >> ipAddress) { 
        getline(inputFile, razon);  
        razon = razon.substr(1);
        ipComplete = ipAddress.substr(0, ipAddress.find(":")); //obtener el ip address completo, sin port number
        istringstream ipStream(ipComplete);
        int ipSections[4];
        char dot;
        for (int i = 0; i < 4; i++) {
            ipStream >> ipSections[i];
            if (i < 3) {
                ipStream >> dot;
            }
        }

        bool contains60 = false;

        for (int i = 0; i < 3; i++) {
            startVertex = ipSections[i];
            endVertex = ipSections[i+1];
            g.addEdge(startVertex, endVertex);

            if (startVertex == 60 || endVertex == 60) {
                contains60 = true;
            }
        }  

        if (contains60) {
            cout << mes << " " << dia << " " << fecha << " " << ipAddress << " " << razon << endl;
        }

    }

    cout << "\nTop 10 direcciones IP con out-degree value más grande: " << endl;

    g.outDegree();

    inputFile.close();

    return 0;
}